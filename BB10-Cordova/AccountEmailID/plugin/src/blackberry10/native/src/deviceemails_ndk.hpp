/*
* Copyright (c) 2014 BlackBerry Limited
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

#ifndef DeviceEmailsNDK_HPP_
#define DeviceEmailsNDK_HPP_

#include <string>
#include <pthread.h>
#include <QObject>
#include <QString>
#include <bb/pim/account/AccountService.hpp>
#include <bb/pim/account/Account.hpp>

using namespace bb::pim::account;

class DeviceEmailsJS;

namespace webworks {

class DeviceEmailsNDK: public QObject {
	/*
	 * Add the Q_OBJECT macro below causes the app to crash at runtime. This macro enables signal and slots.
	 *
	 * Uncomment the Q_OBJECT macro below, compile, and run the app to stop the party.
	 */
	//Q_OBJECT

public:
	explicit DeviceEmailsNDK(DeviceEmailsJS *parent = NULL);
	virtual ~DeviceEmailsNDK();

	// The extension methods are defined here
	std::string getDefaultEmailAddress();
	std::string getEmailAddresses();

	std::string ping();
	std::string version();


private:
	DeviceEmailsJS *m_pParent;
	AccountService *m_accountService;


	int templateProperty;

	int templateThreadCount;
	bool threadHalt;
	std::string threadCallbackId;
	pthread_t m_thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
};

} // namespace webworks

#endif /* DeviceEmailsNDK_H_ */
