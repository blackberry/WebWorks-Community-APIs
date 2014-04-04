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
#include <screen/screen.h>
#include "preventSleep_ndk.hpp"
#include "preventSleep_js.hpp"

namespace webworks {

PreventSleepNDK::PreventSleepNDK(PreventSleepJS *parent) {
	m_pParent = parent;
	preventSleepIsOn = false;
}

PreventSleepNDK::~PreventSleepNDK() {
}

// Take in JSON of value (bool) and handle (string) and return a message when complete
std::string PreventSleepNDK::setPreventSleep(const std::string& inputString) {
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		return "Cannot parse JSON object for Prevent Sleep";
	}
	// the jsScreenWindowHandle of the UIWebView that we passed in
	int handle = (int) strtol(root["handle"].asCString(), NULL, 10);
	// As an integer is the actual window handle
	screen_window_t window = (screen_window_t) handle;
	// Set the mode to normal by default
	int mode = SCREEN_IDLE_MODE_NORMAL;
	if (root["value"].asBool()) {
		mode = SCREEN_IDLE_MODE_KEEP_AWAKE;
	}
	int rc = screen_set_window_property_iv(window, SCREEN_PROPERTY_IDLE_MODE, &mode );
	// check that we were successful, and we need to flush the command anyway
	rc = screen_get_window_property_iv(window, SCREEN_PROPERTY_IDLE_MODE, &mode );
	if (mode == SCREEN_IDLE_MODE_KEEP_AWAKE) {
		preventSleepIsOn = true;
		return "Screen now keeping awake";
	} else {
		preventSleepIsOn = false;
		return "Screen now allowed to sleep";
	}
}

// Get the prevent sleep state
std::string PreventSleepNDK::isPreventSleep() {
	stringstream ss;
	ss << preventSleepIsOn;
	return ss.str();
}

} /* namespace webworks */
