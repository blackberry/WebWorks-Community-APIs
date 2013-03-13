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
#include <cctype>
#include <algorithm>
#include <json/reader.h>
#include <json/writer.h>
#include <huctx.h>
#include <sbreturn.h>
#include <husha1.h>
#include <husha2.h>
#include <hugse56.h>

#include "gsecrypto.hpp"
#include "gsecryptojs.hpp"
#include "aes.hpp"
#include "util/util.hpp"

#include "aes.hpp"
#include "sha.hpp"

class GSECryptoJS;

namespace webworks {

GSECrypto::GSECrypto(GSECryptoJS *owner) {
	parent = owner;

	sbCtx = NULL;

	try {
		int error = hu_GlobalCtxCreateDefault(&sbCtx);
		if (error != SB_SUCCESS) {
			throw "Failed to create global context";
		}

		error = hu_RegisterSbg56(sbCtx);
		if (error != SB_SUCCESS) {
			throw "Failed to register sbg 5.6";
		}

		error = hu_InitSbg56(sbCtx);
		if (error != SB_SUCCESS) {
			throw "Failed to init sbg 5.6";
		}

		providers.push_back(new SHA(this));
		providers.push_back(new AES(this));

	} catch (const char * message) {
		lastError = message;
	}
}

GSECrypto::~GSECrypto() {
	if (sbCtx != NULL) {
		hu_GlobalCtxDestroy(&sbCtx);
		sbCtx = NULL;
	}
}

// Take in input and return a value
std::string GSECrypto::hash(const std::string& inputString) {
	try {
		Json::Value args;

		readJson(inputString,args);
		std::string alg(getAlgorithm(args,"sha1"));
		Provider * p = findProvider(alg);

		return toString(p->hash(alg,args));

	} catch (std::string & error) {
		return fail(error);
	}
}

void GSECrypto::readJson(const std::string & inputStream, Json::Value & value) {
	Json::Reader reader;
	if (reader.parse(inputStream,value)) {
		// great!
	} else {
		throw std::string("Could not read JSON input");
	}
}

Provider * GSECrypto::findProvider(const std::string & algorithm) {
	for (std::list<Provider*>::iterator i = providers.begin(); i!=providers.end(); ++i) {
		if ((*i)->doesSupport(algorithm)) {
			return (*i);
		}
	}
	std::string message = "Could not find support for ";
	message += algorithm;
	throw message;
}

std::string GSECrypto::getAlgorithm(Json::Value & value, const std::string & defaultAlgorithm) {
	if (value.isMember("alg")) {
		std::string toReturn = value.get("alg",defaultAlgorithm).asString();
		std::transform(toReturn.begin(), toReturn.end(), toReturn.begin(), tolower);
		toReturn.erase(std::remove(toReturn.begin(), toReturn.end(), '-'), toReturn.end());

		return toReturn;
	} else if (defaultAlgorithm.size()!=0) {
		return defaultAlgorithm;
	}
	throw std::string("Could not determine algorithm");
}

sb_GlobalCtx GSECrypto::context() {
	return sbCtx;
}

sb_RngCtx GSECrypto::randomContext() {
	return NULL;
}

std::string GSECrypto::toString(const Json::Value & value) {
	Json::FastWriter writer;
	return writer.write(value);
}

std::string GSECrypto::fail(const std::string & error) {
	Json::Value value;
	value["error"] = error;
	return toString(value);
}

} /* namespace webworks */
