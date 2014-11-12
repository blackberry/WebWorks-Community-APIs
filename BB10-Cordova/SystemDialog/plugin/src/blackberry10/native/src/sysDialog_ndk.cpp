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

#include <json/reader.h>
#include <json/value.h>
//#include <algorithm>
#include <json/writer.h>
#include <QMetaObject>

#include <map>
#include <sstream>
#include "sysDialog_ndk.hpp"
#include "sysDialog_js.hpp"
#include <QObject>
#include <bb/platform/NotificationError>

// TODO: root["error"] = NotificationError works?
// TODO: m_notificationDialog->clearButtons(); works?


namespace webworks {

    using namespace bb::platform;
    using namespace bb::system;
    using namespace std;
    map<NotificationError::Type, string> SysDialogNDK::m_errorMessage;

    SysDialogNDK::~SysDialogNDK() {
        // unsigned int i, len;
        // len = m_buttonList.size();
        // for (i = 0; i < len; ++i) {
        //     delete m_buttonList[i]
        // }
        m_notificationDialog->clearButtons();
        // The button instances will be deleted.
        delete[] m_buttonList;
    };

    // enum NotificationError::Type cannot be safely converted to int
    // mapping type to string
    void SysDialogNDK::initErrorMsg() {
        if (m_errorMessage.count(NotificationError::None) == 0) {
            m_errorMessage[NotificationError::None] = "None";
            m_errorMessage[NotificationError::Unknown] = "Unknown";
            m_errorMessage[NotificationError::InvalidRequest] = "InvalidRequest";
            m_errorMessage[NotificationError::NoButtons] = "NoButtons";
            m_errorMessage[NotificationError::ServiceUnavailable] = "ServiceUnavailable";
        }
    }

        void SysDialogNDK::onChanged(bool value) {
            int x = 0;
        }

    void SysDialogNDK::onDialogFinished(NotificationResult::Type value) {
        Json::FastWriter writer;
        Json::Value root;
        SystemUiButton * select;

        switch(value) {

            case NotificationResult::None:
                root["result"] = "None";
                break;

            case NotificationResult::Error:
                // root["error"] = m_errorMessage[m_notificationDialog->error()];
                root["error"] = m_notificationDialog->error();
                break;

            case NotificationResult::ButtonSelection:
                select = m_notificationDialog->buttonSelection();
                if ( NULL != select) {
                    root["select"] = (select - m_buttonList[0])/sizeof(SystemUiButton * );
                }
                break;

            default:
                root["error"] = "unknown NotificationResult Type";
                break;
        } // switch
        m_pParent->NotifyEvent(m_callbackId + " " + writer.write(root));

        delete m_notificationDialog;
    }


    string SysDialogNDK::show(std::string& callbackId, const std::string& inputString) {

        m_callbackId = callbackId;

        Json::Reader reader;
        Json::Value root, value;
        QString context;
        unsigned int i, len;

        bool parse = reader.parse(inputString, root);

        if (!parse) {
            return "sysDialogNDK Parse Error";
        }

        m_notificationDialog = new NotificationDialog();

        // message
        value = root["message"];
        if (!value.empty()) {
            context = QString::fromStdString(value.asString());
            m_notificationDialog->setBody(context);
        }

        // button list
        value = root["buttons"];
        if (!value.empty()) {

            len = value.size();
            m_buttonList = new SystemUiButton*[len];
            for (i = 0; i < len; ++i) {
                context = QString::fromStdString(value[i].asString());
                m_buttonList[i] = new SystemUiButton(context);
                m_notificationDialog->appendButton(m_buttonList[i]);
            }
        }

        // title
        value = root["settings"]["title"];
        if (!value.empty()) {
            context = QString::fromStdString(value.asString());
            m_notificationDialog->setTitle(context);
        }

        // repeat
        value = root["settings"]["repeat"];
        if (value.asBool()) {
            m_notificationDialog->setRepeat(true);
        }

        bool success = false;
        // success = QMetaObject::checkConnectArgs(SIGNAL(finished(SystemUiResult::Type)), SLOT(onDialogFinished(SystemUiResult::Type)));
// QMetaMethod valueChangedSignal = QMetaMethod::fromSignal(&MyObject::valueChanged);
// if (! success) {
//     return "not match";
// }

// success = m_notificationDialog->isSignalConnected();
// if ( success){
//     return "signal connected";
// }
        success = QObject::connect(m_notificationDialog, SIGNAL(finished(SystemUiResult::ButtonSelection *)),
                                        this, SLOT(onDialogFinished(SystemUiResult::Type)));

        // success = QObject::connect(m_notificationDialog, SIGNAL(finished(SystemUiResult::Type)),
        //                                 this, SLOT(onDialogFinished(SystemUiResult::Type)));

        // success = QObject::connect(m_notificationDialog, SIGNAL(finished(SystemUiResult::Type)),
        //                                 m_notificationDialog, SLOT(onDialogFinished(SystemUiResult::Type)));



 // bool success = QObject::connect(m_notificationDialog, SIGNAL(repeatChanged (bool )),
 //         this, SLOT(onSelected(bool)));

            // NotificationResult::Type ret = m_notificationDialog->exec();
            m_notificationDialog->show();
            NotificationResult::Type ret = m_notificationDialog->result();

            // while (NotificationResult::None == m_notificationDialog->result()){}


// onDialogFinished(NotificationResult::ButtonSelection);
// m_notificationDialog->finished(NotificationResult::Error);

        if (success) {
            // m_notificationDialog->show();
            return "ok?";

        } else {

            // onDialogFinished(m_notificationDialog->result());


            if (NotificationResult::None == ret) {
                return "No result";
            }

            if (NotificationResult::Error == ret) {
                return m_errorMessage[m_notificationDialog->error()];

            }

            return "select";

            // onDialogFinished(m_notificationDialog->result()  );
            // return "SysDialogNdk fail to connect finished()";
            // return m_errorMessage[m_notificationDialog->error()];
        }

        return "";
    }

} /* namespace webworks */
