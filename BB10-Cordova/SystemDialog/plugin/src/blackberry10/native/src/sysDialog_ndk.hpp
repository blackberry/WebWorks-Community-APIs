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

#ifndef _SYSDILOG_NDK_HPP_
#define _SYSDILOG_NDK_HPP_

#include <string>
#include <vector>
#include <QObject>
#include <QString>
#include <QtCore>

#include <bb/system/SystemUiButton>
#include <bb/platform/NotificationDialog>
#include <bb/platform/NotificationResult>

class SysDialogJS;

namespace webworks {

	using namespace bb::platform;
	using namespace bb::system;
	using namespace std;

	class SysDialogNDK : public QObject {

	    Q_OBJECT

		SysDialogJS *m_pParent;

// TODO: what if multiply dialogs are invoked simultaneously? use vector<dialog>?
// seems cannot delete before finished(). Also it seems new dialog is not popped
// before old one cancelled... maybe its ok, later show() is bloked...

		NotificationDialog * m_notificationDialog;
		string m_callbackId;

		// store allocated SystemUiButton, used to calculate button index
		SystemUiButton ** m_buttonList;

		// map enum NotificationResult to error message to return
		static map<NotificationError::Type, string> m_errorMessage;

		// init m_errorMessage
		void initErrorMsg();

		// delete buttons & m_notificationDialog
		void cleanUp();

	  public:

		explicit SysDialogNDK(SysDialogJS *parent = NULL) :
		m_pParent(parent), m_buttonList(NULL) { initErrorMsg();}

		virtual ~SysDialogNDK();

		// create a NotificationDialog according to inputString & show
		Q_INVOKABLE string show(std::string& callbackId, const std::string& inputString);

	  public slots:

		// slot for finished(); have to be class member; have to include the complete namespace
		// to match finished()'s signature
		void onDialogFinished (bb::platform::NotificationResult::Type value);
	};

} // namespace webworks

#endif // _SYSDILOG_NDK_HPP_
