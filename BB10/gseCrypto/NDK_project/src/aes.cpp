/*
 * AES.cpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#include "AES.hpp"

#include <huaes.h>
#include <sbreturn.h>

#include "gsecrypto.hpp"

#include "util/util.hpp"

using namespace gsecrypto::util;

namespace webworks {

AES::AES(GSECrypto * owner) :
		Provider(owner) {
}

AES::~AES() {
}

bool AES::doesSupport(const std::string & algorithm) {
	return "aes" == algorithm;
}

Json::Value AES::generateKey(const std::string & algorithm,
		Json::Value & args) {
	AESParams params(owner);
	params.create(SB_AES_CBC, SB_AES_128_BLOCK_BITS, true);

	AESKey key(params);
	key.generate(128);

	DataTracker dt;
	key.get(dt);

	Json::Value keyData = toJson(dt.data,dt.dataLen);
	Json::Value toReturn;
	toReturn["key"] = keyData;

	return toReturn;
}

AESParams::AESParams(GSECrypto * own) :
		owner(own), params(NULL) {
}

AESParams::~AESParams() {
	if (params != NULL) {
		hu_AESParamsDestroy(&params, owner->context());
		params = NULL;
	}
}

void AESParams::create(int mode, size_t blockLength, bool withRandom) {
	int rc = hu_AESParamsCreate(mode, blockLength,
			withRandom ? owner->randomContext() : NULL, NULL, &params,
			owner->context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not create AES params",rc);
	}
}

AESKey::AESKey(AESParams & own) :
		owner(own), key(NULL) {
}

AESKey::~AESKey() {
	if (key != NULL) {
		hu_AESKeyDestroy(owner.params, &key, owner.owner->context());
	}
}

void AESKey::generate(size_t size) {
	if (key != NULL) {
		throw std::string("Key already exists");
	}
	int rc = hu_AESKeyGen(owner.params, size, &key, owner.owner->context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not generate AES key", rc);
	}
}

void AESKey::get(DataTracker & dt) {
	size_t keyLen = 0;

	int rc = hu_AESKeyGet(owner.params,key,&keyLen,NULL,owner.owner->context());
	if (rc != SB_SUCCESS) {
		throw std::string("Could not get AES key size");
	}

	dt.data = new unsigned char [keyLen/8];
	dt.dataLen = keyLen / 8;

	rc = hu_AESKeyGet(owner.params,key,&keyLen,dt.data,owner.owner->context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not get AES key content",rc);
	}
}


} /* namespace webworks */
