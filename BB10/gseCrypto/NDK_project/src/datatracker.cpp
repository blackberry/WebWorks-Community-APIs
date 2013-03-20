/*
 * datatracker.cpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#include "datatracker.hpp"

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

} // namespace gsecrypto
