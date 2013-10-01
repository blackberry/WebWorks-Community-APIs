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

// Async extract file from zip
void ExtractZipFileNDK::extractFile(const std::string& callbackId, const std::string& inputString) {
	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	Json::Value retval;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		retval["result"] = "Cannot parse internal JSON object";
		m_pParent->NotifyEvent(callbackId + " " + writer.write(retval));
		return;
	}

	std::string command =
			"unzip " +
			root["zip"].asString()
			+ " " +
			root["file"].asString();

	int unzip_result = system(
		command.c_str()
	);

	retval["result"] = errno;//unzip_result;
	m_pParent->NotifyEvent(callbackId + " " + writer.write(retval));

}

} /* namespace webworks */
