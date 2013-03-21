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
#ifndef PROVIDER_HPP_
#define PROVIDER_HPP_

#include <string>

#include <json/value.h>

#include <huctx.h>
#include <hurandom.h>

#include "datatracker.hpp"

namespace gsecrypto {

/**
 * Algorithms are implemente as providers. They it's not expected that a Provider implement each of the virtual functions...
 */
class Provider {
public:
	Provider(class GSECrypto & owner);
	virtual ~Provider();

	virtual bool doesSupport(const std::string & algorithm) = 0;

	virtual Json::Value generateKey(const std::string & algorithm,
			Json::Value & input);

	virtual Json::Value hash(const std::string & algorithm,
			Json::Value & input);

	virtual Json::Value encrypt(const std::string & algorithm,
			Json::Value & input);
	virtual Json::Value decrypt(const std::string & algorithm,
			Json::Value & input);

	virtual Json::Value sign(const std::string & algorithm,
			Json::Value & input);
	virtual Json::Value verify(const std::string & algorithm,
			Json::Value & input);

	static Json::Value toJson(unsigned char * data, size_t dataLen);
	static Json::Value toJson(DataTracker & data);
protected:
	void getData(Json::Value & value, DataTracker & data);

	GSECrypto & owner;

	sb_GlobalCtx context();
	sb_RngCtx randomContext();
};

} /* namespace gsecrypto */
#endif /* PROVIDER_HPP_ */
