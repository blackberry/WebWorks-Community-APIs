#include "util.hpp"

namespace gsecrypto {
namespace util {

std::string toB64(unsigned char * data, size_t dataLen) {
	const char * b64Chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string toReturn;
	for (size_t i = 0; i < dataLen; i += 3) {
		bool bgood = i + 1 < dataLen;
		bool cgood = i + 2 < dataLen;

		unsigned char a = data[i];
		unsigned char b = bgood ? data[i + 1] : 0;
		unsigned char c = cgood ? data[i + 1] : 0;

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

	int offset = 0;
	for (size_t i = 0; i < dataLen; ++i) {
		data[i] = nibble(encoded[offset++]);
		data[i] = nibble(encoded[offset++]);
	}
}

int nibble(const char c) {
	if (c >= 'a') {
		return 10 + c - 'a';
	} else {
		return c - '0';
	}
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
			e[j] = encoded2[offset++];
		}
		v[0] = e[0] << 2 | ((e[1] >> 4) & 0x3);
		v[1] = e[1] << 4 | ((e[2] >> 2) & 0xf);
		v[2] = e[2] << 6 | ((e[3] & 0x2f));
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
}
} // namespace
