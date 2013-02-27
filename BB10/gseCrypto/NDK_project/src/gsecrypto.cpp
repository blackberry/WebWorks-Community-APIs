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

#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "gsecrypto.hpp"
#include "gsecryptojs.hpp"

#include <huctx.h>
#include <sbreturn.h>
#include <husha1.h>
#include <husha2.h>
#include <hugse56.h>

class GSECryptoJS;

namespace webworks {

GSECrypto::GSECrypto(GSECryptoJS *parent) {
	m_pParent = parent;
	templateProperty = 50;
	templateThreadCount = 1;
	m_thread = 0;
	pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	threadHalt = true;


	sbCtx = NULL;

	try {
		int error = hu_GlobalCtxCreateDefault(&sbCtx);
		if (error!=SB_SUCCESS) {
			throw "Failed to create global context";
		}

		error = hu_RegisterSbg56(sbCtx);
		if (error!=SB_SUCCESS) {
			throw "Failed to register sbg 5.6";
		}

		error = hu_InitSbg56(sbCtx);
		if (error!=SB_SUCCESS) {
			throw "Failed to init sbg 5.6";
		}
	} catch (const char * message) {
		lastError = message;
	}
}

GSECrypto::~GSECrypto() {
	if (sbCtx!=NULL) {
		hu_GlobalCtxDestroy(&sbCtx);
		sbCtx = NULL;
	}
}

// These methods are the true native code we intend to reach from WebWorks
std::string GSECrypto::templateTestString() {
	return "Template Test Function";
}

class DataTracker {
public:
	DataTracker() : data(0), dataLen(0) { };
	virtual ~DataTracker() {
		cleanUp();
	}
	void cleanUp() {
		if (data!=NULL) {
			delete [] data;
			data = NULL;
			dataLen = 0;
		}
	}
	void setData(std::string in) {
		cleanUp();
		dataLen = in.length();
		data = new unsigned char[dataLen+1];
		strcpy((char*)data,in.data());
	}
	unsigned char * data;
	size_t dataLen;
};

// Take in input and return a value
std::string GSECrypto::templateTestString(const std::string& inputString) {
	DataTracker data;
	std::string toReturn;

	try {

		Json::Value args;
		Json::Reader reader;
		if (!reader.parse(inputString,args,0)) {
			throw "Input is not decodable Json";
		}

		if (args.isMember("hex")) {
			fromHex(args["hex"].asString(),data.data,data.dataLen);
		} else if (args.isMember("b64")) {
			fromB64(args["b64"].asString(),data.data,data.dataLen);
		} else if (args.isMember("raw")) {
			data.setData(args["raw"].asString());
		} else {
			throw "Input must have one of hex,b64,raw";
		}

		std::string alg = args.get("alg","SHA1").asString();
		std::transform(alg.begin(),alg.end(),alg.begin(),tolower);

		size_t digestLen = 0;

		int (*algFunc)(size_t,sb_YieldCtx,size_t,const unsigned char *, unsigned char *, sb_GlobalCtx) = NULL;


		if (alg=="sha1") {
			digestLen = SB_SHA1_DIGEST_LEN;
			algFunc = hu_SHA1Msg;
		} else if (alg=="sha224") {
			digestLen = SB_SHA224_DIGEST_LEN;
			algFunc = hu_SHA224Msg;
		} else if (alg=="sha256") {
			digestLen = SB_SHA256_DIGEST_LEN;
			algFunc = hu_SHA256Msg;
		} else if (alg=="sha384") {
			digestLen = SB_SHA384_DIGEST_LEN;
			algFunc = hu_SHA384Msg;
		} else if (alg=="sha512") {
			digestLen = SB_SHA512_DIGEST_LEN;
			algFunc = hu_SHA512Msg;
		} else {
			throw "Unknown SHA operation";
		}

		lastMessage = "";
		std::stringstream tmp;
		tmp << "dataLength: " << data.dataLen << " data: ";
		for (size_t i=0; i<data.dataLen; ++i) {
			tmp << data.data[i];
		}
		tmp << "\n";
		tmp << "Input: " << inputString;
		lastMessage = tmp.str();

		unsigned char digest[digestLen];
		for (size_t i=0; i<digestLen; ++i) {
			digest[i] = i;
		}
		if (SB_SUCCESS!=algFunc(digestLen,NULL,data.dataLen,data.data,digest,sbCtx)) {
			throw "Could not call hash function";
		}

		toReturn = toString(digest,digestLen);
	} catch (const char * error) {
		Json::Value errorJ;
		Json::FastWriter writer;
		errorJ["error"] = error;
		return writer.write(errorJ);
	}

	return toReturn;
}

std::string GSECrypto::toString(unsigned char * data, size_t dataLen) {
	Json::Value toReturn;
	Json::FastWriter writer;
	toReturn["hex"] = toHex(data,dataLen);
	toReturn["b64"] = toB64(data,dataLen);
	if (lastMessage.length()!=0) {
		toReturn["lastMessage"] = lastMessage;
		lastMessage = "";
	}

	return writer.write(toReturn);
}

std::string GSECrypto::toB64(unsigned char * data, size_t dataLen) {
	const char * b64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string toReturn;
	for (size_t i=0; i<dataLen; i+=3) {
		bool bgood = i+1 < dataLen;
		bool cgood = i+2 < dataLen;

		unsigned char a = data[i];
		unsigned char b = bgood ? data[i+1] : 0;
		unsigned char c = cgood ? data[i+1] : 0;

		toReturn += b64Chars[a>>2];
		toReturn += b64Chars[((a&3)<<4)|b>>4];
		toReturn += bgood ? b64Chars[((b&0xf)<<2)|(c>>6)] : '=';
		toReturn += cgood ? b64Chars[c&0x3f] : '=';
	}

	return toReturn;
}

void GSECrypto::fromHex(std::string en, unsigned char * & data, size_t & dataLen) {
	std::string encoded;
	for (size_t i=0; i<en.length(); ++i) {
		char c = tolower(en[i]);
		if ((c>='a' && c<='f') || (c>='0' && c<='9')) {
			encoded += c;
		}
	}

	if (encoded.length()==0) {
		data = NULL;
		dataLen = 0;
		return;
	}

	if (encoded.length()%2!=0) {
		throw "Data length must be multiple of 2";
	}

	dataLen = encoded.length()/2;
	data = new unsigned char[dataLen];

	int offset = 0;
	for (size_t i =0; i<dataLen; ++i) {
		data[i] = nibble(encoded[offset++]);
		data[i] = nibble(encoded[offset++]);
	}
}

int GSECrypto::nibble(const char c) {
	if (c>='a') {
		return 10 + c - 'a';
	} else {
		return c - '0';
	}
}

void GSECrypto::fromB64(std::string encoded, unsigned char * & data, size_t & dataLen) {
	std::string encoded2;
	for (size_t i=0; i<encoded.length(); ++i) {
		char c = encoded[i];
		if ((c>='A' && c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9') || c=='+' || c=='/' || c=='=') {
			encoded2+=c;
		}
	}

	if (encoded2.length()==0) {
		data = NULL;
		dataLen = 0;
		return;
	}

	if (encoded2.length()%4!=0) {
		throw "Base64 encoded length should by multiple of 4";
	}

	dataLen = (encoded2.length()/4)*3;

	if (encoded2[encoded2.length()-1]=='=') {
		dataLen--;
		if (encoded2[encoded2.length()-2]=='=') {
			dataLen--;
		}
	}

	data = new unsigned char[dataLen];

	int offset = 0;
	size_t outOffset = 0;

	for (size_t i=0; i<dataLen; i+=3) {
		unsigned char v[3];
		unsigned char e[4];
		for (int j=0; j<4; ++j) {
			e[j] = encoded2[offset++];
		}
		v[0] = e[0]<<2 | ((e[1]>>4)&0x3);
		v[1] = e[1]<<4 | ((e[2]>>2)&0xf);
		v[2] = e[2]<<6 | ((e[3]&0x2f));
		for (int j=0; j<3 && outOffset < dataLen; ++j) {
			data[outOffset++] = v[j];
		}
	}
}

std::string GSECrypto::toHex(unsigned char * data, size_t dataLen) {
	const char * hexChars = "0123456789abcdef";
	std::string toReturn;
	for (size_t i=0; i < dataLen; ++i) {
		toReturn += hexChars[data[i]>>4];
		toReturn += hexChars[data[i]&15];
	}
	return toReturn;
}

// Get an integer property
std::string GSECrypto::getTemplateProperty() {
	stringstream ss;
	ss << templateProperty;
	return ss.str();
}

// set an integer property
void GSECrypto::setTemplateProperty(const std::string& inputString) {
	templateProperty = (int) strtoul(inputString.c_str(), NULL, 10);
}

// Asynchronous call
void GSECrypto::templateTestAsync() {
	templateCallback();
}

// Asynchronous call returning JSON data
void GSECrypto::templateTestAsyncJSON() {
	templateCallbackJSON();
}

// Asynchronous callback
void GSECrypto::templateCallback() {
	std::string event = "community.gseCrypto.aSyncCallback";
	m_pParent->NotifyEvent(event);
}

// Asynchronous callback with JSON data object
void GSECrypto::templateCallbackJSON() {
	Json::FastWriter writer;
	Json::Value root;
	root["GSECryptoJSONString"] = "JSON String";
	root["GSECryptoJSONInt"] = 85;

	std::string event = "community.gseCrypto.aSyncJSONCallback";
	m_pParent->NotifyEvent(event + " " + writer.write(root));
}

// Asynchronous callback with JSON data input and output
void GSECrypto::templateCallbackJSONio(const std::string& inputString) {
	std::string event = "community.gseCrypto.aSyncJSONCallbackResult";

	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		Json::Value error;
		error["result"] = "Cannot parse JSON object";
		m_pParent->NotifyEvent(event + " " + writer.write(error));
	} else {
		root["result"] = root["value1"].asInt() + root["value2"].asInt();
		m_pParent->NotifyEvent(event + " " + writer.write(root));
	}
}

// Thread functions
// The following functions are for controlling a Thread in the extension

// The actual thread (must appear before the startThread method)
// Loops and runs the callback method
void* TemplateThread(void* parent) {
	GSECrypto *pParent = static_cast<GSECrypto *>(parent);

	// Loop calls the callback function and continues until stop is set
	while (!pParent->isThreadHalt()) {
		pParent->templateThreadCallback();
		sleep(1);
	}

	return NULL;
}

// Starts the thread and returns a message on status
std::string GSECrypto::templateStartThread() {
	if (!m_thread) {
		int rc;
	    rc = pthread_mutex_lock(&mutex);
	    threadHalt = false;
	    rc = pthread_cond_signal(&cond);
	    rc = pthread_mutex_unlock(&mutex);

		pthread_attr_t thread_attr;
		pthread_attr_init(&thread_attr);
		pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

		pthread_create(&m_thread, &thread_attr, TemplateThread,
				static_cast<void *>(this));
		pthread_attr_destroy(&thread_attr);
		return "Thread Started";
	} else {
		return "Thread Running";
	}
}

// Sets the stop value
std::string GSECrypto::templateStopThread() {
	int rc;
	// Request thread to set prevent sleep to false and terminate
	rc = pthread_mutex_lock(&mutex);
	threadHalt = true;
	rc = pthread_cond_signal(&cond);
	rc = pthread_mutex_unlock(&mutex);

    // Wait for the thread to terminate.
    void *exit_status;
    rc = pthread_join(m_thread, &exit_status) ;

	// Clean conditional variable and mutex
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);

	m_thread = 0;
	threadHalt = true;
	return "Thread stopped";
}

// The callback method that sends an event through JNEXT
void GSECrypto::templateThreadCallback() {
	std::string event = "community.gseCrypto.jsonThreadCallback";
	Json::FastWriter writer;
	Json::Value root;
	root["threadCount"] = templateThreadCount++;
	m_pParent->NotifyEvent(event + " " + writer.write(root));
}

// getter for the stop value
bool GSECrypto::isThreadHalt() {
	int rc;
	bool isThreadHalt;
	rc = pthread_mutex_lock(&mutex);
	isThreadHalt = threadHalt;
	rc = pthread_mutex_unlock(&mutex);
	return isThreadHalt;
}

} /* namespace webworks */
