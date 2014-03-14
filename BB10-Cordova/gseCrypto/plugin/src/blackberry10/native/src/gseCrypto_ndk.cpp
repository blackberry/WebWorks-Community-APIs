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

#include "gseCrypto_ndk.hpp"
#include "gseCrypto_js.hpp"
#include "aes.hpp"
#include "util/util.hpp"
#include "sha.hpp"

class GSECryptoJS;

namespace gsecrypto {

GSECryptoNDK::GSECryptoNDK(GSECryptoJS *owner) {
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

GSECryptoNDK::~GSECryptoNDK() {
	if (sbCtx != NULL) {
		hu_GlobalCtxDestroy(&sbCtx);
		sbCtx = NULL;
	}
}

std::string GSECryptoNDK::hash(const std::string& inputString) {
	try {
		Json::Value args;
		readJson(inputString, args);

		std::string alg(getAlgorithm(args, ""));
		Provider * p = findProvider(alg);

		return toString(p->hash(alg, args));

	} catch (std::string & error) {
		return fail(error);
	}
}

std::string GSECryptoNDK::encrypt(const std::string & input) {
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

std::string GSECryptoNDK::decrypt(const std::string & input) {
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

void GSECryptoNDK::readJson(const std::string & inputStream, Json::Value & value) {
	Json::Reader reader;
	if (!reader.parse(inputStream, value)) {
		throw std::string("Could not read JSON input");
	}
}

Provider * GSECryptoNDK::findProvider(const std::string & algorithm) {
	int count(0);
	for (std::list<Provider*>::iterator i = providers.begin();
			i != providers.end(); ++i) {
		++count;
		if ((*i)->doesSupport(algorithm)) {
			return (*i);
		}
	}
	std::stringstream stream;
	stream << "Algorithm '" << algorithm << "' not supported.";
	throw stream.str();
}

std::string GSECryptoNDK::getAlgorithm(Json::Value & value,
		const std::string & defaultAlgorithm) {
	if (value.isMember("alg")) {
		return gsecrypto::util::lowerCaseRemoveDashes(
				value.get("alg", defaultAlgorithm).asString());
	} else if (defaultAlgorithm.size() != 0) {
		return defaultAlgorithm;
	}
	throw std::string("Could not determine algorithm");
}

sb_GlobalCtx GSECryptoNDK::context() {
	return sbCtx;
}

sb_RngCtx GSECryptoNDK::randomContext() {
	return rngCtx;
}

std::string GSECryptoNDK::toString(const Json::Value & value) {
	Json::FastWriter writer;
	return writer.write(value);
}

std::string GSECryptoNDK::fail(const std::string & error) {
	Json::Value value;
	value["error"] = error
			+ (lastError.length() != 0 ? " [" + lastError + "]" : "");
	lastError = "";
	return toString(value);
}

std::string GSECryptoNDK::random(const std::string & inputString) {
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
