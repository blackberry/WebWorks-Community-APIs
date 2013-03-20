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

namespace gsecrypto {

class DataTracker {
public:
	DataTracker();
	DataTracker(size_t size);
	virtual ~DataTracker();

	void cleanUp();
	void setData(const std::string & in);

	unsigned char * data;
	size_t dataLen;
};


} /* namespace gsecrypto */
#endif /* DATATRACKER_HPP_ */
