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
#include "AES.hpp"

#include <huaes.h>
#include <sbreturn.h>
#include <sstream>

#include "gsecrypto.hpp"

#include "util/util.hpp"

using namespace gsecrypto::util;

namespace gsecrypto {

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

	AESParams params(*this,SB_AES_CBC, SB_AES_128_BLOCK_BITS, true);

	DataTracker dt;
	AESKey key(params,keySize);
	key.get(dt);

	Json::Value keyData = toJson(dt.data,dt.dataLen);
	Json::Value toReturn;
	toReturn["key"] = keyData;

	return toReturn;
}

Json::Value AES::encrypt(const std::string & algorithm, Json::Value & args) {
	return crypt(algorithm,args,true);
}

Json::Value AES::decrypt(const std::string & algorithm, Json::Value & args) {
	return crypt(algorithm,args,false);
}

Json::Value AES::crypt(const std::string & algorithm, Json::Value & args,bool isEncrypt) {
	if (!args.isMember("key")) {
		throw std::string("key missing");
	}
	if (!args.isMember("input")) {
		throw std::string("input missing");
	}
	if (!args.isMember("iv")) {
		throw std::string("iv missing");
	}
	if (!args.isMember("mode")) {
		throw std::string("mode missing");
	}

	std::string modeString(gsecrypto::util::lowerCaseRemoveDashes(args["mode"].asString()));
	if ("cbc" != modeString) {
		throw std::string("Only CBC currently supported");
	}

	DataTracker input;
	getData(args["input"],input);

	DataTracker result(input.dataLen);

	if (input.dataLen==0) {
		Json::Value toReturn;
		toReturn["output"] = toJson(result);
		return toReturn;
	}

	DataTracker keyBytes;
	getData(args["key"],keyBytes);

	DataTracker iv;
	getData(args["iv"],iv);

	int mode = SB_AES_CBC;

	AESParams params(*this,SB_AES_CBC,SB_AES_128_BLOCK_BITS,false);
	AESKey key(params,keyBytes);
	AESContext context(params,key,mode,iv);
	context.crypt(input,result,isEncrypt);

	Json::Value toReturn;
	toReturn["output"] = toJson(result);
	return toReturn;
}

AESParams::AESParams(AES & own, int mode, size_t blockLength, bool withRandom) :
		owner(own), params(NULL) {
	int rc = hu_AESParamsCreate(mode, blockLength,
			withRandom ? owner.randomContext() : NULL, NULL, &params,
			owner.context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not create AES params",rc);
	}
}

AESParams::~AESParams() {
	if (params != NULL) {
		hu_AESParamsDestroy(&params, owner.context());
		params = NULL;
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

void AESContext::crypt(DataTracker & in, DataTracker & out, bool isEncrypt) {
	int rc(0);
	if (isEncrypt) {
		rc = hu_AESEncrypt(context,in.dataLen,in.data,out.data,params.owner.context());
	} else {
		rc = hu_AESDecrypt(context,in.dataLen,in.data,out.data,params.owner.context());
	}
	if (rc!=SB_SUCCESS) {
		throw errorMessage("Could not encrypt data",rc);
	}
}

} /* namespace gsecrypto */
