/*
 * AES.cpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#include "AES.hpp"

#include <huaes.h>
#include <sbreturn.h>
#include <sstream>

#include "gsecrypto.hpp"

#include "util/util.hpp"

using namespace gsecrypto::util;

namespace webworks {

AES::AES(GSECrypto & owner) :
		Provider(owner) {
}

AES::~AES() {
}

bool AES::doesSupport(const std::string & algorithm) {
	return "aes" == algorithm;
}

Json::Value AES::generateKey(const std::string & algorithm,
		Json::Value & args) {
	size_t keySize = 128;

	if (args.isMember("keySize")) {
		Json::Value keySizeV = args["keySize"];
		if (!keySizeV.isInt()) {
			throw std::string("keySize must be an int");
		}
		keySize = keySizeV.asInt();
		switch(keySize) {
		case 128:
		case 192:
		case 256:
			// awesome!
			break;
		default:
			throw errorMessage("Not a valid key size: ",keySize);
		}
	}

	AESParams params(*this);
	params.create(SB_AES_CBC, SB_AES_128_BLOCK_BITS, true);

	DataTracker dt;
	AESKey key(params,keySize);
	key.get(dt);

	Json::Value keyData = toJson(dt.data,dt.dataLen);
	Json::Value toReturn;
	toReturn["key"] = keyData;

	return toReturn;
}

Json::Value AES::encrypt(const std::string & algorithm, Json::Value & args) {
	if (!args.isMember("key")) {
		throw std::string("key missing");
	}
	if (!args.isMember("input")) {
		throw std::string("input missing");
	}
	if (!args.isMember("iv")) {
		throw std::string("iv missing");
	}

	DataTracker input;
	getData(args["input"],input);

	DataTracker keyBytes;
	getData(args["key"],keyBytes);

	DataTracker iv;
	getData(args["iv"],iv);

	AESParams params(*this);
	params.create(SB_AES_CBC,SB_AES_128_BLOCK_BITS,false);

	AESKey key(params,keyBytes);

	int mode = SB_AES_CBC;

	AESContext context(params,key,mode,iv);

	DataTracker result(input.dataLen);
	if (input.dataLen!=0) {
		context.encrypt(input,result);
	}

	return toJson(result);
}

AESParams::AESParams(AES & own) :
		owner(own), params(NULL) {
}

AESParams::~AESParams() {
	if (params != NULL) {
		hu_AESParamsDestroy(&params, owner.context());
		params = NULL;
	}
}

void AESParams::create(int mode, size_t blockLength, bool withRandom) {
	int rc = hu_AESParamsCreate(mode, blockLength,
			withRandom ? owner.randomContext() : NULL, NULL, &params,
			owner.context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not create AES params",rc);
	}
}

AESKey::AESKey(AESParams & own, DataTracker & dt) :
		params(own), key(NULL) {
	int rc = hu_AESKeySet(params.params,dt.dataLen*8,dt.data,&key,params.owner.context());
	if (rc!=SB_SUCCESS) {
		std::stringstream s;
		s << "Could not set AES Key" << rc;
		s << " dtLen: " << dt.dataLen;
		throw s.str();
	}
}

AESKey::AESKey(AESParams & own, size_t size) : params(own), key(NULL) {
	if (key != NULL) {
		throw std::string("Key already exists");
	}
	int rc = hu_AESKeyGen(params.params, size, &key, params.owner.context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not generate AES key", rc);
	}
}

AESKey::~AESKey() {
	if (key != NULL) {
		hu_AESKeyDestroy(params.params, &key, params.owner.context());
	}
}

void AESKey::get(DataTracker & dt) {
	size_t keyLen = 0;

	int rc = hu_AESKeyGet(params.params,key,&keyLen,NULL,params.owner.context());
	if (rc != SB_SUCCESS) {
		throw std::string("Could not get AES key size");
	}

	dt.data = new unsigned char [keyLen/8];
	dt.dataLen = keyLen / 8;

	rc = hu_AESKeyGet(params.params,key,&keyLen,dt.data,params.owner.context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not get AES key content",rc);
	}
}

AESContext::AESContext(AESParams & p, AESKey & key, int mode, DataTracker & iv) : params(p), context(NULL) {
	int rc = hu_AESBeginV2(params.params,key.key,mode,iv.dataLen,iv.data,&context,params.owner.context());
	if (rc!=SB_SUCCESS) {
		throw errorMessage("Could not create AES context",rc);
	}
}

AESContext::~AESContext() {
	if (context!=NULL) {
		hu_AESEnd(&context,params.owner.context());
		context = NULL;
	}
}

void AESContext::encrypt(DataTracker & in, DataTracker & out) {
	int rc = hu_AESEncrypt(context,in.dataLen,in.data,out.data,params.owner.context());
	if (rc!=SB_SUCCESS) {
		throw errorMessage("Could not encrypt data",rc);
	}
}

} /* namespace webworks */