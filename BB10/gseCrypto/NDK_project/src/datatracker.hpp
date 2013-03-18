/*
 * datatracker.hpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#ifndef DATATRACKER_HPP_
#define DATATRACKER_HPP_

#include <string>
#include <string.h>

namespace webworks {

class DataTracker {
public:
	DataTracker() :
			data(0), dataLen(0) {
	}
	;
	DataTracker(size_t size) {
		data = new unsigned char[size];
		dataLen = size;
	}
	virtual ~DataTracker() {
		cleanUp();
 	}
	void cleanUp() {
		if (data != NULL) {
			delete[] data;
			data = NULL;
			dataLen = 0;
		}
	}
	void setData(std::string in) {
		cleanUp();
		dataLen = in.length();
		data = new unsigned char[dataLen + 1];
		strcpy((char*) data, in.data());
	}
	unsigned char * data;
	size_t dataLen;
};


} /* namespace webworks */
#endif /* DATATRACKER_HPP_ */
