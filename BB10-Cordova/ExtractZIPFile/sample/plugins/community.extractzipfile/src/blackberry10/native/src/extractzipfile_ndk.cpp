/*
 * Copyright (c) 2013 BlackBerry Limited
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
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "extractzipfile_ndk.hpp"
#include "extractzipfile_js.hpp"

namespace webworks {

ExtractZIPFileNDK::ExtractZIPFileNDK(ExtractZIPFileJS *parent) {
	m_pParent = parent;
	templateProperty = 50;
	templateThreadCount = 1;
	m_thread = 0;
	pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	threadHalt = true;
}

ExtractZIPFileNDK::~ExtractZIPFileNDK() {
}

// These methods are the true native code we intend to reach from WebWorks

// Async extract file from zip
void ExtractZIPFileNDK::extractFile(const std::string& callbackId, const std::string& inputString) {
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

	string command =
			"unzip " +
			root["zip"].asString()
			+ " " +
			root["file"].asString();

	int unzip_result_raw = system(
		command.c_str()
	);

	stringstream unzip_result;
	unzip_result << unzip_result_raw;
	retval["result"] = unzip_result.str();
	m_pParent->NotifyEvent(callbackId + " " + writer.write(retval));

}

} /* namespace webworks */
