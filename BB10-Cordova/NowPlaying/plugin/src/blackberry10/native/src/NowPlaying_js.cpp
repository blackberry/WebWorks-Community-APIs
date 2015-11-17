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
#include "NowPlaying_js.hpp"

using namespace std;

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
    static char name[] = "NowPlayingJS";
    return name;
}

/**
 * This method is used by JNext to instantiate the TemplateJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "NowPlayingJS") {
        return new NowPlayingJS(id);

    }

    return NULL;
}

NowPlayingJS::NowPlayingJS(const string& id): m_id(id) {
    m_thread = new bb::webworks::extensions::ApplicationThread(QThread::LowPriority);
    m_NowPlayingMgr = new webworks::NowPlayingNDK(this);
    m_NowPlayingMgr->moveToThread(m_thread);
}

NowPlayingJS::~NowPlayingJS(){
    // delete m_NowPlayingMgr;
    m_thread->wait(50);
    delete m_thread;
}

/**
 * Notifies JavaScript of an event
 */
void NowPlayingJS::NotifyEvent(const string& event) {
    string eventString = m_id + " ";
    eventString.append(event);
    SendPluginEvent(eventString.c_str(), m_pContext);
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string NowPlayingJS::InvokeMethod(const string& command) {
    // Format assumed to be "command", "command callbackId", or
    // "command callbackId params"
    size_t commandIndex = command.find_first_of(" ");
    string strCommand = command.substr(0, commandIndex);

    size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
    string callbackId = command.substr(commandIndex + 1, callbackIndex - (commandIndex + 1));
    string arg = command.substr(callbackIndex + 1, command.length());

    bool success = false;
    string result = "";

    if (strCommand == "joinSlot") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "joinSlot",
                Qt::BlockingQueuedConnection,
                Q_ARG(const string&, arg));

    } else if (strCommand == "NowPlayingRequestPlayback") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingRequestPlayback",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result));

    } else if (strCommand == "NowPlayingBindPlayCallback") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingBindPlayCallback",
                Qt::BlockingQueuedConnection,
                Q_ARG(const string&, callbackId));

    } else if (strCommand == "NowPlayingBindPauseCallback") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingBindPauseCallback",
                Qt::BlockingQueuedConnection,
                Q_ARG(const string&, callbackId));

    } else if (strCommand == "NowPlayingBindStopCallback") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingBindStopCallback",
                Qt::BlockingQueuedConnection,
                Q_ARG(const string&, callbackId));

    } else if (strCommand == "NowPlayingBindNextCallback") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingBindNextCallback",
                Qt::BlockingQueuedConnection,
                Q_ARG(const string&, callbackId));

    } else if (strCommand == "NowPlayingBindPreviousCallback") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingBindPreviousCallback",
                Qt::BlockingQueuedConnection,
                Q_ARG(const string&, callbackId));

    } else if (strCommand == "NowPlayingBindErrorCallback") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingBindErrorCallback",
                Qt::BlockingQueuedConnection,
                Q_ARG(const string&, callbackId));

    } else if (strCommand == "NowPlayingPlay") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingPlay",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result),
                Q_ARG(const string&, arg));

    } else if (strCommand == "NowPlayingPause") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingPause",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result));

    } else if (strCommand == "NowPlayingResume") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingResume",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result));

    } else if (strCommand == "NowPlayingStop") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingStop",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result));

    } else if (strCommand == "NowPlayingNext") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingNext",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result));

    } else if (strCommand == "NowPlayingPrevious") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingPrevious",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result));

    } else if (strCommand == "NowPlayingError") {
        /*
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingPlay",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result),
                Q_ARG(const string&, arg));

        */

        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingError",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result),
                Q_ARG(const string&, arg));

    } else if (strCommand == "NowPlayingGetState") {
        success = QMetaObject::invokeMethod(m_NowPlayingMgr,
                "NowPlayingGetState",
                Qt::BlockingQueuedConnection,
                Q_RETURN_ARG(string, result));

    } else {
        result = "Invalid Method " + strCommand;
    }

    return (success ? "SUCCESS: " : "FAILURE: ") + result;
}




