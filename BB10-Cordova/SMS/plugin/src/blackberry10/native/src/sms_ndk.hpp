/*
* Copyright 2014 BlackBerry Limited.
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

#ifndef SMSNDK_HPP_
#define SMSNDK_HPP_

#include <string>
#include <pthread.h>
#include <QChar>
#include <QObject>
#include <bb/pim/account/AccountService>
#include <bb/pim/account/Account>
#include <bb/pim/message/Keys>
#include <bb/pim/message/MessageUpdate>
#include <bb/pim/message/MessageService>

#include <qobjectdefs.h>

class SMS_JS;

namespace webworks {

class SMS_NDK {
public:
	explicit SMS_NDK(SMS_JS *parent = NULL);
	virtual ~SMS_NDK();

	// The extension methods are defined here

	std::string ping(); // this method exists just to check for connectivity to this extension
	std::string sendMessage(const QString & text, const QString & destination_addr, const QString & personName);
	std::string udSMS_Sync(const std::string& inputString);
	void udSMS_Async(const std::string& callbackId, const std::string& inputString);


private:
	SMS_JS *m_pParent;
	int templateProperty;
	int templateThreadCount;
	bool threadHalt;
	std::string threadCallbackId;
	pthread_t m_thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	bb::pim::account::AccountService* _account_service;
	bb::pim::message::MessageService* _message_service;
	int _sms_account_id;

	void sendMessage(const QString & text, const QString & destination_addr);

};

} // namespace webworks

#endif /* TEMPLATENDK_H_ */
