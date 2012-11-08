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
	if (command == "unzipPackageNative")
	{
		// Retrieve first parameter to method
		size_t index = command.find_first_of(" ");
		string pathToZipStr = command.substr(index + 1, command.length());

		return unzipPackageNative(pathToZipStr.c_str());
	}
    else
    {
        return "Unsupported Method";
    }
}

/**
 * Returns a JSON object in the form of a C++ string to be evaluated on the JavaScript side
 */
string Unzip::unzipPackageNative(const char *zipPath)
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

	// Iterate over all files in the Zip package
	for (ZPOS64_T fileIndex = 0; fileIndex < packageGlobalInfo.number_entry; ++fileIndex)
	{
		if ( UNZ_OK != unzGetCurrentFileInfo64(zipPackage, &fileInfo, fileName, sizeof(fileName), NULL, 0, NULL, 0) )
		{
			// Log error here - Getting the file info of a file inside the Zip
			fprintf(stderr, "Error retrieving the file information of a file inside the Zip");
		}

		// Read file name
		std::string fileNameStr = fileName;



	}

	// Close the root package
	if ( UNZ_OK != unzClose(zipPackage) )
	{
		// Log error here - Closing the zip package
		fprintf(stderr, "Error Closing Zip package");
	}

	return NULL;
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
