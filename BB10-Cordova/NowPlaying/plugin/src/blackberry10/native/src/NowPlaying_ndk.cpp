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

   void NowPlayingNDK::sendEvent(const string& msg){
       m_pParent->NotifyEvent(msg);
   }

   string NowPlayingNDK::NowPlayingConnectionTest(){
       return "NowPlayingConnectionTest finished, everything looks good.";
   }

   string NowPlayingNDK::NowPlayingStop(){
       emit stopSignal();
       return "Player Stopped.";
   }

   string NowPlayingNDK::NowPlayingPause(){
         emit pauseSignal();
         return "Player Paused.";
   }


   string NowPlayingNDK::NowPlayingPlay(){
        emit playSignal();
        return "Player started.";
   }


   string NowPlayingNDK::NowPlayingSwitchMusic(const string &src){

         emit stopSignal();
         QString str = QString::fromUtf8(src.c_str());
         QUrl url(str);
         if(url.isValid()){
               mp->setSourceUrl(url);
               return "Music switch to " + src;
          }else{
               return "Sorry, but the url is not valid";
          }

    }

   string NowPlayingNDK::NowPlayingGetSourceAddress(){
       return mp->sourceUrl().toString().toUtf8().constData();
   }

   string NowPlayingNDK::NowPlayingGetDuration(){
       int duration = mp->duration();
       QString s = QString::number(duration);
       return s.toUtf8().constData();
   }

   string NowPlayingNDK::NowPlayingGetPosition(){
       int position = mp->position();
       QString s = QString::number(position);
       return s.toUtf8().constData();
   }




   void NowPlayingNDK::play(){
      mp->play();

      npc->setDuration(mp->duration());
      npc->setPosition(mp->position());

      npc->setMediaState(bb::multimedia::MediaState::Started);
      npc->setMetaData(mp->metaData());
   }

   void NowPlayingNDK::stop(){
       mp->stop();
   }

   void NowPlayingNDK::pause(){
         mp->pause();

     }


   void NowPlayingNDK::NowPlayingSetMetadata(const std::string& callbackId, const std::string& data){
              // Parse the arg string as JSON
              Json::FastWriter writer;
              Json::Reader reader;
              Json::Value root;
              bool parse = reader.parse(data, root);

              if (!parse) {
                  Json::Value error;
                  error["result"] = "Cannot parse JSON object";
                  sendEvent(callbackId + " " + writer.write(error));
              } else {
                  QVariantMap metadata;

                  metadata[MetaData::Title] = QString::fromStdString(root["Title"].asString());
                  metadata[MetaData::Artist] = QString::fromStdString(root["Artist"].asString());
                  metadata[MetaData::Album] = QString::fromStdString(root["Album"].asString());


                  root["result"] = "SetMetadata Succeed.";
                  sendEvent(callbackId + " " + writer.write(root));
              }

      }
} /* namespace webworks */
