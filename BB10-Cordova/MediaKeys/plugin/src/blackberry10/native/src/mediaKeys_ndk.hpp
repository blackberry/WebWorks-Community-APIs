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

#include <bb/system/SystemUiButton>
#include <bb/platform/NotificationDialog>
#include <bb/platform/NotificationResult>

#include <bb/multimedia/MediaKey>
#include <bb/multimedia/MediaKeyWatcher>
#include <bb/multimedia/MediaKeyNotification>

class MediaKeysJS;

namespace webworks {

	using namespace bb::platform;
	using namespace bb::system;
	using namespace std;

	using namespace bb::multimedia;

	class MediaKeysNDK;

	class DialogHandler : public QObject {

		Q_OBJECT
		MediaKeysNDK * m_parentNDK;

		string m_callbackId;
		NotificationDialog * m_notificationDialog;

	  public:

		DialogHandler( MediaKeysNDK * parentNDK, string callbackId, NotificationDialog * dialog) :
		m_parentNDK(parentNDK), m_callbackId(callbackId), m_notificationDialog(dialog) { }

	  public slots:

  		// return m_notificationDialog->error as string
		string getError();

		// call m_notificationDialog->show() and return error
		string show();

		// slot for finished(); have to be class member; have to include the complete namespace
		Q_SLOT void onDialogFinished (bb::platform::NotificationResult::Type value);
	};


	class MediaKeysNDK : public QObject {

	    Q_OBJECT

		MediaKeysJS *m_pParent;

		int m_id;

		// multiply dialogs can be invoked simultaneously
		QHash<int, DialogHandler*> m_dialogHandlerList;

		MediaKeyWatcher *keyWatcher;

	  public:

		explicit MediaKeysNDK(MediaKeysJS *parent = NULL) :
			m_pParent(parent), m_id(0), success(false), changed(false) {

		}

		virtual ~MediaKeysNDK() {};

		void sendEvent( const std::string& msg);

	public slots:

	    string bind(string arg);

	    string checkVolume(string arg);

	    Q_SLOT void onShortPress(bb::multimedia::MediaKey::Type key);

		void join( string inputString);

		string create(string callbackId, string inputString);
		string show(string id);

		void cleanup();

	private:
		bool success;

		bool changed;
	};



} // namespace webworks

#endif // _MEDIAKEYS_NDK_HPP_
