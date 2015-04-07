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

NowPlayingJS::NowPlayingJS(const std::string& id): m_id(id) {
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
    if(className == "NowPlayingJS"){
        return new NowPlayingJS(id);

    }

    return NULL;
}

bool NowPlayingJS::CanDelete(){
    return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string NowPlayingJS::InvokeMethod(const string& command) {
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


    if(strCommand=="NowPlayingConnectionTest"){
        result = m_NowPlayingMgr->NowPlayingConnectionTest();
        return result;
    }else if(strCommand=="testAsync"){
        m_NowPlayingMgr->testAsync(callbackId,arg);
        return "";
    }else if(strCommand=="NowPlayingStop"){
        result = m_NowPlayingMgr->NowPlayingStop();
        return result;
    }else if(strCommand=="NowPlayingPlay"){
        result = m_NowPlayingMgr->NowPlayingPlay();
        return result;
    }else if(strCommand=="NowPlayingPause"){
        result = m_NowPlayingMgr->NowPlayingPause();
        return result;
    }else if(strCommand=="NowPlayingSwitchMusic"){
        result = m_NowPlayingMgr->NowPlayingSwitchMusic(arg);
        return result;
    }else if(strCommand=="NowPlayingGetSourceAddress"){
        result = m_NowPlayingMgr->NowPlayingGetSourceAddress();
        return result;
    }else if(strCommand=="NowPlayingGetDuration"){
        result = m_NowPlayingMgr->NowPlayingGetDuration();
        return result;
    }else if(strCommand=="NowPlayingGetPosition"){
        result = m_NowPlayingMgr->NowPlayingGetPosition();
        return result;
    }else{
        result = "Invalid Method " + strCommand;
    }

    return result;
}




// Notifies JavaScript of an event
void NowPlayingJS::NotifyEvent(const std::string& event) {
    std::string eventString = m_id + " ";
    eventString.append(event);
    SendPluginEvent(eventString.c_str(), m_pContext);
}


