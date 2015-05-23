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
                    mp->setSourceUrl(QUrl("http://www.pch.gc.ca/DAMAssetPub/DAM-hymChs-antSgn/STAGING/audio-audio/o-canada_1359474460106_eng.MP3"));

                   QObject::connect(this, SIGNAL(playSignal()),
                                    this, SLOT(play()));

                   QObject::connect(this, SIGNAL(stopSignal()),
                                    this, SLOT(stop()));

                   QObject::connect(this, SIGNAL(pauseSignal()),
                                    this, SLOT(pause()));

                }


            virtual ~NowPlayingNDK(){};

            void sendEvent( const std::string& msg);



      signals:
          void playSignal();
          void stopSignal();
          void pauseSignal();

        public slots:
            void play();
            void stop();
            void pause();

        public:
            std::string NowPlayingConnectionTest();
            std::string NowPlayingStop();
            std::string NowPlayingPlay();
            std::string NowPlayingPause();
            std::string NowPlayingSwitchMusic(const std::string& src);
            std::string NowPlayingGetSourceAddress();
            std::string NowPlayingGetDuration();
            std::string NowPlayingGetPosition();
            void NowPlayingSetMetadata(const std::string& callbackId, const std::string& data);

        };




} // namespace webworks

#endif
