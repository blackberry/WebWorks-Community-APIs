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

#include <sstream>
#include <QMetaObject>
#include <QMetaEnum>
#include <QObject>
#include <bb/platform/NotificationError>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include "mediaKeys_ndk.hpp"
#include "mediaKeys_js.hpp"
#include "ApplicationThread.hpp"

#include "Logger.hpp"
#include <QCoreApplication>
#include <QDebug>

namespace webworks {

    using namespace bb::platform;
    using namespace bb::system;
    using namespace std;


     void MediaKeysNDK::join( string inputString) {
        QByteArray byteArray(inputString.c_str(), inputString.length());
        m_pParent->applicationThread()->join(byteArray);
     }


    void MediaKeysNDK::cleanup() {
            qDeleteAll(m_dialogHandlerList.values());
            deleteLater();
    }

    void MediaKeysNDK::sendEvent( const std::string& msg) {
        m_pParent->NotifyEvent(msg);
    }

    string MediaKeysNDK::create(string callbackId, string inputString) {

        Json::Reader reader;
        Json::Value root, value;
        QString context;
        unsigned int i, len;

        bool parse = reader.parse(inputString, root);

        if (!parse) {
            return "parse error";
        }

        NotificationDialog * dialog = new NotificationDialog();

        // message
        value = root["message"];
        if (!value.empty()) {
            context = QString::fromStdString(value.asString());
            dialog->setBody(context);
        }

        // button list
        value = root["buttons"];
        if (!value.empty()) {
            len = value.size();
            for (i = 0; i < len; ++i) {
                context = QString::fromStdString(value[i].asString());
                // dialog will delete its buttons
                SystemUiButton * button = new SystemUiButton(context);
                dialog->appendButton(button);
            }
        }

        // title
        value = root["settings"]["title"];
        if (!value.empty()) {
            context = QString::fromStdString(value.asString());
            dialog->setTitle(context);
        }

        // repeat
        value = root["settings"]["repeat"];
        if (value.asBool()) {
            dialog->setRepeat(true);
        }

        // key passed by reference
        int key = ++m_id;
        DialogHandler * dialogHandler = new DialogHandler( this, callbackId, dialog);
        m_dialogHandlerList.insert(key, dialogHandler);

        bool success = QObject::connect(dialog,
                        SIGNAL(finished(bb::platform::NotificationResult::Type)),
                        dialogHandler,
                        SLOT(onDialogFinished(bb::platform::NotificationResult::Type)));

        if (!success) {
            // this is not normal in most cases
            return "qt connect fail";
        }

        stringstream ss;
        ss << key;
        string retId = ss.str();

        return retId;
    }


    string DialogHandler::getError(){
        const QMetaObject mObj = NotificationError::staticMetaObject;
        NotificationError::Type nError = m_notificationDialog->error();
        int index = mObj.indexOfEnumerator("Type");
        QMetaEnum mEnum = mObj.enumerator(index);
        const char * errorType = (mEnum.valueToKey(nError));

        return string(errorType);
    }


    string DialogHandler::show() {
        m_notificationDialog->show();
        if (m_notificationDialog->result() == NotificationResult::Error) {
            return getError();
        }
        return "";
    }

    string MediaKeysNDK::show(string id){

        int key;
        stringstream ss( id );
        ss >> dec >> key;

        DialogHandler *dialogHandler = m_dialogHandlerList.value(key, NULL);

        if (NULL != dialogHandler) {
            return dialogHandler->show();
        } else {
            return "dialog empty";
        }
    }

    // this doesn't work yet
    // just returns a string
    string MediaKeysNDK::bind(string callbackId, string inputString) {
        this->m_pParent->getLog()->debug("Binding keyWatcher");
        this->m_pParent->getLog()->debug(inputString.c_str());

        Json::Reader reader;
        Json::Value root, value;
        string mediaKey;

        bool parse = reader.parse(inputString, root);

        if (parse == false) {
            return "parse error";
        }

        value = root["mediakey"];
        if (value.empty()) {
            return "no mediakey defined";
        }

        mediaKey = value.asString();

        MediaKeyWatcher *mediaKeyWatcher = NULL;
        //MediaKeysHandler *mediaKeysHandler = NULL;

        if (mediaKey == "volumeUp") {
            mediaKeyWatcher = new MediaKeyWatcher(MediaKey::VolumeUp);
            //mediaKeysHandler = new MediaKeysHandler(this, callbackId, mediaKeyWatcher);
        } else if (mediaKey == "volumeDown") {
            mediaKeyWatcher = new MediaKeyWatcher(MediaKey::VolumeDown);
            //mediaKeysHandler = new MediaKeysHandler(this, callbackId, mediaKeyWatcher);
        }

        //int key = this->m_id++;
        //this->m_mediaKeyHandlerList.insert(key, mediaKeysHandler);

        // just hacking it
        this->success = true;
        this->keyWatcher = mediaKeyWatcher;

        bool success = QObject::connect(
                this->keyWatcher, SIGNAL(shortPress(bb::multimedia::MediaKey::Type)),
                this, SLOT(onShortPress(bb::multimedia::MediaKey::Type)));
/*
        bool success = QObject::connect(
                mediaKeyWatcher, SIGNAL(shortPress(bb::multimedia::MediaKey::Type)),
                mediaKeysHandler, SLOT(onShortPress(bb::multimedia::MediaKey::Type)));
*/
        /*
        stringstream ss;
        ss << key;
        string retId = ss.str();
        */

        return (success == true ? "binded" : "not binded");
    }

    string MediaKeysNDK::checkVolume(string arg) {
        if (this->success == false) {
            this->m_pParent->getLog()->debug("Setting up KeyWatcher");
            this->keyWatcher = new MediaKeyWatcher(MediaKey::VolumeUp);

            this->success = QObject::connect(
                    this->keyWatcher, SIGNAL(shortPress(bb::multimedia::MediaKey::Type)),
                    this, SLOT(onShortPress(bb::multimedia::MediaKey::Type)));

            this->m_pParent->getLog()->debug(this->success ? "CONNECTION SUCCESSFUL" : "CONNECTION FAILURE");

            this->changed = false;
        }

        this->m_pParent->getLog()->debug("Checking Volume");

        if (this->changed == true) {
            this->m_pParent->getLog()->debug("changed is TRUE");
            this->changed = false;

            return "Volume IS changed";
        }

        return "Volume IS NOT changed";
    }

    void MediaKeysNDK::onShortPress(bb::multimedia::MediaKey::Type key) {
        this->m_pParent->getLog()->debug("Volume SLOT is pressed");

        this->changed = true;
    }

// MediaKeyHandler Functions
    void MediaKeysHandler::onPress(bb::multimedia::MediaKey::Type key) {
        this->m_parentNDK->getParent()->getLog()->debug("Volume SLOT is pressed");
        this->m_parentNDK->changed = true;
    }

    void DialogHandler::onDialogFinished(bb::platform::NotificationResult::Type value) {

        Json::FastWriter writer;
        Json::Value root;

        switch(value) {

            case NotificationResult::None:
                // shouldn't happen
                root["result"] = "None";
                root["error"] = "None";
                break;

            case NotificationResult::Error:
                root["result"] = "Error";
                root["error"] = getError();
                break;

            case NotificationResult::ButtonSelection:
            {
                // get selected button index
                root["result"] = "ButtonSelection";
                SystemUiButton * select = m_notificationDialog->buttonSelection();
                if (NULL != select) {
                    const int size = m_notificationDialog->buttonCount();
                    for (int i = 0; i < size; ++i ) {
                        if (m_notificationDialog->buttonAt(i) == select ) {
                            root["select"] = i;
                            break;
                        }
                    }
                }
                break;
            }

            default:
                root["result"] = "Error";
                root["error"] = "unknown NotificationResult Type";
                break;
        } // switch

        // callback, notify js object
        m_parentNDK->sendEvent(m_callbackId + " " + writer.write(root));

        // cleanup, buttons will be deleted
        m_notificationDialog->clearButtons();
        m_notificationDialog->deleteLater();
    }

} /* namespace webworks */
