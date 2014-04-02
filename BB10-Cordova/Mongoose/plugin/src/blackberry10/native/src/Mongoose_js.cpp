/*
 * Copyright (c) 2014 BlackBerry Limited
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
#include "Mongoose_js.hpp"
#include "Mongoose_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
MongooseJS::MongooseJS(const std::string& id) :
		m_id(id) {
	m_pLogger = new webworks::Logger("MongooseJS", this);
	m_pMongooseController = new webworks::MongooseNDK(this);
}

/**
 * MongooseJS destructor.
 */
MongooseJS::~MongooseJS() {
	if (m_pMongooseController)
		delete m_pMongooseController;
	if (m_pLogger)
		delete m_pLogger;
}

webworks::Logger* MongooseJS::getLog() {
	return m_pLogger;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "MongooseJS";
	return name;
}

/**
 * This method is used by JNext to instantiate the MongooseJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "MongooseJS") {
		return new MongooseJS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool MongooseJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string MongooseJS::InvokeMethod(const string& command) {
	// command appears with parameters following after a space
	int index = command.find_first_of(" ");
	std::string strCommand = command.substr(0, index);
	std::string arg = command.substr(index + 1, command.length());

	const char * c = strCommand.c_str();
	m_pLogger->debug(c);

	// based on the command given, run the appropriate method in Mongoose_ndk.cpp
	if (strCommand == "start") {
		return m_pMongooseController->MGstart(arg);
	} else if (strCommand == "stop") {
		return m_pMongooseController->MGstop();
	}

	strCommand.append(";");
	strCommand.append(command);
	return strCommand;
}

// Notifies JavaScript of an event
void MongooseJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}