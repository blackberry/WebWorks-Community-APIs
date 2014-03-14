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
#ifndef _UTIL_UTIL_
#define _UTIL_UTIL_

#include <string>
#include <stddef.h>
#include <algorithm>

namespace gsecrypto {
/**
 * A bunch of helper functions... Utility like!
 */
namespace util {
std::string toHex(unsigned char * data, size_t dataLen);
std::string toB64(unsigned char * data, size_t dataLen);
void fromHex(std::string encoded, unsigned char * & data, size_t & dataLen);
void fromB64(std::string encoded, unsigned char * & data, size_t & dataLen);
int nibble(const char c);

std::string errorMessage(const char * message, int error);

std::string lowerCaseRemoveDashes(const std::string &);
}
} /* namespace util */

#endif /* _UTIL_UTIL_ */
