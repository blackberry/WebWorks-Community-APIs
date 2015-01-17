/*
* Copyright (c) 2013 BlackBerry Limited
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

#ifndef TEMPLATENDK_HPP_
#define TEMPLATENDK_HPP_

#include <string>
#include <pthread.h>

#include <bb/pim/account/AccountService>
#include <bb/pim/message/MessageService>

class MessageJS;

namespace webworks {

class MessageNDK {
public:
	explicit MessageNDK(MessageJS *parent = NULL);
	virtual ~MessageNDK();

	// The extension methods are defined here
	std::string ping();

	std::string getEmailMessage(const std::string& accountId, const std::string& messageId);


private:
	MessageJS *m_pParent;
	int messageProperty;
	int messageThreadCount;
	bool threadHalt;
	std::string threadCallbackId;
	pthread_t m_thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	bb::pim::message::MessageService* _message_service;
	bb::pim::account::AccountService* _account_service;
};

} // namespace webworks

#endif /* TEMPLATENDK_H_ */
