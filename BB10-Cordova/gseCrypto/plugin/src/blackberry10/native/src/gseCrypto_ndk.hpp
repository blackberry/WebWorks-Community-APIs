/*
 * Copyright 2013-2014 BlackBerry Limited.
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
#ifndef _GSECryptoNDK_
#define _GSECryptoNDK_

#include <string>
#include <list>
#include <pthread.h>
#include <huctx.h>

#include "provider.hpp"
#include <json/value.h>

class GSECryptoJS;

namespace gsecrypto {

/**
 * Manager of a set of providers, all crypto operations go via an instance of GSECryptoNDK.
 */
class GSECryptoNDK {
public:
	explicit GSECryptoNDK(GSECryptoJS *parent = NULL);
	virtual ~GSECryptoNDK();

	std::string hash(const std::string& inputString);

	std::string encrypt(const std::string & inputString);
	std::string decrypt(const std::string & inputString);

	std::string sign(const std::string & inputString) {
		return "";
	}
	std::string verify(const std::string & inputString) {
		return "";
	}

	std::string random(const std::string & inputStream);

	sb_GlobalCtx context();
	sb_RNGCtx randomContext();

private:
	void readJson(const std::string & inputString, Json::Value & result);
	std::string getAlgorithm(Json::Value & args,
			const std::string & defaultAlgorithm = "");
	Provider * findProvider(const std::string & algorithm);

	std::string fail(const std::string & error);

	std::string toString(const Json::Value &);

	GSECryptoJS * parent;

	sb_GlobalCtx sbCtx;
	sb_RNGCtx rngCtx;

	std::string lastError;

	std::list<Provider*> providers;
};

} /* namespace gsecrypto */

#endif /* _GSECryptoNDK_ */
