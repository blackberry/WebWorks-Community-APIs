/*
 * Copyright 2013-2014 Research In Motion Limited.
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
#include "dataTracker.hpp"

namespace gsecrypto {

DataTracker::DataTracker() :
		data(0), dataLen(0) {
}

DataTracker::DataTracker(size_t size) {
	if (size == 0) {
		data = NULL;
		dataLen = 0;
	} else {
		data = new unsigned char[size];
		dataLen = size;
	}
}

DataTracker::~DataTracker() {
	cleanUp();
}

void DataTracker::cleanUp() {
	if (data != NULL) {
		delete[] data;
		data = NULL;
		dataLen = 0;
	}
}

void DataTracker::setData(const std::string & in) {
	cleanUp();
	dataLen = in.length();
	if (dataLen == 0) {
		data = NULL;
	} else {
		data = new unsigned char[dataLen];
		const char * inData = in.data();
		for (size_t i = 0; i < dataLen; ++i) {
			data[i] = inData[i];
		}
	}
}

} /* namespace gsecrypto */
