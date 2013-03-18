/*
 * SHA.cpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#include <husha1.h>
#include <husha2.h>
#include <sbreturn.h>

#include "sha.hpp"
#include "datatracker.hpp"
#include "util/util.hpp"

namespace webworks {

SHA::SHA(GSECrypto & owner) : Provider(owner) {
}

SHA::~SHA() {
}

bool SHA::doesSupport(const std::string & algorithm) {
	return "sha" == algorithm || "sha1" == algorithm || "sha224" == algorithm
			|| "sha256" == algorithm || "sha384" == algorithm
			|| "sha512" == algorithm;
}

Json::Value SHA::hash(const std::string & alg, Json::Value & args) {
	DataTracker data;

	getData(args,data);

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

	unsigned char digest[digestLen];
	for (size_t i = 0; i < digestLen; ++i) {
		digest[i] = i;
	}
	if (SB_SUCCESS
			!= algFunc(digestLen, NULL, data.dataLen, data.data, digest,
					context())) {
		throw std::string("Could not call hash function");
	}

	return toJson(digest,digestLen);
}

} /* namespace webworks */
