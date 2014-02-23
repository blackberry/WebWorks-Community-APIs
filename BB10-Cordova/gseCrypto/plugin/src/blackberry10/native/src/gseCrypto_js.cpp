/*
 * Copyright 2013-2014 Research In Motion Limited.
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
#include "gseCrypto_js.hpp"
#include "gseCrypto_ndk.hpp"
#include "json/writer.h"

using namespace std;

/**
 * Default constructor.
 */
GSECryptoJS::GSECryptoJS(const std::string& id) :
		m_id(id) {
	gseCryptoController = new gsecrypto::GSECryptoNDK(this);
}

/**
 * GSECryptoJS destructor.
 */
GSECryptoJS::~GSECryptoJS() {
	if (gseCryptoController)
		delete gseCryptoController;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "GSECryptoJS";
	return name;
}

/**
 * This method is used by JNext to instantiate the GSECryptoJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "GSECryptoJS") {
		return new GSECryptoJS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool GSECryptoJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string GSECryptoJS::InvokeMethod(const string& command) {
	// command appears with parameters following after a space
	size_t commandIndex = command.find_first_of(" ");
	std::string strCommand = command.substr(0, commandIndex);
	size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
	std::string callbackId = command.substr(commandIndex + 1, callbackIndex - commandIndex - 1);
	std::string arg = command.substr(callbackIndex + 1, command.length());

	if (strCommand == "hash") {
		return gseCryptoController->hash(arg);
	} else if (strCommand == "generateKey") {
		//return gseCryptoController->generateKey(arg);
	} else if (strCommand == "encrypt") {
		//return gseCryptoController->encrypt(arg);
	} else if (strCommand == "decrypt") {
		//return gseCryptoController->decrypt(arg);
	} else if (strCommand == "random") {
		//return gseCryptoController->random(arg);
	}

	Json::Value error;
	error["error"] = "No implementation found for " + strCommand + "(" + arg + ")";
	Json::FastWriter writer;
	return writer.write(error);
}

// Notifies JavaScript of an event
void GSECryptoJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
