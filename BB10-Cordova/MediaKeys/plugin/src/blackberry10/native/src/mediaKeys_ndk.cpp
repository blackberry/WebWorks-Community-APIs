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

namespace webworks {

    using namespace std;

    void MediaKeysNDK::join( string inputString) {
        QByteArray byteArray(inputString.c_str(), inputString.length());
        m_pParent->applicationThread()->join(byteArray);
    }

    void MediaKeysNDK::cleanup() {
            qDeleteAll(m_mediaKeyHandlerList.values());
            deleteLater();
    }

    void MediaKeysNDK::sendEvent( const std::string& msg) {
        m_pParent->NotifyEvent(msg);
    }

    // All Enum media key types can be found at:
    // http://developer.blackberry.com/native/reference/cascades/bb__multimedia__mediakey.html#enum-type
    MediaKey::Type MediaKeysNDK::getMediaKey(string mediaKeyStr) {
        MediaKey::Type mediaKey = MediaKey::None;

        if (mediaKeyStr == "fastForward") {
            mediaKey = MediaKey::FastForward;
        } else if (mediaKeyStr == "pause") {
            mediaKey = MediaKey::Pause;
        } else if (mediaKeyStr == "play") {
            mediaKey = MediaKey::Play;
        } else if (mediaKeyStr == "playPause") {
            mediaKey = MediaKey::PlayPause;
        } else if (mediaKeyStr == "rewind") {
            mediaKey = MediaKey::Rewind;
        } else if (mediaKeyStr == "stop") {
            mediaKey = MediaKey::Stop;
        } else if (mediaKeyStr == "volumeDown") {
            mediaKey = MediaKey::VolumeDown;
        } else if (mediaKeyStr == "volumeUp") {
            mediaKey = MediaKey::VolumeUp;
        } else if (mediaKeyStr == "previous") {
            mediaKey = MediaKey::Previous;
        } else if (mediaKeyStr == "next") {
            mediaKey = MediaKey::Next;
        }

        return mediaKey;
    }

    // returns the ID of the mediaKey
    string MediaKeysNDK::bind(string callbackId, string inputString) {
        this->m_pParent->getLog()->debug("Binding keyWatcher");
        this->m_pParent->getLog()->debug(inputString.c_str());

        Json::Reader reader;
        Json::Value root, value;
        string mediaKey, keyLength;

        bool parse = reader.parse(inputString, root);

        if (parse == false) {
            return "parse error";
        }

        value = root["mediakey"];
        if (value.empty()) {
            return "no mediakey defined";
        }

        mediaKey = value.asString();

        value = root["keylength"];
        if (value.empty()) {
            return "no keylength defined";
        }

        keyLength = value.asString();

        MediaKeyWatcher *mediaKeyWatcher = NULL;
        MediaKeysHandler *mediaKeysHandler = NULL;

        MediaKey::Type keyType = getMediaKey(mediaKey);
        if (keyType == MediaKey::None) {
            return "invalid mediakey type";
        }

        mediaKeyWatcher = new MediaKeyWatcher(keyType);
        mediaKeysHandler = new MediaKeysHandler(this, callbackId, mediaKeyWatcher, mediaKey, keyLength);

        int key = this->m_id++;
        this->m_mediaKeyHandlerList.insert(key, mediaKeysHandler);

        bool success = false;

        if (keyLength == "short") {
            success = QObject::connect(
                mediaKeyWatcher, SIGNAL(shortPress(bb::multimedia::MediaKey::Type)),
                mediaKeysHandler, SLOT(onPress(bb::multimedia::MediaKey::Type)));
        } else if (keyLength == "long") {
            success = QObject::connect(
                mediaKeyWatcher, SIGNAL(longPress(bb::multimedia::MediaKey::Type)),
                mediaKeysHandler, SLOT(onPress(bb::multimedia::MediaKey::Type)));
        }

        string error = "Unable to bind "+mediaKey+" with "+keyLength+" key";

        return (success == true ? "" : error);
    }

    void MediaKeysHandler::onPress(bb::multimedia::MediaKey::Type key) {
        Json::FastWriter writer;
        Json::Value root;

        root["result"] = "mediaKeyPressed";
        root["mediakey"] = this->m_mediaKey;
        root["keylength"] = this->m_keyLength;

        m_parentNDK->sendEvent(this->m_callbackId + " " + writer.write(root));
    }

} /* namespace webworks */
