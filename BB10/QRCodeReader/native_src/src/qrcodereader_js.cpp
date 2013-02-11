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

#include "../public/tokenizer.h"
#include "qrcodereader_js.hpp"
#include "qrcodereader_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
QRCodeReaderJS::QRCodeReaderJS(const std::string& id) :
		m_id(id) {
	m_pQRCodeReaderController = new webworks::QRCodeReaderNDK(this);
}

/**
 * VibrationJS destructor.
 */
QRCodeReaderJS::~QRCodeReaderJS() {
	if (m_pQRCodeReaderController)
		delete m_pQRCodeReaderController;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "QRCodeReaderJS";
	return name;
}

/**
 * This method is used by JNext to instantiate the QRCodeReaderJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "QRCodeReaderJS") {
		return new QRCodeReaderJS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool QRCodeReaderJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string QRCodeReaderJS::InvokeMethod(const string& command) {
	// command appears with parameters following after a space
	int index = command.find_first_of(" ");
	std::string strCommand = command.substr(0, index);
	std::string arg = command.substr(index + 1, command.length());

	if (strCommand == "QRCodeReaderStart") {
		m_pQRCodeReaderController->startQRCodeRead();
	}
	else if (strCommand == "QRCodeReaderStop") {
		m_pQRCodeReaderController->stopQRCodeRead();
	}

	strCommand.append(";");
		strCommand.append(command);
		return strCommand;
}

// Notifies JavaScript of an event
void QRCodeReaderJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
