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
#ifndef SHA_HPP_
#define SHA_HPP_

#include "provider.hpp"

namespace gsecrypto {

/**
 * Provider implementing SHA functionality.
 */
class SHA: public Provider {
public:
	SHA(class GSECrypto &);
	virtual ~SHA();

	virtual bool doesSupport(const std::string &);
	virtual Json::Value hash(const std::string & algorithm, Json::Value &);

};

} /* namespace gsecrypto */
#endif /* SHA_HPP_ */
