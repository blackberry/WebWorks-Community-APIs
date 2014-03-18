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

#include <QDir>
#include <QString>

#include <qdebug.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#include <string>
#include "../public/tokenizer.h"
#include "sms_js.hpp"
#include "sms_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
SMS_JS::SMS_JS(const std::string& id) :
		m_id(id) {
	m_pTemplateController = new webworks::SMS_NDK(this);
}

/**
 * SMS_JS destructor.
 */
SMS_JS::~SMS_JS() {
	if (m_pTemplateController)
		delete m_pTemplateController;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "SMS_JS";
	return name;
}

/**
 * This method is used by JNext to instantiate the SMS_JS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "SMS_JS") {
		return new SMS_JS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool SMS_JS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string SMS_JS::InvokeMethod(const string& command) {
	// format must be: "command callbackId params"
	size_t commandIndex = command.find_first_of(" ");
	std::string strCommand = command.substr(0, commandIndex);
	size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
	std::string callbackId = command.substr(commandIndex + 1, callbackIndex - commandIndex - 1);
	std::string arg = command.substr(callbackIndex + 1, command.length());

	// based on the command given, run the appropriate method in SMS_ndk.cpp
	if (strCommand == "ping") {
		return m_pTemplateController->ping();
	} else if (strCommand == "udSMS_Sync") {
			return m_pTemplateController->udSMS_Sync(arg);
	} else if (strCommand == "udSMS_Async") { //
		m_pTemplateController->udSMS_Async(callbackId, arg);
	}

	strCommand.append(";");
	strCommand.append(command);
	return strCommand;
}

// Notifies JavaScript of an event
void SMS_JS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
