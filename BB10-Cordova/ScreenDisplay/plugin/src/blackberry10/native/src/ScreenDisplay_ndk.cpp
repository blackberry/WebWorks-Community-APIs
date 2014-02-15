/*
 * Copyright (c) 2013 - 2014 BlackBerry Limited
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
#include <math.h>
#include <json/writer.h>
#include <json/reader.h>
#include <pthread.h>
#include "ScreenDisplay_ndk.hpp"
#include "ScreenDisplay_js.hpp"

#include <bb/device/DisplayInfo>

#define MMPERINCH 25.4;

namespace webworks {

ScreenDisplayNDK::ScreenDisplayNDK(ScreenDisplayJS *parent):
	m_pParent(parent),
	templateProperty(50),
	templateThreadCount(1),
	threadHalt(true),
	m_thread(0) {
		pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		m_pParent->getLog()->info("Template Created");
}

ScreenDisplayNDK::~ScreenDisplayNDK() {
}

std::string ScreenDisplayNDK::sdgetsize() {
	bb::device::DisplayInfo display;
	Json::FastWriter writer;
	Json::Value root;

	double physx = display.physicalSize().width();
	double physy = display.physicalSize().height();
	int pixx = display.pixelSize().width();
	int pixy = display.pixelSize().height();
	double ppmm = 0;
	double ppmmx = 0;
	double ppmmy = 0;
	double pshape = 0;

	double physdiag = 0; // Diagonal metrics
	double pixdiag = 0;

	if((pixx > 0) && (pixy > 0) && (physx > 0) && (physy > 0)) {
		ppmmx = pixx / physx;
		ppmmy = pixy / physy;
		pshape = (ppmmx / ppmmy);
		physdiag = sqrt((physx * physx) + (physy * physy));
		pixdiag = sqrt((pixx * pixx) + (pixy * pixy));
		ppmm = pixdiag / physdiag;
	}

	root["physicalWidth"] = physx;
	root["physicalHeight"] = physy;
	root["pixelWidth"] = pixx;
	root["pixelHeight"] = pixy;
	root["ppmm"] = ppmm;
	root["ppmmX"] = ppmmx;
	root["ppmmY"] = ppmmy;
	root["ppi"] = ppmm * MMPERINCH;
	root["ppiX"] = ppmmx * MMPERINCH;
	root["ppiY"] = ppmmy * MMPERINCH;
	root["pixelShape"] = pshape;

	return writer.write(root);
}

// These methods are the true native code we intend to reach from WebWorks
std::string ScreenDisplayNDK::templateTestString() {
	m_pParent->getLog()->debug("testString");
	return "Template Test Function";
}

// Take in input and return a value
std::string ScreenDisplayNDK::templateTestString(const std::string& inputString) {
	m_pParent->getLog()->debug("testStringInput");
	return "Template Test Function, got: " + inputString;
}

// Get an integer property
std::string ScreenDisplayNDK::getTemplateProperty() {
	m_pParent->getLog()->debug("get templateProperty");
	stringstream ss;
	ss << templateProperty;
	return ss.str();
}

// set an integer property
void ScreenDisplayNDK::setTemplateProperty(const std::string& inputString) {
	m_pParent->getLog()->debug("set templateProperty");
	templateProperty = (int) strtoul(inputString.c_str(), NULL, 10);
}

// Asynchronous callback with JSON data input and output
void ScreenDisplayNDK::templateTestAsync(const std::string& callbackId, const std::string& inputString) {
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
void* TemplateThread(void* parent) {
	ScreenDisplayNDK *pParent = static_cast<ScreenDisplayNDK *>(parent);

	// Loop calls the callback function and continues until stop is set
	while (!pParent->isThreadHalt()) {
		sleep(1);
		pParent->templateThreadCallback();
	}

	return NULL;
}

// Starts the thread and returns a message on status
std::string ScreenDisplayNDK::templateStartThread(const std::string& callbackId) {
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
		threadCallbackId = callbackId;
		m_pParent->getLog()->info("Thread Started");
		return "Thread Started";
	} else {
		m_pParent->getLog()->warn("Thread Started but already running");
		return "Thread Running";
	}
}

// Sets the stop value
std::string ScreenDisplayNDK::templateStopThread() {
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
void ScreenDisplayNDK::templateThreadCallback() {
	Json::FastWriter writer;
	Json::Value root;
	root["threadCount"] = templateThreadCount++;
	m_pParent->NotifyEvent(threadCallbackId + " " + writer.write(root));
}

// getter for the stop value
bool ScreenDisplayNDK::isThreadHalt() {
	int rc;
	bool isThreadHalt;
	rc = pthread_mutex_lock(&mutex);
	isThreadHalt = threadHalt;
	rc = pthread_mutex_unlock(&mutex);
	return isThreadHalt;
}

} /* namespace webworks */
