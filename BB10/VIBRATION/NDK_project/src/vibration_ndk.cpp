/*
 * Copyright 2013 Research In Motion Limited.
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
#include "vibration_ndk.hpp"
#include "vibration_js.hpp"
#include <bps/vibration.h>
namespace webworks {

VibrationNDK::VibrationNDK(VibrationJS *parent) {
	m_pParent = parent;
	bps_initialize();
}

VibrationNDK::~VibrationNDK() {
	bps_shutdown();
}

// Asynchronous callback with JSON data input and output
void VibrationNDK::vibrationRequest(const std::string& inputString) {
	std::string event = "vibration_requestCallbackResult";

	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		Json::Value error;
		error["result"] = "Cannot parse JSON object";
		m_pParent->NotifyEvent(event + " " + writer.write(error));
	} else {
		root["id"] =  vibration_request(
				                     (int) root["intensity"].asInt()
				                    ,(int) root["duration"].asInt()
		);
		m_pParent->NotifyEvent(event + " " + writer.write(root));
	}



}
}

