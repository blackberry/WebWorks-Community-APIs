/*
 * Copyright (c) 2013 BlackBerry Limited.
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

#include <iostream>
#include <sstream>
#include <QMetaObject>
#include "mediaKeys_js.hpp"

using namespace std;

/**
 * constructor.
 */
MediaKeysJS::MediaKeysJS(const std::string& id) : m_id(id) {
	m_thread = new bb::webworks::extensions::ApplicationThread(QThread::LowPriority);
	m_sysDialogMgr = new webworks::MediaKeysNDK(this);
	m_pLogger = new webworks::Logger("MediaKeysJS", this);
	m_sysDialogMgr->moveToThread(m_thread);
}

/**
 * destructor.
 */
MediaKeysJS::~MediaKeysJS() {
	QMetaObject::invokeMethod(m_sysDialogMgr, "cleanup", Qt::BlockingQueuedConnection);
	m_thread->wait(50);

	delete m_thread;
	delete m_pLogger;
}

webworks::Logger* MediaKeysJS::getLog() {
    return m_pLogger;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "MediaKeysJS";
	return name;
}

/**
 * This method is used by JNext to instantiate the TemplateJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "MediaKeysJS") {
        return new MediaKeysJS(id);
    }

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool MediaKeysJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string MediaKeysJS::InvokeMethod(const string& command) {
	// format must be: "command callbackId params"
    size_t commandIndex = command.find_first_of(" ");
	std::string strCommand = command.substr(0, commandIndex);
	size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
	std::string callbackId = command.substr(commandIndex + 1, callbackIndex - commandIndex - 1);
	std::string arg = command.substr(callbackIndex + 1, command.length());

	// After call finished, result can be inited value (call fail), method returned error or
	// noError return value. hasError = (result.find_first_of(" ")!=string::npos)
	string result = "Fail to invoke method " + strCommand;
	bool success = false;

	// based on the command given, run the appropriate method in sysDialog_ndk.cpp
	if (strCommand == "create") {

		success = QMetaObject::invokeMethod(m_sysDialogMgr, "create", Qt::BlockingQueuedConnection,
				Q_RETURN_ARG(string, result), Q_ARG(string, callbackId), Q_ARG(string, arg));

	} else if (strCommand == "show") {

		success = QMetaObject::invokeMethod(m_sysDialogMgr, "show", Qt::BlockingQueuedConnection,
				Q_RETURN_ARG(string, result), Q_ARG(string, arg));

	} else if (command == "join") {

		success = QMetaObject::invokeMethod(m_sysDialogMgr, "join", Qt::BlockingQueuedConnection,
				Q_ARG(string, arg));

		if ( success) {
			// join() no return, reset
			result = "";
		}

	} else if (command == "checkVolume") {
		//result = m_sysDialogMgr->checkVolume();
        success = QMetaObject::invokeMethod(m_sysDialogMgr, "checkVolume", Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result), Q_ARG(string, arg));


	} else {
		result = "Invalid Method " + strCommand;
	}

	return result;
}

// Notifies JavaScript of an event
void MediaKeysJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
