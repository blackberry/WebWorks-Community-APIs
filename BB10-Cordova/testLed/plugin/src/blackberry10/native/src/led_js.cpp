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
#include "led_js.hpp"
#include <errno.h>
using namespace std;

FlashLed::FlashLed(const std::string& id) :
		m_id(id) {

}

FlashLed::~FlashLed() {
}

char* onGetObjList() {
	static char name[] = "FlashLed";
	return name;
}

JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "FlashLed") {
		return new FlashLed(id);
	}

	return NULL;
}


bool FlashLed::CanDelete() {
	return true;
}

/**
 * Usage from javascript:
 * var id = flashLED('color', 'blink_count')
 * stopLed(id);
 */
string FlashLed::InvokeMethod(const string& command) {

	bps_initialize();

	// Determine which function should be executed
	size_t index = command.find_first_of(" ");
	string strCommand = command.substr(0, index);

	if (strCommand == "flashLedStartNative") {
		int colorCode = LED_COLOR_RED;
		int blinkCount = 0;
		if (command.length() > index) {
			string params = command.substr(index + 1, command.length());

			size_t indexColor = params.find_first_of(" ");
			string color = params.substr(0, indexColor);

			// attempt to convert hex string passed in from JavaScript to integer
			char * p;
			long n = strtoul(color.c_str(), &p, 16);
			if (*p != 0) {
				fprintf(stderr, "Invalid color");

			} else {
				colorCode = (int) n;
			}

			if (params.length() > indexColor) {
				string blink = params.substr(indexColor + 1, params.length());
				blinkCount = atoi(blink.c_str());
			}

		}
		return this->startLed(colorCode, blinkCount);
	} else if (strCommand == "flashLedStopNative") {
		string id = command.substr(index + 1, command.length());
		return this->stopLed(id.c_str());
	}
	return "Unsupported Method";

}

string FlashLed::startLed(int color, int blinkCount) {

	//creates a reference id for user
	string id = convertLongToString(clock());
	int status = led_request_rgb(id.c_str(), color, blinkCount);

	return id;
}
string FlashLed::stopLed(string id) {
	int status = led_cancel(id.c_str());
	if (status == BPS_SUCCESS) {
		return "BPS_SUCCESS";
	} else {
		return strerror(errno);
	}

}
string FlashLed::convertLongToString(long l) {
	stringstream ss;
	ss << l;
	return ss.str();
}

