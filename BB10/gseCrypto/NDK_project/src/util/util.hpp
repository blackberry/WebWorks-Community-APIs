#ifndef _UTIL_UTIL_HPP
#define _UTIL_UTIL_HPP

#include <string>
#include <stddef.h>
#include <algorithm>

namespace gsecrypto {
namespace util {
std::string toHex(unsigned char * data, size_t dataLen);
std::string toB64(unsigned char * data, size_t dataLen);
void fromHex(std::string encoded, unsigned char * & data,
		size_t & dataLen);
void fromB64(std::string encoded, unsigned char * & data,
		size_t & dataLen);
int nibble(const char c);
}
}

#endif
