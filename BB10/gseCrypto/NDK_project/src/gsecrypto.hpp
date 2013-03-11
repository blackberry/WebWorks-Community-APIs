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

#ifndef GSECrypto_HPP_
#define GSECrypto_HPP_

#include <string>
#include <pthread.h>
#include <huctx.h>

class GSECryptoJS;

namespace webworks {

class GSECrypto {
public:
	explicit GSECrypto(GSECryptoJS *parent = NULL);
	virtual ~GSECrypto();

	std::string hash(const std::string& inputString);
private:
	std::string toString(unsigned char * data, size_t dataLen);
	GSECryptoJS * parent;

	sb_GlobalCtx sbCtx;
	std::string lastError;
	std::string lastMessage;
};

} // namespace

#endif /* GSECrypto_H_ */
