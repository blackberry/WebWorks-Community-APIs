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
#include "util/util.hpp"

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

class DataTracker {
public:
	DataTracker() :
			data(0), dataLen(0) {
	}
	;
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

// Take in input and return a value
std::string GSECrypto::hash(const std::string& inputString) {
	DataTracker data;
	std::string toReturn;

	try {

		Json::Value args;
		Json::Reader reader;
		if (!reader.parse(inputString, args, 0)) {
			throw "Input is not decodable Json";
		}

		if (args.isMember("hex")) {
			gsecrypto::util::fromHex(args["hex"].asString(), data.data, data.dataLen);
		} else if (args.isMember("b64")) {
			gsecrypto::util::fromB64(args["b64"].asString(), data.data, data.dataLen);
		} else if (args.isMember("raw")) {
			data.setData(args["raw"].asString());
		} else {
			throw "Input must have one of hex,b64,raw";
		}

		std::string alg = args.get("alg", "SHA1").asString();
		std::transform(alg.begin(), alg.end(), alg.begin(), tolower);
		alg.erase(std::remove(alg.begin(), alg.end(), '-'), alg.end());


		size_t digestLen = 0;

		int (*algFunc)(size_t, sb_YieldCtx, size_t, const unsigned char *,
				unsigned char *, sb_GlobalCtx) = NULL;

		if (alg == "sha1") {
			digestLen = SB_SHA1_DIGEST_LEN;
			algFunc = hu_SHA1Msg;
		} else if (alg == "sha224") {
			digestLen = SB_SHA224_DIGEST_LEN;
			algFunc = hu_SHA224Msg;
		} else if (alg == "sha256") {
			digestLen = SB_SHA256_DIGEST_LEN;
			algFunc = hu_SHA256Msg;
		} else if (alg == "sha384") {
			digestLen = SB_SHA384_DIGEST_LEN;
			algFunc = hu_SHA384Msg;
		} else if (alg == "sha512") {
			digestLen = SB_SHA512_DIGEST_LEN;
			algFunc = hu_SHA512Msg;
		} else {
			throw "Unknown SHA operation";
		}

		lastMessage = "";
		std::stringstream tmp;
		tmp << "dataLength: " << data.dataLen << " data: ";
		for (size_t i = 0; i < data.dataLen; ++i) {
			tmp << data.data[i];
		}
		tmp << "\n";
		tmp << "Input: " << inputString;
		lastMessage = tmp.str();

		unsigned char digest[digestLen];
		for (size_t i = 0; i < digestLen; ++i) {
			digest[i] = i;
		}
		if (SB_SUCCESS
				!= algFunc(digestLen, NULL, data.dataLen, data.data, digest,
						sbCtx)) {
			throw "Could not call hash function";
		}

		toReturn = toString(digest, digestLen);
	} catch (const char * error) {
		Json::Value errorJ;
		Json::FastWriter writer;
		errorJ["error"] = error;
		return writer.write(errorJ);
	}

	return toReturn;
}

std::string GSECrypto::toString(unsigned char * data, size_t dataLen) {
	Json::Value toReturn;
	Json::FastWriter writer;
	toReturn["hex"] = gsecrypto::util::toHex(data, dataLen);
	toReturn["b64"] = gsecrypto::util::toB64(data, dataLen);
	if (lastMessage.length() != 0) {
		toReturn["lastMessage"] = lastMessage;
		lastMessage = "";
	}

	return writer.write(toReturn);
}

} /* namespace webworks */
