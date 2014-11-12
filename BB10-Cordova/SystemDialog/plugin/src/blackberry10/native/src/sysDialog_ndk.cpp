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

#include <map>
#include <sstream>
#include <QObject>
#include <bb/platform/NotificationError>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include "sysDialog_ndk.hpp"
#include "sysDialog_js.hpp"

namespace webworks {

    using namespace bb::platform;
    using namespace bb::system;
    using namespace std;
    map<NotificationError::Type, string> SysDialogNDK::m_errorMessage;

    SysDialogNDK::~SysDialogNDK() {
        // in case show returns with error before finished();
        cleanUp();
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

    void SysDialogNDK::cleanUp() {

        if (NULL != m_notificationDialog) {
            // The button instances will be deleted.
            m_notificationDialog->clearButtons();
            delete m_notificationDialog;
        }
        m_notificationDialog = NULL;


        if (NULL != m_buttonList) {
            delete[] m_buttonList;
        }
        m_buttonList = NULL;
    }


    void SysDialogNDK::onDialogFinished(bb::platform::NotificationResult::Type value) {
        Json::FastWriter writer;
        Json::Value root;
        SystemUiButton * select;

        switch(value) {

            case NotificationResult::None:
                root["result"] = "None";
                break;

            case NotificationResult::Error:
                root["result"] = "error";
                root["error"] = m_errorMessage[m_notificationDialog->error()];
                break;

            case NotificationResult::ButtonSelection:
                root["result"] = "select";
                select = m_notificationDialog->buttonSelection();
                if ( NULL != select) {
                    root["select"] = (select - m_buttonList[0])/sizeof(SystemUiButton * );
                }
                break;

            default:
                root["result"] = "error";
                root["error"] = "unknown NotificationResult Type";
                break;
        } // switch

        // callback, notify js object
        m_pParent->NotifyEvent(m_callbackId + " " + writer.write(root));

        // clean before next dialog invoked
        cleanUp();
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

        bool success = QObject::connect(m_notificationDialog,
                        SIGNAL(finished(bb::platform::NotificationResult::Type)),
                        this,
                        SLOT(onDialogFinished(bb::platform::NotificationResult::Type)));


        if (success) {
            m_notificationDialog->show();
        } else {
            // this is not normal in most cases
            return "sysDialogNDK qt connect fail";
        }

        return "";
    }

} /* namespace webworks */
