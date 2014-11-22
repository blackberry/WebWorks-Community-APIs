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

#include <string>
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "message_ndk.hpp"
#include "message_js.hpp"

#include <bb/pim/account/Account>
#include <bb/pim/account/AccountService>
#include <bb/pim/message/Message>
#include <bb/pim/message/MessageService>

using namespace bb::pim::account;
using namespace bb::pim::message;

namespace webworks {

MessageNDK::MessageNDK(MessageJS *parent) :
		m_pParent(parent), messageProperty(50), messageThreadCount(1), threadHalt(
				true), m_thread(0) {
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	m_pParent->getLog()->info("MessagePlugin Created");
	_message_service = new MessageService();
	_account_service = new AccountService();
}

MessageNDK::~MessageNDK() {
}

// These methods are the true native code we intend to reach from WebWorks
std::string MessageNDK::ping() {
	m_pParent->getLog()->debug("ping-pong");
	return "pong";
}

// Take in input and return a value
std::string MessageNDK::getEmailMessage(const std::string& accountId,
		const std::string& messageId) {
	Json::FastWriter writer;
	Json::Value root;

	AccountKey accountKey = atoll(accountId.c_str());
	MessageKey messageKey = atoll(messageId.c_str());

	const Message message = _message_service->message(accountKey, messageKey);

	QString m_body = message.body(MessageBody::PlainText).plainText();
	if (m_body.isEmpty())
		m_body = message.body(MessageBody::Html).plainText();
	root["subject"] = message.subject().toStdString();
	root["sender"] = message.sender().displayableName().toStdString();
	root["senderAddress"] = message.sender().address().toStdString();
	root["body"] = m_body.toStdString();

	return writer.write(root);

}

} /* namespace webworks */
