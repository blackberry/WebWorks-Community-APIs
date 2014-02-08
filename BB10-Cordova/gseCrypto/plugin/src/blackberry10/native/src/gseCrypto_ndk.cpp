/*
 * Copyright (c) 2013-2014 BlackBerry Limited
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
#include <json/reader.h>
#include <json/writer.h>
#include "util/util.hpp"
#include "gseCrypto_ndk.hpp"
#include "gseCrypto_js.hpp"


namespace gseCrypto {

GSECryptoNDK::GSECryptoNDK(GSECryptoJS *parent) :
	m_pParent(parent) {
}

GSECryptoNDK::~GSECryptoNDK() {
}

// Cryptographic Hash Functions
std::string GSECryptoNDK::hash(const std::string& inputString) {
	m_pParent->getLog()->debug("hash function called");

	std::string result;

	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;

	bool parse = reader.parse(inputString, root);
	if (!parse) {
		m_pParent->getLog()->error("JSON Parse Error");
		return error("Cannot parse JSON object");
	}

	//JSON arguments format check
	if (!root.isMember("alg")) {
		return error("missing alg field");
	}
	if (!root.isMember("input")) {
		return error("missing input field");
	}

	//extract binary data from JSON
	unsigned char* data;
	size_t dataLen;

	if ( root["input"].isMember("hex") ) {

		//parse hex data
		try {
			gsecrypto::util::fromHex( root["input"]["hex"].asString(), data, dataLen);
		} catch( char const* e ) {
			return error(e);
		}

	} else if( root["input"].isMember("b64") ) {

		//parse base64 data
		try {
			gsecrypto::util::fromB64( root["input"]["b64"].asString(), data, dataLen);
		} catch( char const* e ) {
			return error(e);
		}

	} else {
		return error("no hex or b64 input");
	}

	//determine which algorithm to use
	if ( root["alg"] == "md5" ) {
		result = error("md5 not yet implemented");
	} else if ( root["alg"] == "sha" || root["alg"] == "sha1") {
		result = error("sha1 not yet implemented");
	} else if ( root["alg"] == "sha224" ) {
		result = error("sha224 not yet implemented");
	} else if ( root["alg"] == "sha256" ) {
		result = error("sha256 not yet implemented");
	} else if ( root["alg"] == "sha384" ) {
		result = error("sha384 not yet implemented");
	} else if ( root["alg"] == "sha512" ) {
		result = error("sha512 not yet implemented");
	} else {
		result = error("unsupported hash algorithm");
	}

	return result;
}

//errorMsg: helpful message to be returned to extension caller
//returns: string version of json object { "error":errorMsg }
std::string GSECryptoNDK::error(const std::string& errorMsg) {
	Json::FastWriter writer;
	Json::Value root;
	root["error"] = errorMsg;
	return writer.write(root);
}

} /* namespace gseCrypto */
