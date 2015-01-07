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
#include "simplebtlehrplugin_js.hpp"
#include "simplebtlehrplugin_ndk.hpp"

#ifdef LOGD
#undef LOGD
#define LOGD(...) \
    do { \
        char __temp[500]; \
        sprintf(__temp, "XXXX " __VA_ARGS__); \
        m_pLogger->debug(__temp); \
    } while (0)
#endif

using namespace std;

/**
 * Default constructor.
 */
SimpleBtLeHrPlugin_JS::SimpleBtLeHrPlugin_JS(const std::string& id) :
		m_id(id) {
	m_pLogger = new webworks::Logger("SimpleBtLeHrPlugin_JS", this);
	m_pSimpleBtLeHrPluginController = new webworks::SimpleBtLeHrPlugin_NDK(this);
}

/**
 * SimpleBtLeHrPlugin_JS destructor.
 */
SimpleBtLeHrPlugin_JS::~SimpleBtLeHrPlugin_JS() {
	if (m_pSimpleBtLeHrPluginController)
		delete m_pSimpleBtLeHrPluginController;
	if (m_pLogger)
		delete m_pLogger;
}

webworks::Logger* SimpleBtLeHrPlugin_JS::getLog() {
	return m_pLogger;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "SimpleBtLeHrPlugin_JS";
	return name;
}

/**
 * This method is used by JNext to instantiate the SimpleBtLeHrPlugin_JS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "SimpleBtLeHrPlugin_JS") {
		return new SimpleBtLeHrPlugin_JS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool SimpleBtLeHrPlugin_JS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string SimpleBtLeHrPlugin_JS::InvokeMethod(const string& command) {
	// format must be: "command callbackId params"
	size_t commandIndex = command.find_first_of(" ");
	std::string strCommand = command.substr(0, commandIndex);
	size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
	std::string callbackId = command.substr(commandIndex + 1, callbackIndex - commandIndex - 1);
	std::string arg = command.substr(callbackIndex + 1, command.length());

	// based on the command given, run the appropriate method in simplebtlehrplugin_ndk.cpp
    if (strCommand == "initialiseBluetooth") {
        return m_pSimpleBtLeHrPluginController->initialiseBluetooth();

    } else if (strCommand == "terminateBluetooth") {
        return m_pSimpleBtLeHrPluginController->terminateBluetooth();

    } else if (strCommand == "bluetoothAddress") {
        if (arg != strCommand) {
            m_pSimpleBtLeHrPluginController->setBluetoothAddress(arg);
        } else {
            return m_pSimpleBtLeHrPluginController->getBluetoothAddress();
        }
    } else if (strCommand == "startDeviceScan") {
        return m_pSimpleBtLeHrPluginController->startDeviceScan(callbackId);

    } else if (strCommand == "pluginVersion") {
        return m_pSimpleBtLeHrPluginController->pluginVersion();

    } else if (strCommand == "startMonitoringHr") {
        return m_pSimpleBtLeHrPluginController->startMonitoringHr(callbackId);

    } else if (strCommand == "stopMonitoringHr") {
        return m_pSimpleBtLeHrPluginController->stopMonitoringHr();

    }

	strCommand.append(";");
	strCommand.append(command);
	return strCommand;
}

// Notifies JavaScript of an event
void SimpleBtLeHrPlugin_JS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
