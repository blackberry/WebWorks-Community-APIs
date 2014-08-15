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
#include "../public/tokenizer.h"
#include "???PROJECT_NAME_LOWERCASE???_js.hpp"
#include "???PROJECT_NAME_LOWERCASE???_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
???PROJECT_NAME???_JS::???PROJECT_NAME???_JS(const std::string& id) :
		m_id(id) {
	m_pLogger = new webworks::Logger("???PROJECT_NAME???_JS", this);
	m_p???PROJECT_FUNCTION???Controller = new webworks::???PROJECT_NAME???_NDK(this);
}

/**
 * ???PROJECT_NAME???_JS destructor.
 */
???PROJECT_NAME???_JS::~???PROJECT_NAME???_JS() {
	if (m_p???PROJECT_FUNCTION???Controller)
		delete m_p???PROJECT_FUNCTION???Controller;
	if (m_pLogger)
		delete m_pLogger;
}

webworks::Logger* ???PROJECT_NAME???_JS::getLog() {
	return m_pLogger;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "???PROJECT_NAME???_JS";
	return name;
}

/**
 * This method is used by JNext to instantiate the ???PROJECT_NAME???_JS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "???PROJECT_NAME???_JS") {
		return new ???PROJECT_NAME???_JS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool ???PROJECT_NAME???_JS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string ???PROJECT_NAME???_JS::InvokeMethod(const string& command) {
	// format must be: "command callbackId params"
	size_t commandIndex = command.find_first_of(" ");
	std::string strCommand = command.substr(0, commandIndex);
	size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
	std::string callbackId = command.substr(commandIndex + 1, callbackIndex - commandIndex - 1);
	std::string arg = command.substr(callbackIndex + 1, command.length());

	// based on the command given, run the appropriate method in ???PROJECT_NAME_LOWERCASE???_ndk.cpp
	if (strCommand == "???PROJECT_FUNCTION_START???Test") {
		return m_p???PROJECT_FUNCTION???Controller->???PROJECT_FUNCTION_START???Test();
	} else if (strCommand == "???PROJECT_FUNCTION_START???TestInput") {
		return m_p???PROJECT_FUNCTION???Controller->???PROJECT_FUNCTION_START???Test(arg);
	} else if (strCommand == "???PROJECT_FUNCTION_START???Property") {
		// if arg exists we are setting property
		if (arg != strCommand) {
			m_p???PROJECT_FUNCTION???Controller->set???PROJECT_FUNCTION???Property(arg);
		} else {
			return m_p???PROJECT_FUNCTION???Controller->get???PROJECT_FUNCTION???Property();
		}
	} else if (strCommand == "???PROJECT_FUNCTION_START???TestAsync") {
		m_p???PROJECT_FUNCTION???Controller->???PROJECT_FUNCTION_START???TestAsync(callbackId, arg);
	} else if (strCommand == "???PROJECT_FUNCTION_START???StartThread") {
		return m_p???PROJECT_FUNCTION???Controller->???PROJECT_FUNCTION_START???StartThread(callbackId);
	} else if (strCommand == "???PROJECT_FUNCTION_START???StopThread") {
		return m_p???PROJECT_FUNCTION???Controller->???PROJECT_FUNCTION_START???StopThread();
	}

	strCommand.append(";");
	strCommand.append(command);
	return strCommand;
}

// Notifies JavaScript of an event
void ???PROJECT_NAME???_JS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
