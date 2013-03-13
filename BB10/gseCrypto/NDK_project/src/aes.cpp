/*
 * AES.cpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#include "AES.hpp"

namespace webworks {

AES::AES(GSECrypto * owner):Provider(owner) {
}

AES::~AES() {
}

bool AES::doesSupport(const std::string & algorithm) {
	return "aes" == algorithm;
}

} /* namespace webworks */
