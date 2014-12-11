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
#include "simplebtsppplugin_js.hpp"
#include "simplebtsppplugin_ndk.hpp"

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
SimpleBtSppPlugin_JS::SimpleBtSppPlugin_JS(const std::string& id) :
		m_id(id) {
	m_pLogger = new webworks::Logger("SimpleBtSppPlugin_JS", this);
	m_pSimpleBtSppPluginController = new webworks::SimpleBtSppPlugin_NDK(this);
}

/**
 * SimpleBtSppPlugin_JS destructor.
 */
SimpleBtSppPlugin_JS::~SimpleBtSppPlugin_JS() {
	if (m_pSimpleBtSppPluginController)
		delete m_pSimpleBtSppPluginController;
	if (m_pLogger)
		delete m_pLogger;
}

webworks::Logger* SimpleBtSppPlugin_JS::getLog() {
	return m_pLogger;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "SimpleBtSppPlugin_JS";
	return name;
}

/**
 * This method is used by JNext to instantiate the SimpleBtSppPlugin_JS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "SimpleBtSppPlugin_JS") {
		return new SimpleBtSppPlugin_JS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool SimpleBtSppPlugin_JS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string SimpleBtSppPlugin_JS::InvokeMethod(const string& command) {
	// format must be: "command callbackId params"
	size_t commandIndex = command.find_first_of(" ");
	std::string strCommand = command.substr(0, commandIndex);
	size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
	std::string callbackId = command.substr(commandIndex + 1, callbackIndex - commandIndex - 1);
	std::string arg = command.substr(callbackIndex + 1, command.length());

	// based on the command given, run the appropriate method in simplebtsppplugin_ndk.cpp
    if (strCommand == "initialiseBluetooth") {
        return m_pSimpleBtSppPluginController->initialiseBluetooth();

    } else if (strCommand == "terminateBluetooth") {
        return m_pSimpleBtSppPluginController->terminateBluetooth();

    } else if (strCommand == "bluetoothAddress") {
        if (arg != strCommand) {
            m_pSimpleBtSppPluginController->setBluetoothAddress(arg);
        } else {
            return m_pSimpleBtSppPluginController->getBluetoothAddress();
        }
    } else if (strCommand == "startDeviceScan") {
        return m_pSimpleBtSppPluginController->startDeviceScan(callbackId);

    } else if (strCommand == "connectToDevice") {
        return m_pSimpleBtSppPluginController->connectToDevice(callbackId);

    } else if (strCommand == "disconnectFromDevice") {
        return m_pSimpleBtSppPluginController->disconnectFromDevice();

    } else if (strCommand == "sendMessage") {
        return m_pSimpleBtSppPluginController->sendMessage(arg);

    } else if (strCommand == "listenForConnection") {
        return m_pSimpleBtSppPluginController->listenForConnection(callbackId);

    } else if (strCommand == "stopListening") {
        return m_pSimpleBtSppPluginController->stopListening();

    } else if (strCommand == "pluginVersion") {
        return m_pSimpleBtSppPluginController->pluginVersion();

    } else if (strCommand == "sppServiceUuid") {
        if (arg != strCommand) {
            m_pSimpleBtSppPluginController->setSppServiceUuid(arg);
        } else {
            return m_pSimpleBtSppPluginController->getSppServiceUuid();
        }
    } else if (strCommand == "sppRfcommMode") {
        if (arg != strCommand) {
            m_pSimpleBtSppPluginController->setRfcommMode(((arg == "true") ? true : false));
        } else {
            return (m_pSimpleBtSppPluginController->getRfcommMode() ? "true" : "false");
        }
    } else if (strCommand == "sppRfcommServicePort") {
        if (arg != strCommand) {
            m_pSimpleBtSppPluginController->setRfcommServicePort((int)std::strtol(arg.c_str(), NULL, 10));
        } else {
            return SSTR(m_pSimpleBtSppPluginController->getRfcommServicePort());
        }
    } else if (strCommand == "sppBufferSize") {
        if (arg != strCommand) {
            m_pSimpleBtSppPluginController->setSppBufferSize((uint)std::strtol(arg.c_str(), NULL, 10));
        } else {
            return SSTR(m_pSimpleBtSppPluginController->getSppBufferSize());
        }
    }

	strCommand.append(";");
	strCommand.append(command);
	return strCommand;
}

// Notifies JavaScript of an event
void SimpleBtSppPlugin_JS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
