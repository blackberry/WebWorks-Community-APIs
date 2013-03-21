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
#include <hurandom.h>
#include <huseed.h>

#include "gsecrypto.hpp"
#include "gsecryptojs.hpp"
#include "aes.hpp"
#include "util/util.hpp"

#include "aes.hpp"
#include "sha.hpp"

class GSECryptoJS;

namespace gsecrypto {

GSECrypto::GSECrypto(GSECryptoJS *owner) {
	parent = owner;

	sbCtx = NULL;
	rngCtx = NULL;

	try {
		int error = hu_GlobalCtxCreateDefault(&sbCtx);
		if (error != SB_SUCCESS) {
			throw gsecrypto::util::errorMessage(
					"Failed to create global context", error);
		}

		error = hu_RegisterSbg56(sbCtx);
		if (error != SB_SUCCESS) {
			throw gsecrypto::util::errorMessage("Failed to register sbg 5.6",
					error);
		}

		error = hu_InitSbg56(sbCtx);
		if (error != SB_SUCCESS) {
			throw gsecrypto::util::errorMessage("Failed to init sbg 5.6",
					error);
		}

		size_t seedLength = 100;
		unsigned char seed[100];

		error = hu_RegisterSystemSeed(sbCtx);
		if (error != SB_SUCCESS) {
			throw gsecrypto::util::errorMessage(
					"Failed to register system seed", error);
		}

		error = hu_RngDrbgCreate(HU_DRBG_CIPHER, 256, 0, 0, NULL, NULL, &rngCtx,
				sbCtx);
		if (error != SB_SUCCESS) {
			throw gsecrypto::util::errorMessage("Failed to create DRBG", error);
		}

		providers.push_back(new SHA(*this));
		providers.push_back(new AES(*this));

	} catch (std::string & message) {
		lastError = message;
	}
}

GSECrypto::~GSECrypto() {
	if (sbCtx != NULL) {
		hu_GlobalCtxDestroy(&sbCtx);
		sbCtx = NULL;
	}
}

std::string GSECrypto::hash(const std::string& inputString) {
	try {
		Json::Value args;

		readJson(inputString, args);
		std::string alg(getAlgorithm(args, "sha1"));
		Provider * p = findProvider(alg);

		return toString(p->hash(alg, args));

	} catch (std::string & error) {
		return fail(error);
	}
}

std::string GSECrypto::generateKey(const std::string& input) {
	try {
		Json::Value args;
		readJson(input, args);
		std::string alg(getAlgorithm(args));
		Provider * p = findProvider(alg);

		return toString(p->generateKey(alg, args));
	} catch (std::string & error) {
		return fail(error);
	}
}

std::string GSECrypto::encrypt(const std::string & input) {
	try {
		Json::Value args;
		readJson(input, args);
		std::string alg(getAlgorithm(args));
		Provider * p = findProvider(alg);

		return toString(p->encrypt(alg, args));
	} catch (std::string & error) {
		return fail(error);
	}
}

std::string GSECrypto::decrypt(const std::string & input) {
	try {
		Json::Value args;
		readJson(input, args);
		std::string alg(getAlgorithm(args));
		Provider * p = findProvider(alg);

		return toString(p->decrypt(alg, args));
	} catch (std::string & error) {
		return fail(error);
	}
}

void GSECrypto::readJson(const std::string & inputStream, Json::Value & value) {
	Json::Reader reader;
	if (reader.parse(inputStream, value)) {
		// great!
	} else {
		throw std::string("Could not read JSON input");
	}
}

Provider * GSECrypto::findProvider(const std::string & algorithm) {
	int count(0);
	for (std::list<Provider*>::iterator i = providers.begin();
			i != providers.end(); ++i) {
		++count;
		if ((*i)->doesSupport(algorithm)) {
			return (*i);
		}
	}
	std::stringstream stream;
	stream << "Could not find support for " << algorithm.c_str() << ". Tried "
			<< count << " providers.";
	throw stream.str();
}

std::string GSECrypto::getAlgorithm(Json::Value & value,
		const std::string & defaultAlgorithm) {
	if (value.isMember("alg")) {
		return gsecrypto::util::lowerCaseRemoveDashes(
				value.get("alg", defaultAlgorithm).asString());
	} else if (defaultAlgorithm.size() != 0) {
		return defaultAlgorithm;
	}
	throw std::string("Could not determine algorithm");
}

sb_GlobalCtx GSECrypto::context() {
	return sbCtx;
}

sb_RngCtx GSECrypto::randomContext() {
	return rngCtx;
}

std::string GSECrypto::toString(const Json::Value & value) {
	Json::FastWriter writer;
	return writer.write(value);
}

std::string GSECrypto::fail(const std::string & error) {
	Json::Value value;
	value["error"] = error
			+ (lastError.length() != 0 ? " [" + lastError + "]" : "");
	lastError = "";
	return toString(value);
}

std::string GSECrypto::random(const std::string & inputString) {
	try {
		Json::Value args;
		readJson(inputString, args);
		if (args.isMember("size")) {
			Json::Value sizeValue(args["size"]);
			if (sizeValue.isInt()) {
				size_t size = sizeValue.asInt();

				DataTracker dt(size);

				int rc = hu_RngGetBytes(rngCtx, dt.dataLen, dt.data, sbCtx);
				if (rc != SB_SUCCESS) {
					throw gsecrypto::util::errorMessage(
							"Could not get random bytes", rc);
				}

				Json::Value toReturn;
				toReturn["output"] = Provider::toJson(dt);
				return toString(toReturn);
			} else {
				throw std::string("size must be an integer");
			}
		} else {
			throw std::string("size is missing");
		}
	} catch (std::string & error) {
		return fail(error);
	}
}

} /* namespace gsecrypto */
