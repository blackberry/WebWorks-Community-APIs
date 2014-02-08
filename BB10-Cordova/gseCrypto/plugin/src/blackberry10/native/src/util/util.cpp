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
#include "util.hpp"
#include <sstream>
#include <algorithm>

namespace gsecrypto {
namespace util {

std::string toB64(unsigned char * data, size_t dataLen) {
	static const char * b64Chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string toReturn;
	for (size_t i = 0; i < dataLen; i += 3) {
		bool bgood = i + 1 < dataLen;
		bool cgood = i + 2 < dataLen;

		unsigned char a = data[i];
		unsigned char b = bgood ? data[i + 1] : 0;
		unsigned char c = cgood ? data[i + 2] : 0;

		toReturn += b64Chars[a >> 2];
		toReturn += b64Chars[((a & 3) << 4) | b >> 4];
		toReturn += bgood ? b64Chars[((b & 0xf) << 2) | (c >> 6)] : '=';
		toReturn += cgood ? b64Chars[c & 0x3f] : '=';
	}

	return toReturn;
}

void fromHex(std::string en, unsigned char * & data, size_t & dataLen) {
	std::string encoded;
	for (size_t i = 0; i < en.length(); ++i) {
		char c = std::tolower(en[i]);
		if ((c >= 'a' && c <= 'f') || (c >= '0' && c <= '9')) {
			encoded += c;
		}
	}

	if (encoded.length() == 0) {
		data = NULL;
		dataLen = 0;
		return;
	}

	if (encoded.length() % 2 != 0) {
		throw "Data length must be multiple of 2";
	}

	dataLen = encoded.length() / 2;
	data = new unsigned char[dataLen];

	for (size_t i = 0; i < dataLen; ++i) {
		data[i] = (nibble(encoded[i << 1]) << 4)
				| nibble(encoded[(i << 1) + 1]);
	}
}

int nibble(const char c) {
	if (c >= 'a') {
		return 10 + c - 'a';
	} else {
		return c - '0';
	}
}

unsigned char b64Nibble(unsigned char c) {
	if (c >= 'A' && c <= 'Z') {
		return c - 'A';
	} else if (c >= 'a' && c <= 'z') {
		return c - 'a' + 26;
	} else if (c >= '0' && c <= '9') {
		return c - '0' + 52;
	} else if (c == '+') {
		return 62;
	} else if (c == '/') {
		return 63;
	}
	return 0;
}

void fromB64(std::string encoded, unsigned char * & data, size_t & dataLen) {
	std::string encoded2;
	for (size_t i = 0; i < encoded.length(); ++i) {
		char c = encoded[i];
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
				|| (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=') {
			encoded2 += c;
		}
	}

	if (encoded2.length() == 0) {
		data = NULL;
		dataLen = 0;
		return;
	}

	if (encoded2.length() % 4 != 0) {
		throw "Base64 encoded length should by multiple of 4";
	}

	dataLen = (encoded2.length() / 4) * 3;

	if (encoded2[encoded2.length() - 1] == '=') {
		dataLen--;
		if (encoded2[encoded2.length() - 2] == '=') {
			dataLen--;
		}
	}

	data = new unsigned char[dataLen];

	int offset = 0;
	size_t outOffset = 0;

	for (size_t i = 0; i < dataLen; i += 3) {
		unsigned char v[3];
		unsigned char e[4];
		for (int j = 0; j < 4; ++j) {
			e[j] = b64Nibble(encoded2[offset++]);
		}
		v[0] = e[0] << 2 | ((e[1] >> 4) & 0x3);
		v[1] = e[1] << 4 | ((e[2] >> 2) & 0xf);
		v[2] = e[2] << 6 | ((e[3] & 0x3f));
		for (int j = 0; j < 3 && outOffset < dataLen; ++j) {
			data[outOffset++] = v[j];
		}
	}
}

std::string toHex(unsigned char * data, size_t dataLen) {
	const char * hexChars = "0123456789abcdef";
	std::string toReturn;
	for (size_t i = 0; i < dataLen; ++i) {
		toReturn += hexChars[data[i] >> 4];
		toReturn += hexChars[data[i] & 15];
	}
	return toReturn;
}

std::string lowerCaseRemoveDashes(const std::string & input) {
	std::string toReturn(input);
	std::transform(toReturn.begin(), toReturn.end(), toReturn.begin(),
			std::tolower);
	toReturn.erase(std::remove(toReturn.begin(), toReturn.end(), '-'),
			toReturn.end());
	return toReturn;
}

} /* namespace util */
} /* namespace gsecrypto */
