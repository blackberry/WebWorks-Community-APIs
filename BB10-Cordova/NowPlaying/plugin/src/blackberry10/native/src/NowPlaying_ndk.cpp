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
#include <QtCore>
#include <QMetaObject>
#include <QMetaEnum>
#include <QObject>
#include <bb/platform/NotificationError>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include "NowPlaying_ndk.hpp"
#include "NowPlaying_js.hpp"
#include "ApplicationThread.hpp"

#include <bb/multimedia/MediaPlayer>
#include <bb/multimedia/NowPlayingConnection>
#include <bb/multimedia/MetaData>

namespace webworks {

    using namespace std;
    using namespace bb::multimedia;

    void NowPlayingNDK::sendEvent(const string& msg) {
        m_pParent->NotifyEvent(msg);
    }

    string NowPlayingNDK::NowPlayingSetMusic(const string& data) {
        // Ensure media player is stopped.
        emit stopSignal();

        QString str = QString::fromUtf8(data.c_str());

        if (! str.startsWith("http", Qt::CaseInsensitive)) {
            char cwd[PATH_MAX];
            getcwd(cwd, PATH_MAX);
            str.prepend(QString(cwd).append("/app/native/"));
        }

        QUrl url(str);

        if (url.isValid()){
          mp->setSourceUrl(url);
          return "Music set to " + data + ".";
        } else {
          return "Music couldn't be set to argument because it is invalid.";
        }
    }

    string NowPlayingNDK::NowPlayingSetMetadata(const string& data) {
        string returnValue = "";

        Json::Value root;
        Json::Reader reader;

        bool parse = reader.parse(data, root);
        if (!parse) {
            returnValue = "Error setting metadata";
        } else {
            QVariantMap metadata;

            metadata[MetaData::Title] = QString::fromStdString(root["Title"].asString());
            metadata[MetaData::Artist] = QString::fromStdString(root["Artist"].asString());
            metadata[MetaData::Album] = QString::fromStdString(root["Album"].asString());

            npc->setMetaData(metadata);

            returnValue = "Metadata set successfully.";
        }

        return returnValue;
    }

    string NowPlayingNDK::NowPlayingSetIcon(const string& data) {
        return "Icon set.";
    }

    string NowPlayingNDK::NowPlayingChangeTrack(const string& callbackId, const string& data) {
        return "Changed track.";
    }

    string NowPlayingNDK::NowPlayingEnableNextPrevious() {
        return "Enabled Next Previous.";
    }

    string NowPlayingNDK::NowPlayingDisableNextPrevious() {
        return "Disabled Next Previous.";
    }

    string NowPlayingNDK::NowPlayingPlay(const string& callbackId, const string& data) {
        emit playSignal();
        return "Player started.";
    }

    string NowPlayingNDK::NowPlayingPause(const string& callbackId) {
        emit pauseSignal();
        return "Player Paused.";
    }

    string NowPlayingNDK::NowPlayingResume(const string& callbackId) {
        emit resumeSignal();
        return "Player Resumed.";
    }

    string NowPlayingNDK::NowPlayingStop(const string& callbackId) {
        emit stopSignal();
        return "Player Stopped.";
    }

    string NowPlayingNDK::NowPlayingGetState() {
        string state = "State: ";
        switch (mp->mediaState()) {
            case bb::multimedia::MediaState::Unprepared:
                state += "Unprepared";
                break;
            case bb::multimedia::MediaState::Prepared:
                state += "Prepared";
                break;
            case bb::multimedia::MediaState::Started:
                state += "Started";
                break;
            case bb::multimedia::MediaState::Paused:
                state += "Paused";
                break;
            case bb::multimedia::MediaState::Stopped:
                state += "Stopped";
                break;
            default:
                state += "Unknown";
        }

        state += " Acquired: ";
        state += npc->isAcquired() ? "True" : "False";

        state += " Preempted: ";
        state += npc->isPreempted() ? "True" : "False";

        return state.c_str();
    }

    /***
     * Slots
     ***/

    void NowPlayingNDK::play() {
        //npc->acquire();

        mp->play();

        //npc->setMediaState(mp->mediaState());
    }

    void NowPlayingNDK::resume() {
        mp->play();

        //npc->setMediaState(mp->mediaState());
    }

    void NowPlayingNDK::pause() {
        mp->pause();

        //npc->setMediaState(mp->mediaState());
    }

    void NowPlayingNDK::stop() {
        //npc->revoke();

        mp->stop();

        //npc->setMediaState(mp->mediaState());
    }

} /* namespace webworks */
