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

#ifndef _MEDIAKEYS_NDK_HPP_
#define _MEDIAKEYS_NDK_HPP_

#include <string>
#include <vector>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QtCore>
#include "Logger.hpp"

#include <bb/multimedia/MediaKey>
#include <bb/multimedia/MediaKeyWatcher>
#include <bb/multimedia/MediaKeyNotification>

class MediaKeysJS;

namespace webworks {

	using namespace bb::multimedia;
	using namespace std;

	class MediaKeysNDK;

	class MediaKeysHandler : public QObject {

		Q_OBJECT

		MediaKeysNDK * m_parentNDK;

		string m_callbackId;

		MediaKeyWatcher *m_mediaKeyWatcher;

	public:

		MediaKeysHandler(MediaKeysNDK * parentNDK, string callbackId, MediaKeyWatcher *mediaKeyWatcher, string mediaKey, string keyLength) :
				m_parentNDK(parentNDK), m_callbackId(callbackId), m_mediaKeyWatcher(mediaKeyWatcher), m_mediaKey(mediaKey), m_keyLength(keyLength) {}

	public slots:
		Q_SLOT void onPress(bb::multimedia::MediaKey::Type key);

	private:
		string m_mediaKey;

		string m_keyLength;
	};

	class MediaKeysNDK : public QObject {

		Q_OBJECT

		MediaKeysJS *m_pParent;

		int m_id;

		// <id, mediakeywatcher>
		QHash<int, MediaKeysHandler *> m_mediaKeyHandlerList;

	  public:

		explicit MediaKeysNDK(MediaKeysJS *parent = NULL) :
			m_pParent(parent), m_id(0) {}

		virtual ~MediaKeysNDK() {};

		void sendEvent( const std::string& msg);

		MediaKeysJS * getParent() {
			return this->m_pParent;
		}

		MediaKey::Type getMediaKey(string mediaKeyStr);

	public slots:

		string bind(string callbackId, string inputString);

		void join( string inputString);

		void cleanup();
	};

} // namespace webworks

#endif // _MEDIAKEYS_NDK_HPP_
