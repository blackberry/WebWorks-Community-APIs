/*
 * Copyright (c) 2013 BlackBerry Limited
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
#include "???PROJECT_NAME_LOWERCASE???_ndk.hpp"
#include "???PROJECT_NAME_LOWERCASE???_js.hpp"

namespace webworks {

???PROJECT_NAME???_NDK::???PROJECT_NAME???_NDK(???PROJECT_NAME???_JS *parent):
	m_pParent(parent),
	???PROJECT_FUNCTION_START???Property(50),
	???PROJECT_FUNCTION_START???ThreadCount(1),
	threadHalt(true),
	m_thread(0) {
		cond  = PTHREAD_COND_INITIALIZER;
		mutex = PTHREAD_MUTEX_INITIALIZER;
		m_pParent->getLog()->info("???PROJECT_NAME??? Created");
}

???PROJECT_NAME???_NDK::~???PROJECT_NAME???_NDK() {
}

// These methods are the true native code we intend to reach from WebWorks
std::string ???PROJECT_NAME???_NDK::???PROJECT_FUNCTION_START???Test() {
	m_pParent->getLog()->debug("testString");
	return "???PROJECT_NAME??? Test Function";
}

// Take in input and return a value
std::string ???PROJECT_NAME???_NDK::???PROJECT_FUNCTION_START???Test(const std::string& inputString) {
	m_pParent->getLog()->debug("testStringInput");
	return "???PROJECT_NAME??? Test Function, got: " + inputString;
}

// Get an integer property
std::string ???PROJECT_NAME???_NDK::get???PROJECT_FUNCTION???Property() {
	m_pParent->getLog()->debug("get???PROJECT_FUNCTION???Property");
	stringstream ss;
	ss << ???PROJECT_FUNCTION_START???Property;
	return ss.str();
}

// set an integer property
void ???PROJECT_NAME???_NDK::set???PROJECT_FUNCTION???Property(const std::string& inputString) {
	m_pParent->getLog()->debug("set???PROJECT_FUNCTION???Property");
	???PROJECT_FUNCTION_START???Property = (int) strtoul(inputString.c_str(), NULL, 10);
}

// Asynchronous callback with JSON data input and output
void ???PROJECT_NAME???_NDK::???PROJECT_FUNCTION_START???TestAsync(const std::string& callbackId, const std::string& inputString) {
	m_pParent->getLog()->debug("Async Test");
	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		m_pParent->getLog()->error("Parse Error");
		Json::Value error;
		error["result"] = "Cannot parse JSON object";
		m_pParent->NotifyEvent(callbackId + " " + writer.write(error));
	} else {
		root["result"] = root["value1"].asInt() + root["value2"].asInt();
		m_pParent->NotifyEvent(callbackId + " " + writer.write(root));
	}
}

// Thread functions
// The following functions are for controlling a Thread in the extension

// The actual thread (must appear before the startThread method)
// Loops and runs the callback method
void* ???PROJECT_NAME???Thread(void* parent) {
	???PROJECT_NAME???_NDK *pParent = static_cast<???PROJECT_NAME???_NDK *>(parent);

	// Loop calls the callback function and continues until stop is set
	while (!pParent->isThreadHalt()) {
		sleep(1);
		pParent->???PROJECT_FUNCTION_START???ThreadCallback();
	}

	return NULL;
}

// Starts the thread and returns a message on status
std::string ???PROJECT_NAME???_NDK::???PROJECT_FUNCTION_START???StartThread(const std::string& callbackId) {
	if (!m_thread) {
		int rc;
	    rc = pthread_mutex_lock(&mutex);
	    threadHalt = false;
	    rc = pthread_cond_signal(&cond);
	    rc = pthread_mutex_unlock(&mutex);

		pthread_attr_t thread_attr;
		pthread_attr_init(&thread_attr);
		pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

		pthread_create(&m_thread, &thread_attr, ???PROJECT_NAME???Thread,
				static_cast<void *>(this));
		pthread_attr_destroy(&thread_attr);
		threadCallbackId = callbackId;
		m_pParent->getLog()->info("Thread Started");
		return "Thread Started";
	} else {
		m_pParent->getLog()->warn("Thread Started but already running");
		return "Thread Running";
	}
}

// Sets the stop value
std::string ???PROJECT_NAME???_NDK::???PROJECT_FUNCTION_START???StopThread() {
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
	m_pParent->getLog()->info("Thread Stopped");
	return "Thread stopped";
}

// The callback method that sends an event through JNEXT
void ???PROJECT_NAME???_NDK::???PROJECT_FUNCTION_START???ThreadCallback() {
	Json::FastWriter writer;
	Json::Value root;
	root["threadCount"] = ???PROJECT_FUNCTION_START???ThreadCount++;
	m_pParent->NotifyEvent(threadCallbackId + " " + writer.write(root));
}

// getter for the stop value
bool ???PROJECT_NAME???_NDK::isThreadHalt() {
	int rc;
	bool isThreadHalt;
	rc = pthread_mutex_lock(&mutex);
	isThreadHalt = threadHalt;
	rc = pthread_mutex_unlock(&mutex);
	return isThreadHalt;
}

} /* namespace webworks */
