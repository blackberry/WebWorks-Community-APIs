/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <json/reader.h>
#include <json/writer.h>
#include <slog2.h>
/*added*/
#include <vector>
#include <sstream>
#include <sys/dir.h>
/*added*/

// minizip
#include "unzip.h"
#include "zip.h"
#define S2J_ENABLED 1
#include "state2json.h"

#include "extractzipfile_ndk.hpp"
#include "extractzipfile_js.hpp"


static void ExtractZipFileNDK_mkpath(const char *path_raw) {
	size_t len = strlen(path_raw);

	// copy path so we can mangle it
	char *path = (char *)malloc(len + 1);
	snprintf(path, len + 1, "%s", path_raw);

	char last_char = path[len - 1];
	if (last_char == '/')
		path[len - 1] = '\0';

	// mkdir() foreach except last folder
	for (char *upto = path + 1; *upto != '\0'; upto++) {
		if (*upto == '/') {
			*upto = '\0';
			mkdir(path, 0777);
			*upto = '/';
		}
	}

	// mkdir() last folder
	mkdir(path, 0777);
	free(path);
}

namespace webworks {

ExtractZipFileNDK::ExtractZipFileNDK(ExtractZipFileJS *parent) {
	m_pParent = parent;
}

ExtractZipFileNDK::~ExtractZipFileNDK() {
}

// ->extractFile
// Returns a json obejct with "result" set to int ret code
// ret code is < 0 on error.
// and "result_message" a description of the error or success
void ExtractZipFileNDK::extractFile(const std::string& callbackId, const std::string& inputString) {
	#define extractReturn(x,y) \
		do {retval["result"] = x; \
		retval["result_message"] = y; \
		m_pParent->NotifyEvent(callbackId + " " + writer.write(retval)); \
		return;} while (0)

	// Tune this and reduce mem usage
	#define EZIPBUFSIZE 1024
	#define MAX_FILENAME 1024


	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	Json::Value retval;
	s2jInit(retval);


	bool parse = reader.parse(inputString, root);
	if (!parse) {
		extractReturn(-1, "Cannot parse internal JSON object");
	}

	// -- Parse Input
	// callbackToken
	std::string requestedToken = root["callbackToken"].asString();
	if (requestedToken == "")
		requestedToken = root["zip"].asString();
	retval["callbackToken"] = requestedToken;

	// destination
	std::string dest_root = root["destination"].asString();
	if (dest_root == "")
		dest_root = "./";
	if (dest_root[dest_root.size() - 1] != '/')
		dest_root += "/";

	// zip
	std::string src_zip = root["zip"].asString();
	if (src_zip == "")
		extractReturn(-1, "zip argument must not be empty");

	// tarBombProtection
	// ensures everything is extracted into a single folder
	bool prevent_tar_bomb = root["tarBombProtection"].asString() == "true";
	if (prevent_tar_bomb) {
		unsigned filename_start = 1 + src_zip.find_last_of("/");
		unsigned filename_end = src_zip.find_last_of(".");
		std::string filename = src_zip.substr(filename_start, filename_end);
		dest_root += filename + "/";
	}

	// overwriteFiles
	bool overwrite_files = !(root["overwriteFiles"].asString() == "false");

	// -- Extract Zip
	const char *zip_path = src_zip.c_str();
	unzFile zipFile = unzOpen(zip_path);
	if (zipFile == NULL)
		extractReturn(-1, "Failed to open zip file.");

	// get zip metadata
	unz_global_info zipInfo;
	if (unzGetGlobalInfo(zipFile, &zipInfo) != UNZ_OK) {
		unzClose(zipFile);
		extractReturn(-1, "Failed to parse zip metadata.");
	}

	// fixed size buf on stack, WATCH USAGE!
	char fileBuffer[EZIPBUFSIZE];

	// Ensure destination exists
	ExtractZipFileNDK_mkpath(dest_root.c_str());

	int filesExtracted = 0;
	int files_skipped = 0;
	for (int i = 0; i < zipInfo.number_entry; i++) {
		s2jIncre("entries");

		// single file metadata
		unz_file_info fileInfo;
		char filename[MAX_FILENAME + 1];

		// get metadata on specific file
		if (unzGetCurrentFileInfo(
					zipFile,
					&fileInfo,
					filename,
					MAX_FILENAME,
					NULL, 0, NULL, 0) != UNZ_OK) {
			unzClose(zipFile);
			extractReturn(-1, "Failed to parse a file's metadata.");
		}
		filename[MAX_FILENAME] = '\0'; // ensure string termination

		// Handle Directories
		if (filename[strlen(filename) - 1] == '/') {
			// Directory creation cannot lose data
			// so we do not care if a dir already
			// exists
			ExtractZipFileNDK_mkpath((dest_root + filename).c_str());
			s2jIncre("directories");
			// Note: The zip format does store permissions
			// except these are all in platform specific
			// formats. I talked with the guy responsible
			// for window's file navigator's zip lib.
			// His stories have scared me into the thinking
			// that providing a default permission is more
			// user friendly than missing obscure edge cases
			// which could leave files in "magical" states.

		// Handle Files
		} else {

			// Note: This opens zipFile's "current" file
			//       "current" acts as an interator
			if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
				unzClose(zipFile);
				extractReturn(-1, "Failed to extract file");
			}

			// Open destination file in file system
			std::string dest_file_path = dest_root + filename;

			// Check for overwriting
			if (!overwrite_files && access(dest_file_path.c_str(), F_OK) != -1) {
				// file exists and we are not allowed to overwrite
				s2jIncre("files_skipped");

			} else {
				FILE *destFile = fopen(dest_file_path.c_str(), "wb");
				if (destFile == NULL) {
					unzCloseCurrentFile(zipFile);
					unzClose(zipFile);
					extractReturn(-1, "Failed to open destination file = " + dest_file_path);
				}

				// Ferry data into destination
				int readResult = UNZ_OK; // is 0
				do {
					// Read
					readResult = unzReadCurrentFile(zipFile, fileBuffer, EZIPBUFSIZE);
					if (readResult < 0) {
						unzCloseCurrentFile(zipFile);
						unzClose(zipFile);
						fclose(destFile);
						extractReturn(-1, "Failed to read compressed file");
					}

					// Write
					if (readResult > 0) {
						int writeResult = fwrite(fileBuffer, readResult, 1, destFile);
						if (writeResult != 1) {
							// Note: we asked for the full buffer
							// to be written at once, so the 1
							// return value is not "true" but
							// the number blocks writen
							unzCloseCurrentFile(zipFile);
							unzClose(zipFile);
							fclose(destFile);
							extractReturn(-1, "Failed to write to destination file");
						}
					}
				} while (readResult > 0);

				fclose(destFile);
				filesExtracted++;
			}
			s2jIncre("files");
		}

		unzCloseCurrentFile(zipFile);

		// Increment "current" file interator if any more files
		unzGoToNextFile(zipFile);
	}

	unzClose(zipFile);

	// Success!
	extractReturn(filesExtracted, "Extraction successful.");
}

std::vector<std::string> f_path;
std::vector<std::string> f_name;
string file_name = "";
bool getRootFileName = false;

void ExtractZipFileNDK::compressFile(const std::string& callbackId, const std::string& inputString) {
	#define compressReturn(x,y) \
		do {retval["result"] = x; \
		retval["result_message"] = y; \
		m_pParent->NotifyEvent(callbackId + " " + writer.write(retval)); \
		return;} while (0)

	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	Json::Value retval;
	s2jInit(retval);

	// store files that are requested to be compressed but do not exist
	std::vector<std::string> f_miss;

	bool parse = reader.parse(inputString, root);
	if (!parse) {
		compressReturn(-1, "Compression Failed: Cannot parse internal JSON object");
	}

	// -- Parse Input
	// callbackToken
	std::string requestedToken = root["callbackToken"].asString();
	if (requestedToken == "")
			requestedToken = root["zipDestinationPath"].asString();
	retval["callbackToken"] = requestedToken;

	// file to compress
	std::string filePath = root["filePath"].asString();

	/*added*/
	m_pParent->getLog()->info(filePath.c_str());
	std::stringstream ss(filePath);
	int i, f_cout = 100;
	std::string token;
	int c = 0;
	int totalFileCount = 0;
	/*added*/

	if (filePath == "")
		compressReturn(-1, "Compression Failed: filePath argument must not be empty");

	std::string fileName = getFileNameFromPath(filePath);


	/*added*/

	    while(std::getline(ss, token, ':')) {//store them into an array

	        string filePath = token;
	        string fileName = getFileNameFromPath(filePath);

	        struct stat st;

	        if(stat(filePath.c_str(),&st) == 0 ){
                if( st.st_mode & S_IFDIR ){//if it is a directory

                    getDirectoryContent(filePath, fileName);

                }else{
                    f_name.push_back(fileName);
                    f_path.push_back(filePath);
                }

    	        m_pParent->getLog()->info(f_path[c].c_str());
		        m_pParent->getLog()->info(f_name[c].c_str());
		        ++c;

	        } else if (errno == ENOENT) {
                // file <filePath> does not exist
                f_miss.push_back(filePath);
	        }
	    }

    if (f_path.empty()) { // no files to compress
		// we have checked that filePath is nonempty, so here we must have that all given
		// files/directories are not existed; no zip will be created so compression failed
		compressReturn( -1, "Compression Failed: all given files and directories are not found; nothing to do! ");
    }
	/*added*/

	// zip destination
	std::string zipDestinationPath = root["zipDestinationPath"].asString();
	if (zipDestinationPath == "")
		compressReturn(-1, "Compression Failed: zipDestinationPath argument must not be empty");

	// Ensure destination exists
	std::string zipDestinationDirectory = getDirectoryFromPath(zipDestinationPath);
	ExtractZipFileNDK_mkpath(zipDestinationDirectory.c_str());

	// create zip file
	zipFile zipFileCreate = zipOpen(zipDestinationPath.c_str(), APPEND_STATUS_CREATE);
	if(zipFileCreate == NULL) {
		compressReturn(-1, "Compression Failed: Failed to create zip file");
	} else {

	    for( unsigned a = 0; a < f_name.size(); a++){

            zip_fileinfo zi;
            zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
            zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
            zi.dosDate = 0;
            zi.internal_fa = 0;
            zi.external_fa = 0;
            int errorCode = zipOpenNewFileInZip(zipFileCreate,
                                                    f_name[a].c_str(),
                                                    &zi,
                                                    NULL, 0,
                                                    NULL, 0,
                                                    NULL,
                                                    Z_DEFLATED,
                                                    Z_DEFAULT_COMPRESSION
                    );

            if(errorCode != ZIP_OK) {
                zipClose(zipFileCreate, "");
                compressReturn(-1, "Compression Failed: Failed to make file in zip");
            }

            // open file we are going to zip
            FILE* fileToZip = fopen(f_path[a].c_str(), "r");
            if(fileToZip == NULL) {
                zipCloseFileInZip(zipFileCreate);
                zipClose(zipFileCreate, "");
                compressReturn(-1,"Compression Failed: Failed to open file to zip");
            }

            if(fileToZip != NULL) {
                long fileSize = getFileSize(fileToZip);
                if(fileSize > 0) {
                    unsigned int charactorSizeInBytes = 1;
                    unsigned int bufferLength = (fileSize <= 4096) ? fileSize : 4096;
                    void* buffer = malloc(charactorSizeInBytes * bufferLength);

                    bool finishedFileCopy = false;
                    do
                    {
                        int numCharactorsRead = fread(buffer, charactorSizeInBytes, bufferLength, fileToZip);

                        if(ferror(fileToZip)){
                            break;
                        }

                        if(feof(fileToZip)) {
                            finishedFileCopy = true;
                        }
                        zipWriteInFileInZip(zipFileCreate, buffer, numCharactorsRead);
                    }while(!finishedFileCopy);

                    fclose(fileToZip);
                    free(buffer);
                }
            }
	    }
		zipCloseFileInZip(zipFileCreate);
		/*added*/
		while (!f_name.empty()){
		    f_name.pop_back();
		    f_path.pop_back();
		}
		getRootFileName = false;
		file_name = "";
		/*added*/
	}
	zipClose(zipFileCreate, "");

	// warning missed files in result_message
	std::string retMessage = "Compression Successful";
	if ( !f_miss.empty() ) {
		retMessage.append("\\nwarning: the following files or directories are not found:");
		std::vector<std::string>::iterator it;
		for ( it = f_miss.begin() ; it != f_miss.end(); ++it) {
			retMessage.append("\\n");
			retMessage.append(*it);
		}
	}

	compressReturn(1, retMessage );
}

/*added*/
void ExtractZipFileNDK::getDirectoryContent(std::string directoryPath, std::string rootDirectory) {
    DIR *dir;
    struct dirent *ent;
    string f_content;
    if(!getRootFileName){
        file_name = getFileNameFromPath(directoryPath);
    }
    getRootFileName = true;

    if ((dir = opendir (directoryPath.c_str())) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
          f_content = ent->d_name;

              string newPath = directoryPath+"/"+f_content;
              string newContent = "/" + f_content;

              struct stat s;
              if(stat(newPath.c_str(),&s) == 0 ){
                  if( s.st_mode & S_IFDIR ){//directory

                      if(f_content!="." && f_content!=".."){
                          m_pParent->getLog()->warn(f_content.c_str());
                          m_pParent->getLog()->warn(newPath.c_str());

                          getDirectoryContent(newPath, rootDirectory);
                      }
                  }else{
                      string f_final = newPath.substr(newPath.find(file_name),newPath.length() -1);
                      m_pParent->getLog()->error(f_final.c_str());
                      m_pParent->getLog()->error(newPath.c_str());
                      f_name.push_back(f_final);
                      f_path.push_back(newPath);
                  }

              }

      }
      closedir (dir);
    } else {
        m_pParent->getLog()->info("getDirectoryContent:could not open directory");
    }
}
/*added*/

std::string ExtractZipFileNDK::getFileNameFromPath(std::string filePath) {
	if(filePath.length() == 0)
		return "";

	int indexOfLastForwardSlash = 0;
	for(int i = filePath.length() - 1; i >= 0; i--) {
		if(filePath[i] == '/') {
			indexOfLastForwardSlash = i;
			break;
		}
	}

	return filePath.substr(indexOfLastForwardSlash + 1, string::npos);
}

std::string ExtractZipFileNDK::getDirectoryFromPath(std::string path) {
	if(path.length() == 0)
		return "";

	int indexOfLastForwardSlash = 0;
	for(int i = path.length() - 1; i >= 0; i--) {
		if(path[i] == '/') {
			indexOfLastForwardSlash = i;
			break;
		}
	}

	return path.substr(0, indexOfLastForwardSlash + 1);
}

long ExtractZipFileNDK::getFileSize(FILE* file) {
	  fseek (file , 0, SEEK_END);
	  long sizeOfFile = ftell (file);
	  rewind (file);
	  return sizeOfFile;
}

} /* namespace webworks */
