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
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "gsecrypto.hpp"
#include "gsecryptojs.hpp"

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
}

GSECrypto::~GSECrypto() {
}

// These methods are the true native code we intend to reach from WebWorks
std::string GSECrypto::templateTestString() {
	return "Template Test Function";
}

// Take in input and return a value
std::string GSECrypto::templateTestString(const std::string& inputString) {
	return "Template Test Function, got: " + inputString;
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
