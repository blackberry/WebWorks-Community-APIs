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
#include "barcodereader_js.hpp"
#include "barcodereader_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
BarcodeReaderJS::BarcodeReaderJS(const std::string& id) :
        m_id(id) {
    m_pBarcodeReaderController = new webworks::BarcodeReaderNDK(this);
}

/**
 * BarcodeReaderJS destructor.
 */
BarcodeReaderJS::~BarcodeReaderJS() {
    if (m_pBarcodeReaderController)
        delete m_pBarcodeReaderController;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
    static char name[] = "BarcodeReaderJS";
    return name;
}

/**
 * This method is used by JNext to instantiate the BarcodeReaderJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "BarcodeReaderJS") {
        return new BarcodeReaderJS(id);
    }

    return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool BarcodeReaderJS::CanDelete() {
    return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string BarcodeReaderJS::InvokeMethod(const string& command) {
    // command appears with parameters following after a space
    int index = command.find_first_of(" ");
    std::string strCommand = command.substr(0, index);
    std::string arg = command.substr(index + 1, command.length());

    if (strCommand == "startRead") {
        m_pBarcodeReaderController->startRead();
    }
    else if (strCommand == "stopRead") {
        m_pBarcodeReaderController->stopRead();
    }

    strCommand.append(";");
    strCommand.append(command);
    return strCommand;
}

// Notifies JavaScript of an event
void BarcodeReaderJS::NotifyEvent(const std::string& event) {
    std::string eventString = m_id + " ";
    eventString.append(event);
    SendPluginEvent(eventString.c_str(), m_pContext);
}
