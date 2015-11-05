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

// Notifies JavaScript of an event
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

    string result = "";

    if (strCommand == "NowPlayingRequestPlayback") {
        result = m_NowPlayingMgr->NowPlayingRequestPlayback();

    } else if (strCommand == "NowPlayingBindPlayCallback") {
        m_NowPlayingMgr->NowPlayingBindPlayCallback(callbackId);

    } else if (strCommand == "NowPlayingBindPauseCallback") {
        m_NowPlayingMgr->NowPlayingBindPauseCallback(callbackId);

    } else if (strCommand == "NowPlayingBindStopCallback") {
        m_NowPlayingMgr->NowPlayingBindStopCallback(callbackId);

    } else if (strCommand == "NowPlayingBindNextCallback") {
        m_NowPlayingMgr->NowPlayingBindNextCallback(callbackId);

    } else if (strCommand == "NowPlayingBindPreviousCallback") {
        m_NowPlayingMgr->NowPlayingBindPreviousCallback(callbackId);

    } else if (strCommand == "NowPlayingBindErrorCallback") {
        m_NowPlayingMgr->NowPlayingBindErrorCallback(callbackId);

    } else if (strCommand == "NowPlayingPlay") {
        result = m_NowPlayingMgr->NowPlayingPlay(arg);

    } else if (strCommand == "NowPlayingPause") {
        result = m_NowPlayingMgr->NowPlayingPause();

    } else if (strCommand == "NowPlayingResume") {
        result = m_NowPlayingMgr->NowPlayingResume();

    } else if (strCommand == "NowPlayingStop") {
        result = m_NowPlayingMgr->NowPlayingStop();

    } else if (strCommand == "NowPlayingNext") {
        result = m_NowPlayingMgr->NowPlayingNext();

    } else if (strCommand == "NowPlayingPrevious") {
        result = m_NowPlayingMgr->NowPlayingPrevious();

    } else if (strCommand == "NowPlayingError") {
        result = m_NowPlayingMgr->NowPlayingError();

    } else if (strCommand == "NowPlayingGetState") {
        result = m_NowPlayingMgr->NowPlayingGetState();

    } else {
        result = "Invalid Method " + strCommand;
    }

    return result;
}




