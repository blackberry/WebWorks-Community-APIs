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
#include <string>
#include <json/reader.h>
#include <json/writer.h>
#include <slog2.h>

// minizip
#include "unzip.h"

#include "extractzipfile_ndk.hpp"
#include "extractzipfile_js.hpp"

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
void ExtractZIPFileNDK::extractFile(const std::string& callbackId, const std::string& inputString) {
	#define extractReturn(x,y) \
		while (0) {retval["result"] = x; \
		retval["result_message"] = y; \
		m_pParent->NotifyEvent(callbackId + " " + writer.write(retval));}

	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	Json::Value retval;
	bool parse = reader.parse(inputString, root);
	if (!parse) {
		extractReturn(-1, "Cannot parse internal JSON object");
		m_pParent->NotifyEvent(callbackId + " " + writer.write(retval));
		return;
	}

	// Perform the zip unpacking 
	unxFile zipFile = unzOpen(root["zip"].asString().c_str());
	if (zipFile == NULL)
		extractReturn(-1, "Failed to open zip file.");

	unzClose(zipFile);

	// Success!
	extractReturn(0, "Extract successful.");
	#undef extractReturn
}

} /* namespace webworks */
