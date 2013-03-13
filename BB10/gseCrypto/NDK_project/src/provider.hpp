/*
 * provider.hpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#ifndef PROVIDER_HPP_
#define PROVIDER_HPP_

#include <string>

#include <json/value.h>

#include <huctx.h>
#include <hurandom.h>

#include "datatracker.hpp"

namespace webworks {

class Provider {
public:
	Provider(class GSECrypto * owner);
	virtual ~Provider();

	virtual bool doesSupport(const std::string & algorithm) = 0;

	virtual Json::Value generateKey(const std::string & algorithm, Json::Value & input);

	virtual Json::Value hash(const std::string & algorithm, Json::Value & input);

	virtual Json::Value encrypt(const std::string & algorithm, Json::Value & input);
	virtual Json::Value decrypt(const std::string & algorithm, Json::Value & input);

	virtual Json::Value sign(const std::string & algorithm, Json::Value & input);
	virtual Json::Value verify(const std::string & algorithm, Json::Value & input);

protected:
	void getData(Json::Value & value, DataTracker & data);
	Json::Value toJson(unsigned char * data, size_t dataLen);

	GSECrypto * owner;

	sb_GlobalCtx context();
	sb_RngCtx randomContext();
};

} /* namespace webworks */
#endif /* PROVIDER_HPP_ */
