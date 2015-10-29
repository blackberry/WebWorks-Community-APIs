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

#ifndef _NOWPLAYING_NDK_HPP_
#define _NOWPLAYING_NDK_HPP_

#include <string>
#include <vector>
#include <unistd.h>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QtCore>
#include <json/value.h>

#include <bb/multimedia/MediaPlayer>
#include <bb/multimedia/NowPlayingConnection>

class NowPlayingJS;

namespace webworks {

    using namespace bb::multimedia;
    using namespace std;

    class NowPlayingNDK : public QObject {

        Q_OBJECT

        NowPlayingJS *m_pParent;

        bb::multimedia::MediaPlayer *mp;
        bb::multimedia::NowPlayingConnection *npc;

        public:

            explicit NowPlayingNDK(NowPlayingJS *parent = NULL): QObject(), m_pParent(parent)
            {
                mp = new bb::multimedia::MediaPlayer(this);
                npc = new bb::multimedia::NowPlayingConnection(this);
            }

            virtual ~NowPlayingNDK() {};

            void sendEvent(const string& msg);

            signals:
                void playSignal();
                void pauseSignal();
                void stopSignal();
                void resumeSignal();

                /* TODO (wait for email response): Need to be able to connect built-in play, pause signals etc..
                 * so can handle from volume overlay/MNA???  */

            public slots:
                void playSlot();
                void pauseSlot();
                void stopSlot();
                void resumeSlot();
//                void next();

        public:
            string NowPlayingRequestPlayback(const string& data);
            void NowPlayingBindPlayCallback(const string& callbackId);
            void NowPlayingBindPauseCallback(const string& callbackId);
            void NowPlayingBindStopCallback(const string& callbackId);
            void NowPlayingBindResumeCallback(const string& callbackId);

            string NowPlayingTrackChange(const string& data);

            string setMusic(const string& data);
            string setIcon(const string& data);
            string setMetadata(const Json::Value& data);

            string NowPlayingPlay();
            string NowPlayingPause();
            string NowPlayingStop();
            string NowPlayingResume();

            string NowPlayingGetState();

        private:
            string playCallbackId;
            string pauseCallbackId;
            string stopCallbackId;
            string resumeCallbackId;

    }; // class NowPlayingNDK

} // namespace webworks

#endif
