/*
 * Copyright 2014 BlackBerry Limited.
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

#ifndef PASSWORDCRYPTONDK_HPP_
#define PASSWORDCRYPTONDK_HPP_

#include <string>
#include <pthread.h>

class PasswordCryptoJS;

namespace webworks {

class PasswordCryptoNDK {
public:
	explicit PasswordCryptoNDK(PasswordCryptoJS *parent = NULL);
	virtual ~PasswordCryptoNDK();

	// The extension methods are defined here

	std::string ping();

	std::string pbkdf2_Sync(const std::string& jsonArgs);

	void pbkdf2_Async(const std::string& callbackId, const std::string& inputString);

private:
	PasswordCryptoJS *m_pParent;
};

} // namespace webworks

#endif /* TEMPLATENDK_H_ */
