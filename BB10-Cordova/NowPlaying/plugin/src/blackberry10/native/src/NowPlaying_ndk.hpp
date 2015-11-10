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

    using namespace std;
    using namespace bb::multimedia;

    class NowPlayingNDK : public QObject {

        Q_OBJECT

        NowPlayingJS *m_pParent;

        bb::multimedia::MediaPlayer *mp;
        bb::multimedia::NowPlayingConnection *npc;

        void sendEvent(const string& msg);

        string setMusic(const string& data);
        string setIcon(const string& data);
        string setMetadata(const Json::Value& data);

        private:
            string playCallbackId;
            string pauseCallbackId;
            string stopCallbackId;
            string nextCallbackId;
            string previousCallbackId;
            string errorCallbackId;

        public:
            explicit NowPlayingNDK(NowPlayingJS *parent = NULL): QObject(), m_pParent(parent)
            {
                mp = new MediaPlayer(this);
                npc = new NowPlayingConnection(this);
            }

            virtual ~NowPlayingNDK() {};

            signals:
                void playSignal();
                void pauseSignal();
                void stopSignal();
                void nextSignal();
                void previousSignal();

                void errorSignal();

            public slots:
                void joinSlot(const string& windowGroup);

                void playSlot();
                void pauseSlot();
                void stopSlot();
                void nextSlot();
                void previousSlot();

                void errorSlot();

            string NowPlayingRequestPlayback();
            void NowPlayingBindPlayCallback(const string& callbackId);
            void NowPlayingBindPauseCallback(const string& callbackId);
            void NowPlayingBindStopCallback(const string& callbackId);
            void NowPlayingBindNextCallback(const string& callbackId);
            void NowPlayingBindPreviousCallback(const string& callbackId);
            void NowPlayingBindErrorCallback(const string& callbackId);

            string NowPlayingPlay(const string& data);
            string NowPlayingPause();
            string NowPlayingResume();
            string NowPlayingStop();
            string NowPlayingNext();
            string NowPlayingPrevious();

            string NowPlayingError();

            string NowPlayingGetState();

    }; // class NowPlayingNDK

} // namespace webworks

#endif
