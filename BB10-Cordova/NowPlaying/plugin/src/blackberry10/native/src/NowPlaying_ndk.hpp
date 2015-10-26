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

                /* Set the volume overlay over the media notification area. */
                npc->acquire();
                npc->setOverlayStyle(bb::multimedia::OverlayStyle::Fancy);
                npc->revoke();

//                QObject::connect(this, SIGNAL(playSignal()),
//                                this, SLOT(play()));
//                QObject::connect(this, SIGNAL(pauseSignal()),
//                                this, SLOT(pause()));
//                QObject::connect(this, SIGNAL(resumeSignal()),
//                                this, SLOT(resume()));
//                QObject::connect(this, SIGNAL(stopSignal()),
//                                this, SLOT(stop()));
            }

            virtual ~NowPlayingNDK() {};

            void sendEvent(const string& msg);

            signals:
                void playSignal();
                void pauseSignal();
                void stopSignal();
//                void resumeSignal();
//                void stopSignal();

            public slots:
                void playSlot();
                void pauseSlot();
                void stopSlot();
//                void resume();
//                void stop();
//                void next();

        public:
            string NowPlayingRequestPlayback(const string& data);
            void NowPlayingBindPlayCallback(const string& callbackId);
            string NowPlayingSetMusic(const string& data);
            void NowPlayingBindPauseCallback(const string& callbackId);
            void NowPlayingBindStopCallback(const string& callbackId);
//            string NowPlayingSetMetadata(const string& data);
//            string NowPlayingSetIcon(const string& data);
//
//            string NowPlayingChangeTrack(const string& callbackId, const string& data);
//
//            string NowPlayingEnableNextPrevious();
//            string NowPlayingDisableNextPrevious();

            string NowPlayingPlay();
            string NowPlayingPause();
            string NowPlayingStop();
//            string NowPlayingPause(const string& callbackId);
//            string NowPlayingResume(const string& callbackId);
//            string NowPlayingStop(const string& callbackId);

            string NowPlayingGetState();

        private:
            string playCallbackId;
            string pauseCallbackId;
            string stopCallbackId;

    }; // class NowPlayingNDK

} // namespace webworks

#endif
