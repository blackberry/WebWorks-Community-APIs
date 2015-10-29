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

    string NowPlayingNDK::NowPlayingRequestPlayback(const string& data) {
        string returnValue = "";

        npc->acquire();

        /* Set the volume overlay over the media notification area.
         *
         * "I don’t think there’s any control to change the size of the
         * media notification area – it’s set by the OS and the
         * hardware." - quoted from Tim Windsor */
        npc->setOverlayStyle(OverlayStyle::Fancy);

        Json::Value root;
        Json::Reader reader;

        bool parse = reader.parse(data, root);
        if (!parse) {
            returnValue = "Error requesting playback";
        } else {
            returnValue += setMusic(root["songURL"].asString());
            returnValue += setIcon(root["iconURL"].asString());
            returnValue += setMetadata(root["metadata"]);
            returnValue += "Playback requested successfully.";
        }

        return returnValue;
    }

    void NowPlayingNDK::NowPlayingBindPlayCallback(const string& callbackId) {
        playCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);
        connectResult = this->connect(
                            this,
                            SIGNAL(playSignal()),
                            this,
                            SLOT(playSlot())
                        );
        Q_ASSERT(connectResult);
    }

    void NowPlayingNDK::NowPlayingBindPauseCallback(const string& callbackId) {
        pauseCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);
        connectResult = this->connect(
                            this,
                            SIGNAL(pauseSignal()),
                            this,
                            SLOT(pauseSlot())
                        );
        Q_ASSERT(connectResult);
    }

    void NowPlayingNDK::NowPlayingBindStopCallback(const string& callbackId) {
        stopCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);
        connectResult = this->connect(
                            this,
                            SIGNAL(stopSignal()),
                            this,
                            SLOT(stopSlot())
                        );
        Q_ASSERT(connectResult);
    }

    void NowPlayingNDK::NowPlayingBindResumeCallback(const string& callbackId) {
        resumeCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);
        connectResult = this->connect(
                            this,
                            SIGNAL(resumeSignal()),
                            this,
                            SLOT(resumeSlot())
                        );
        Q_ASSERT(connectResult);
    }

    /**
     * Assume NowPlayingConnection instance is acquired. No need to revoke.
     */
    string NowPlayingNDK::setMusic(const string& data) {
        Q_ASSERT(npc->isAcquired());

        QString str = QString::fromUtf8(data.c_str());

        if (! str.startsWith("http", Qt::CaseInsensitive)) {
            char cwd[PATH_MAX];
            getcwd(cwd, PATH_MAX);
            str.prepend(QString(cwd).append("/app/native/"));
        }

        QUrl url(str);

        if (url.isValid()){
          mp->setSourceUrl(url);
          return "Music set to " + data + "\n";
        } else {
          return "Music couldn't be set to " + data
                  + " because it is invalid.\n";
        }
    }

    /**
     * Assume NowPlayingConnection instance is acquired. No need to revoke.
     */
    string NowPlayingNDK::setIcon(const string& data) {
        Q_ASSERT(npc->isAcquired());

        QString str = QString::fromUtf8(data.c_str());

        if (! str.startsWith("http", Qt::CaseInsensitive)) {
            char cwd[PATH_MAX];
            getcwd(cwd, PATH_MAX);
            str.prepend(QString(cwd).append("/app/native/"));
        } else {
            return "Icon couldn't be set to " + data + " because HTTP URLs "
                   + "aren't currently supported as icons. \n";
        }

        QUrl url(str);

        if (url.isValid()){
            npc->setIconUrl(url);
            return "Icon set to " + data + "\n";
        } else {
            return "Icon couldn't be set to " + data +
                    " because it is invalid.\n";
        }
    }

    /**
     * Assume NowPlayingConnection instance is acquired. No need to revoke.
     */
    string NowPlayingNDK::setMetadata(const Json::Value& data) {
        Q_ASSERT(npc->isAcquired());

        QVariantMap metadata;

        metadata[MetaData::Title] =
                QString::fromStdString(data["Title"].asString());
        metadata[MetaData::Artist] =
                QString::fromStdString(data["Artist"].asString());
        metadata[MetaData::Album] =
                QString::fromStdString(data["Album"].asString());

        npc->setMetaData(metadata);

        return "Metadata set successfully. \n";
    }

//    string NowPlayingNDK::NowPlayingChangeTrack(const string& callbackId, const string& data) {
//        return "Changed track.";
//    }
//
//    string NowPlayingNDK::NowPlayingEnableNextPrevious() {
//        if (npc->isAcquired()) {
//            npc->setNextEnabled(true);
//        } else {
//            npc->acquire();
//            npc->setNextEnabled(true);
//            npc->revoke();
//        }
//
//        return "Enabled Next Previous.";
//    }
//
//    string NowPlayingNDK::NowPlayingDisableNextPrevious() {
//        return "Disabled Next Previous.";
//    }

    string NowPlayingNDK::NowPlayingPlay() {
        emit playSignal();
        return "Player started.";
    }

    string NowPlayingNDK::NowPlayingPause() {
        emit pauseSignal();
        return "Player paused.";
    }

    string NowPlayingNDK::NowPlayingStop() {
        emit stopSignal();
        return "Player stopped.";
    }

    string NowPlayingNDK::NowPlayingResume() {
        emit resumeSignal();
        return "Player resumed.";
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

    void NowPlayingNDK::playSlot() {
        mp->play();

        npc->setMediaState(mp->mediaState());

        // Callback
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Playing! (callback).";
        sendEvent(playCallbackId + " " + writer.write(root));
    }

    void NowPlayingNDK::pauseSlot() {
        mp->pause();

        npc->setMediaState(mp->mediaState());

        // Callback
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Pausing! (callback).";
        sendEvent(pauseCallbackId + " " + writer.write(root));
    }

    void NowPlayingNDK::stopSlot() {
        mp->stop();
        npc->setMediaState(mp->mediaState());

        // Callback
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Stopping! (callback).";
        sendEvent(stopCallbackId + " " + writer.write(root));

        // TODO: revoke and disconnect callback bindings?
    }

    void NowPlayingNDK::resumeSlot() {
        mp->play();
        npc->setMediaState(mp->mediaState());

        // Callback
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Resuming! (callback).";
        sendEvent(resumeCallbackId + " " + writer.write(root));
    }

//    void NowPlayingNDK::next() {
//        npc->revoke();
//
//        mp->stop();
//
//        npc->setMediaState(mp->mediaState());
//    }

} /* namespace webworks */
