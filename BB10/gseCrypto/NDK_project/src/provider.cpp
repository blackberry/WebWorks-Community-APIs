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
#include "provider.hpp"

#include "util/util.hpp"

#include "gsecrypto.hpp"

#include <json/writer.h>

namespace gsecrypto {

Provider::Provider(GSECrypto & own) :
		owner(own) {
}

Provider::~Provider() {

}

void Provider::getData(Json::Value & args, DataTracker & data) {
	if (args.isMember("hex")) {
		gsecrypto::util::fromHex(args["hex"].asString(), data.data,
				data.dataLen);
	} else if (args.isMember("b64")) {
		gsecrypto::util::fromB64(args["b64"].asString(), data.data,
				data.dataLen);
	} else if (args.isMember("raw")) {
		data.setData(args["raw"].asString());
	} else {
		throw std::string("Input must have one of hex,b64,raw");
	}
}

sb_GlobalCtx Provider::context() {
	return owner.context();
}

sb_RngCtx Provider::randomContext() {
	return owner.randomContext();
}

Json::Value Provider::toJson(unsigned char * data, size_t dataLen) {
	Json::Value toReturn;
	Json::FastWriter writer;
	toReturn["hex"] = gsecrypto::util::toHex(data, dataLen);
	toReturn["b64"] = gsecrypto::util::toB64(data, dataLen);
	toReturn["raw"] = std::string((char*) data, dataLen);
	return toReturn;
}

Json::Value Provider::toJson(DataTracker & dt) {
	return toJson(dt.data, dt.dataLen);
}

Json::Value Provider::generateKey(const std::string & algorithm,
		Json::Value & input) {
	throw std::string("Key generation not supported");
}

Json::Value Provider::hash(const std::string & algorithm, Json::Value & input) {
	throw std::string("Hash not supported");

}

Json::Value Provider::encrypt(const std::string & algorithm,
		Json::Value & input) {
	throw std::string("Encrypt not supported");

}

Json::Value Provider::decrypt(const std::string & algorithm,
		Json::Value & input) {
	throw std::string("Decrypt not supported");

}

Json::Value Provider::sign(const std::string & algorithm, Json::Value & input) {
	throw std::string("Sign not supported");

}

Json::Value Provider::verify(const std::string & algorithm,
		Json::Value & input) {
	throw std::string("Verify not supported");
}

}
