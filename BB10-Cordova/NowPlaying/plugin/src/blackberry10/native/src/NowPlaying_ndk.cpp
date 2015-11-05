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

    string NowPlayingNDK::setMusic(const string& data) {
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

    string NowPlayingNDK::setIcon(const string& data) {
        QString str = QString::fromUtf8(data.c_str());

        if (! str.startsWith("http", Qt::CaseInsensitive)) {
            char cwd[PATH_MAX];
            getcwd(cwd, PATH_MAX);
            str.prepend(QString(cwd).append("/app/native/"));
        } else {
            /* http://developer.blackberry.com/native/reference/cascades/bb__multimedia__nowplayingconnection.html#function-play
             * Comment 2 years ago from Oct 30, 2015.
             * Theodore Mavrakis: "How can we pass an http url to use for the icon of a NowPlayingConnection?"
             * Wes Barichak: "Currently, this is not possible. But, we will be looking at adding this functionality in a future release." */
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

    string NowPlayingNDK::setMetadata(const Json::Value& data) {
        QVariantMap metadata;

        /**
         * http://developer.blackberry.com/native/reference/cascades/bb__multimedia__nowplayingconnection.html#comment-1134791487
         * Comment 2 years ago from Oct 30, 2015.
         * Wes Barichak: "Currently, the only metadata properties that are available are
         * MetaData::Album, MetaData::Artist, and MetaData::Title, while the
         * rest of the MetaData properties are ignored.
         * This will likely change in the future though."
         *
         * TODO: Want this to show on volume overlay
         * TODO: Try putting more properties */
        metadata[MetaData::Title] =
                QString::fromStdString(data["Title"].asString());
        metadata[MetaData::Artist] =
                QString::fromStdString(data["Artist"].asString());
        metadata[MetaData::Album] =
                QString::fromStdString(data["Album"].asString());

        npc->setMetaData(metadata);

        return "Metadata set successfully. \n";
    }


    /**********
     * Slots
     **********/

    void NowPlayingNDK::mediaStateMapperSlot(bb::multimedia::MediaState::Type mediaState) {
        switch(mediaState) {
            case MediaState::Started:
                emit playSignal();
                break;
            case MediaState::Paused:
                emit pauseSignal();
                break;
            case MediaState::Stopped:
                emit stopSignal();
                break;
            default:
                // Do nothing.
                break;
        }
    }

    void NowPlayingNDK::playSlot() {
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Playing callback!";
        sendEvent(playCallbackId + " " + writer.write(root));
    }

    void NowPlayingNDK::pauseSlot() {
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Pausing callback!";
        sendEvent(pauseCallbackId + " " + writer.write(root));
    }

    void NowPlayingNDK::stopSlot() {
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Stopping callback!";
        sendEvent(stopCallbackId + " " + writer.write(root));
    }

    void NowPlayingNDK::nextSlot() {
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Next track callback!";
        sendEvent(nextCallbackId + " " + writer.write(root));
    }

    void NowPlayingNDK::previousSlot() {
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Previous track callback!";
        sendEvent(previousCallbackId + " " + writer.write(root));
    }

    void NowPlayingNDK::errorSlot() {
        Json::FastWriter writer;
        Json::Value root;
        root["result"] = "Error callback!";
        sendEvent(errorCallbackId + " " + writer.write(root));
    }

    /**
     * NowPlayingNDK::NowPlayingRequestPlayback
     * Sets up NowPlaying for use
     */
    string NowPlayingNDK::NowPlayingRequestPlayback() {

        /* Set the volume overlay over the media notification area.
         *
         * "I don’t think there’s any control to change the size of the
         * media notification area – it’s set by the OS and the
         * hardware." - quoted from Tim Windsor */
        npc->setOverlayStyle(OverlayStyle::Fancy);

        // Set up mapper for user-triggerable play, pause, and stop callbacks
        bool connectResult;
        Q_UNUSED(connectResult);
        connectResult = connect(
                            mp,
                            SIGNAL(mediaStateChanged(bb::multimedia::MediaState::Type)),
                            this,
                            SLOT(mediaStateMapperSlot(bb::multimedia::MediaState::Type))
                        );
        Q_ASSERT(connectResult);

        return "Playback requested successfully.";
    }

    void NowPlayingNDK::NowPlayingBindPlayCallback(const string& callbackId) {

        // Set up callback channel
        playCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);

        // Set up user-triggerable callback
        connectResult = QObject::connect(
                            this,
                            SIGNAL(playSignal()),
                            this,
                            SLOT(playSlot())
                        );
        Q_ASSERT(connectResult);

        // Set up system-triggerable callback
        connectResult = QObject::connect(
                            npc,
                            SIGNAL(play()),
                            this,
                            SLOT(playSlot())
                        );
        Q_ASSERT(connectResult);

    }

    void NowPlayingNDK::NowPlayingBindPauseCallback(const string& callbackId) {

        // Set up callback channel
        pauseCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);

        // Set up user-triggerable callback
        connectResult = QObject::connect(
                            this,
                            SIGNAL(pauseSignal()),
                            this,
                            SLOT(pauseSlot())
                        );
        Q_ASSERT(connectResult);

        // Set up system-triggerable callback
        connectResult = QObject::connect(
                            npc,
                            SIGNAL(pause()),
                            this,
                            SLOT(pauseSlot())
                        );
        Q_ASSERT(connectResult);

    }

    void NowPlayingNDK::NowPlayingBindStopCallback(const string& callbackId) {

        // Set up callback channel
        stopCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);

        // Set up user-triggerable callback
        connectResult = QObject::connect(
                            this,
                            SIGNAL(stopSignal()),
                            this,
                            SLOT(stopSlot())
                        );
        Q_ASSERT(connectResult);

        // Set up system-triggerable callback
        connectResult = QObject::connect(
                            npc,
                            SIGNAL(stop()),
                            this,
                            SLOT(stopSlot())
                        );
        Q_ASSERT(connectResult);

    }

    void NowPlayingNDK::NowPlayingBindNextCallback(const string& callbackId) {

        // Set up callback channel
        nextCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);

        // Set up user-triggerable callback
        connectResult = QObject::connect(
                            this,
                            SIGNAL(nextSignal()),
                            this,
                            SLOT(nextSlot())
                        );
        Q_ASSERT(connectResult);

        // Set up system-triggerable callback
        connectResult = QObject::connect(
                            npc,
                            SIGNAL(next()),
                            this,
                            SLOT(nextSlot())
                        );
        Q_ASSERT(connectResult);

    }

    void NowPlayingNDK::NowPlayingBindPreviousCallback(const string& callbackId) {

        // Set up callback channel
        previousCallbackId = callbackId;

        bool connectResult;
        Q_UNUSED(connectResult);

        // Set up user-triggerable callback
        connectResult = QObject::connect(
                            this,
                            SIGNAL(previousSignal()),
                            this,
                            SLOT(previousSlot())
                        );
        Q_ASSERT(connectResult);

        // Set up system-triggerable callback
        connectResult = QObject::connect(
                            npc,
                            SIGNAL(previous()),
                            this,
                            SLOT(previousSlot())
                        );
        Q_ASSERT(connectResult);

    }

    void NowPlayingNDK::NowPlayingBindErrorCallback(const string& callbackId) {

        // Set up callback channel
        errorCallbackId = callbackId;

        // Set up callback
        bool connectResult;
        Q_UNUSED(connectResult);
        connectResult = QObject::connect(
                            this,
                            SIGNAL(errorSignal()),
                            this,
                            SLOT(errorSlot())
                        );
        Q_ASSERT(connectResult);
    }

    string NowPlayingNDK::NowPlayingPlay(const string& data) {
        string returnValue = "";

        Json::Value root;
        Json::Reader reader;
        bool parse = reader.parse(data, root);
        if (!parse) {
            return "Error parsing data";
        }

        // Set up music, icon, and metadata
        returnValue += setMusic(root["songURL"].asString());
        returnValue += setIcon(root["iconURL"].asString());
        returnValue += setMetadata(root["metadata"]);

        // Set up next/previous buttons on the volume overlay
        npc->setNextEnabled(root["nextEnabled"].asBool());
        npc->setPreviousEnabled(root["previousEnabled"].asBool());

        // Acquire the media notification area if it isn't already
        if (!npc->isAcquired()) {
            npc->acquire();
        }

        // Stop any currently playing music, then play the newly set up one.
        mp->stop();
        mp->play();

        returnValue += "Player started successfully.";
        return returnValue;
    }

    string NowPlayingNDK::NowPlayingPause() {
        mp->pause();

        return "Player paused successfully.";
    }

    string NowPlayingNDK::NowPlayingResume() {
        mp->play();

        return "Player resumed successfully.";
    }

    string NowPlayingNDK::NowPlayingStop() {

        // Stop the music
        mp->stop();

        // Revoke the media notification area
        npc->revoke();

        return "Player stopped successfully.";
    }

    string NowPlayingNDK::NowPlayingNext() {

        // Let the slot handle playing the next song
        emit nextSignal();

        return "Switching to next song.";
    }

    string NowPlayingNDK::NowPlayingPrevious() {

        // Let the slot handle playing the previous song
        emit previousSignal();

        return "Switching to previous song";
    }

    /**
     * NowPlayingNDK::NowPlayingError
     * TODO: can create error enum and this takes params accordingly. This can
     * then be called throughout the methods of this class.
     */
    string NowPlayingNDK::NowPlayingError() {

       // Let the slot handle the error
       emit errorSignal();

       return "Error detected.";
    }

    /**
     * NowPlayingNDK::NowPlayingGetState
     * Return the state of NowPlaying.
     */
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
                break;
        }
        state += "\n";

        state += " Acquired: ";
        state += npc->isAcquired() ? "True" : "False";
        state += "\n";

        state += " Preempted: ";
        state += npc->isPreempted() ? "True" : "False";
        state += "\n";

        return state.c_str();
    }
} /* namespace webworks */
