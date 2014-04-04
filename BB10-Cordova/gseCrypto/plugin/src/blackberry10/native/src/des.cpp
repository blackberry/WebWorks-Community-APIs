
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
#include "des.hpp"

#include <hudes.h>
#include <sbreturn.h>
#include <sstream>

#include "gseCrypto_ndk.hpp"
#include "util/util.hpp"

using namespace gsecrypto::util;

namespace gsecrypto {

DES::DES(GSECryptoNDK & owner) :
		Provider(owner) {
}

DES::~DES() {
}

bool DES::doesSupport(const std::string & algorithm) {
	return ("des" == algorithm) || ("3des" == algorithm);
}

/**
 * See crypt for input/output
 */
Json::Value DES::encrypt(const std::string & algorithm, Json::Value & args) {
	return crypt(algorithm, args, true);
}

/**
 * See crypt for input/output
 */
Json::Value DES::decrypt(const std::string & algorithm, Json::Value & args) {
	return crypt(algorithm, args, false);
}

/**
 * Expected input:
 * {
 * 	alg: "des", [or "3des"]
 * 	mode: "cbc", [or "ecb"]
 * 	key1: key1 data,
 * 	key2: key2 data, [if 3des]
 * 	key3: key3 data, [if 3des]
 * 	iv: iv data (16 bytes), [if cbc mode]
 * 	input: data to encrypt or decrypt
 * }
 *
 * Good output:
 * {
 * 	output: data
 * }
 */
Json::Value DES::crypt(const std::string & algorithm, Json::Value & args,
		bool isEncrypt) {

    int mode;

	if (!args.isMember("key1")) {
		throw std::string("key1 missing");
	}
	if("3des" == algorithm) {
	    if (!args.isMember("key2")) {
            throw std::string("key2 missing");
        }
        if (!args.isMember("key3")) {
            throw std::string("key3 missing");
        }
	}

	if (!args.isMember("input")) {
		throw std::string("input missing");
	}
	if (!args.isMember("mode")) {
		throw std::string("mode missing");
	}

	std::string modeString(
			gsecrypto::util::lowerCaseRemoveDashes(args["mode"].asString()));
	if ("cbc" == modeString) {
	    mode = SB_DES_CBC;
	    if (!args.isMember("iv")) {
		    throw std::string("iv required for CBC mode");
	    }
	} else if("ecb" == modeString) {
	    mode = SB_DES_ECB;
	} else {
	    throw std::string("Only CBC and ECB currently supported");
	}

	DataTracker input;
	getData(args["input"], input);

	DataTracker result(input.dataLen);

	if (input.dataLen == 0) {
		Json::Value toReturn;
		toReturn["output"] = toJson(result);
		return toReturn;
	}

	DataTracker keyBytes1;
	DataTracker keyBytes2(SB_DES_KEY_SIZE);
	DataTracker keyBytes3(SB_DES_KEY_SIZE);
	getData(args["key1"], keyBytes1);

	if("3des" == algorithm) {
	    getData(args["key2"], keyBytes2);
	    getData(args["key3"], keyBytes3);
	}

	if(   keyBytes1.dataLen != SB_DES_KEY_SIZE
       || keyBytes2.dataLen != SB_DES_KEY_SIZE
       || keyBytes3.dataLen != SB_DES_KEY_SIZE ) {
	    std::stringstream stream;
        stream << "Key length must be " << SB_DES_KEY_SIZE << " bytes.";
        throw stream.str();
	}

	if( (input.dataLen % SB_DES_BLOCK_SIZE) != 0 ) {
	    std::stringstream stream;
        stream << "Input not multiple of " << SB_DES_BLOCK_SIZE << " bytes. Use padding.";
        throw stream.str();
	}

    DESParams params(*this, ("3des" == algorithm) ? SB_DES_TDES : SB_DES_DES,
                     mode, SB_DES_BLOCK_SIZE, false);
    DESKey key(params, keyBytes1, keyBytes2, keyBytes3);
    DataTracker iv(SB_DES_IV_SIZE);

    if( mode == SB_DES_CBC ) {
        getData(args["iv"], iv);
        if( iv.dataLen != SB_DES_IV_SIZE ) {
            std::stringstream stream;
            stream << "IV not " << SB_DES_IV_SIZE << " bytes.";
            throw stream.str();
        }
    }

    DESContext context(params, key, mode, iv);
	context.crypt(input, result, isEncrypt);

	Json::Value toReturn;
	toReturn["output"] = toJson(result);
	return toReturn;
}

DESParams::DESParams(DES & own, int alg, int mode, size_t blockLength, bool withRandom) :
		owner(own), params(NULL) {

	int rc = hu_DESParamsCreate(alg, mode, SB_DES_PARITY_OFF, SB_DES_WEAK_KEY_OFF,
            withRandom ? owner.randomContext() : NULL, NULL, &params, owner.context());

	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not create DES params", rc);
	}
}

DESParams::~DESParams() {
	if (params != NULL) {
		hu_DESParamsDestroy(&params, owner.context());
		params = NULL;
	}
}

DESKey::DESKey(DESParams & own, DataTracker & dt1, DataTracker & dt2, DataTracker & dt3) :
		params(own), key(NULL) {
	int rc = hu_DESKeySet(params.params, dt1.dataLen, dt1.data, dt2.dataLen,
                       dt2.data, dt3.dataLen, dt3.data, &key, params.owner.context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not generate DES key", rc);
	}
}


DESKey::~DESKey() {
	if (key != NULL) {
		hu_DESKeyDestroy(params.params, &key, params.owner.context());
	}
}


DESContext::DESContext(DESParams & p, DESKey & key, int mode, DataTracker & iv) :
		params(p), context(NULL) {
	int rc = hu_DESBeginV2(params.params, key.key, mode, iv.dataLen, iv.data,
			&context, params.owner.context());
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not create DES context", rc);
	}
}

DESContext::~DESContext() {
	if (context != NULL) {
		hu_DESEnd(&context, params.owner.context());
		context = NULL;
	}
}

void DESContext::crypt(DataTracker & in, DataTracker & out, bool isEncrypt) {
	int rc(0);
	if (isEncrypt) {
		rc = hu_DESEncrypt(context, in.dataLen, in.data, out.data,
				params.owner.context());
	} else {
		rc = hu_DESDecrypt(context, in.dataLen, in.data, out.data,
				params.owner.context());
	}
	if (rc != SB_SUCCESS) {
		throw errorMessage("Could not encrypt data", rc);
	}
}

} /* namespace gsecrypto */
