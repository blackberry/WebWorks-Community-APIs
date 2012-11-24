/*
* Copyright 2012 Research In Motion Limited.
*
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

#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

#include "unzip.h"
#include "unzip_js.hpp"

using namespace std;

/**
 * Default constructor.
 */
Unzip::Unzip(const std::string& id) : m_id(id)
{
}

/**
 * Unzip destructor.
 */
Unzip::~Unzip()
{
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList()
{
    static char name[] = "Unzip";
    return name;
}

/**
 * This method is used by JNext to instantiate the Unzip object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id)
{
    if (className == "Unzip")
    {
        return new Unzip(id);
    }

    return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool Unzip::CanDelete()
{
    return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string Unzip::InvokeMethod(const string& command)
{
    // Determine which function should be executed
	size_t index = command.find_first_of(" ");
	string strCommand = command.substr(0, index);

	if (strCommand == "unzipPackageNative")
	{
		// Retrieve first parameter to method
		string parameters = command.substr(index + 1, command.length());
		size_t zipPathIndex = parameters.find_first_of(" ");
		string pathToZipStr = parameters.substr(0, zipPathIndex);
		string pathToUnzipToStr = parameters.substr(zipPathIndex + 1, parameters.length());

		return unzipPackageNative(pathToZipStr.c_str(), pathToUnzipToStr.c_str());
	}
    else
    {
        return "Unsupported Method";
    }
}

/*
 * Extracts current file in zipPackage to the working path + filename
 * returns false if error occurs
 */
bool Unzip::extractFile(unzFile zipPackage, string filepath)
{
	if ( UNZ_OK != unzOpenCurrentFilePassword(zipPackage, NULL) )
	{
		// Log error here - Failed opening current file
		fprintf(stderr, "Failed opening current file.");
		return false;
	}

	// Allocate read buffer
	unsigned int bufferSize = 8192;
	void* buffer = (void*)malloc(bufferSize);

	if ( buffer == NULL )
	{
		// Log error here - Failed in allocating memory
		fprintf(stderr, "Failed in allocating memory.");
		return false;
	}

	// Open our file
	FILE* filePointer = NULL;
	filePointer = fopen64(filepath.c_str(), "wb");

	if ( filePointer == NULL )
	{
		// Log error here - Failed in opening file
		fprintf(stderr, "Failed opening file.");
		return false;
	}

	int readError = 0;
	do
	{
		readError = unzReadCurrentFile(zipPackage, buffer, bufferSize);
		if ( readError < 0 )
		{
			// Log error here - Failed reading current file into buffer
			fprintf(stderr, "Error reading current file into buffer.");
			break;
		}

		if ( readError > 0 )
		{
			if ( fwrite(buffer, readError, 1, filePointer) != 1 )
			{
				// Log error here - Failed writing extracted file
				fprintf(stderr, "Error writing extracted file.");
				break;
			}
		}
	}
	while ( readError > 0 );

	// Close our file
	if ( filePointer )
	{
		fclose(filePointer);
	}

	if ( UNZ_OK != unzCloseCurrentFile(zipPackage) )
	{
		// Log error here - Failed closing current file
		fprintf(stderr, "Failed closing current file.");
		return false;
	}

	free(buffer);
	buffer = NULL;

	return true;
}

/**
 * Returns a JSON object in the form of a C++ string to be evaluated on the JavaScript side
 */
string Unzip::unzipPackageNative(const char *zipPath, const char* unzipToPath)
{
	unzFile zipPackage = unzOpen64(zipPath);

	if (zipPackage == NULL)
	{
		// Log error here - Opening the zip Package
		fprintf(stderr, "Error Opening Zip package");
	}

	unz_global_info64 packageGlobalInfo;
	if ( UNZ_OK != unzGetGlobalInfo64(zipPackage, &packageGlobalInfo) )
	{
		// Log error here - Getting the global info of the package
		fprintf(stderr, "Error retrieving the global info of the Zip package");
	}

	unz_file_info64 fileInfo;
	char fileName[256];
	string JSONobject = "";

	// Start the JSONobject String
	JSONobject += "({ \"";
	JSONobject += zipPath;
	JSONobject += "\" : [";

	unsigned int prevNestedLevel = 1;
	bool prevFileInDirectory = false;

	// Iterate over all files in the Zip package
	for (ZPOS64_T fileIndex = 0; fileIndex < packageGlobalInfo.number_entry; ++fileIndex)
	{
		if ( UNZ_OK != unzGetCurrentFileInfo64(zipPackage, &fileInfo, fileName, sizeof(fileName), NULL, 0, NULL, 0) )
		{
			// Log error here - Getting the file info of a file inside the Zip
			fprintf(stderr, "Error retrieving the file information of a file inside the Zip");
		}

		// Full absolute path
		string fullPath = unzipToPath;
		fullPath += fileName;

		// Get filename without path and if the file is a directory
		char* currentCharacter = fileName;
		char* fileNameWithoutPath = fileName;
		unsigned int currentLevel = 1; // keep track of how deep in the directory tree we're in

		while ( (*currentCharacter) != '\0' )
		{
			if ( (*currentCharacter) == '/' || (*currentCharacter) == '\\' )
			{
				fileNameWithoutPath = currentCharacter + 1;
				++currentLevel;
			}

			++currentCharacter;
		}

		if ( currentLevel > prevNestedLevel )
		{
			if ( prevFileInDirectory )
			{
				JSONobject += ",";
			}

			// we went down a directory

			// Create the directory
			if ( mkdir(fullPath.c_str(), 0775) == -1 )
			{
				// Log error here - Failed in making a new directory
				fprintf(stderr, "Error creating directory.");
			}

			// Start constructing a JSON directory tree
			JSONobject += "{ \"";
			JSONobject += fileName;
			JSONobject += "\" : [";

			// if we're at the last element, close this empty directory
			if ( fileIndex == packageGlobalInfo.number_entry - 1 )
			{
				JSONobject += "]}";
			}

			prevFileInDirectory = false;
		}
		else if ( currentLevel < prevNestedLevel )
		{
			// We went up a directory, so we're at a file

			// Extract our file
			if ( !extractFile(zipPackage, fullPath) )
			{
				return "( { error } )";
			}

			//TODO: did we go up multiple directories? if so, we should while loop the closing of
			// previous directory trees

			// Close previous directory tree
			JSONobject += "]},";

			// Append file
			JSONobject += "{ \"filename\" : \"";
			JSONobject += fileNameWithoutPath;
			JSONobject += "\" }";

			// if we're at the last element, close this file
			if ( fileIndex == packageGlobalInfo.number_entry - 1 )
			{
				// TODO: while loop this ending tag depending on how deep current level is
				JSONobject += "]}";
			}

			prevFileInDirectory = true;
		}
		else
		{
			// levels are equal, either we switched from a directory that was empty
			// to another directory, or we're at a file
			if ( *fileNameWithoutPath == '\0' )
			{
				// Create the directory
				if ( mkdir(fullPath.c_str(), 0775) == -1 )
				{
					// Log error here - Failed in making a new directory
					fprintf(stderr, "Error creating directory.");
				}

				// Close previous directory tree
				JSONobject += "]},";

				// Start constructing a JSON directory tree
				JSONobject += "{ \"";
				JSONobject += fileName;
				JSONobject += "\" : [";

				// if we're at the last element, close this empty directory
				if ( fileIndex == packageGlobalInfo.number_entry - 1 )
				{
					JSONobject += "]}";
				}

				prevFileInDirectory = false;
			}
			else
			{
				// Extract our file
				if ( !extractFile(zipPackage, fullPath) )
				{
					return "( { error } )";
				}

				if ( prevFileInDirectory )
				{
					JSONobject += ",";
				}

				// We are at a file
				JSONobject += "{ \"filename\" : \"";
				JSONobject += fileNameWithoutPath;
				JSONobject += "\" }";

				prevFileInDirectory = true;
			}
		}

		// Set previous nested level
		prevNestedLevel = currentLevel;

		if ( UNZ_OK != unzGoToNextFile(zipPackage) )
		{
			// Log error here - Going to next file inside zip
			fprintf(stderr, "Error iterating to next file inside Zip");
		}
	}

	// Close root tag
	JSONobject += "]})";

	// Close the root package
	if ( UNZ_OK != unzClose(zipPackage) )
	{
		// Log error here - Closing the zip package
		fprintf(stderr, "Error Closing Zip package");
	}

	return JSONobject;
}

// Notifies JavaScript of an event
void Unzip::NotifyEvent(const std::string& event)
{
    std::string eventString = m_id + " ";
    eventString.append(event);
    SendPluginEvent(eventString.c_str(), m_pContext);
}

/**
 * Utility function to convert a long into a string.
 */
string Unzip::convertLongToString(long l)
{
    stringstream ss;
    ss << l;
    return ss.str();
}
