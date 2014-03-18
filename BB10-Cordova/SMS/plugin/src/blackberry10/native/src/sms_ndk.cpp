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

#include <string>
#include <sstream>
#include <QString>
#include <QDir>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "sms_ndk.hpp"
#include "sms_js.hpp"


#include <bb/pim/account/Provider>
#include <bb/pim/account/Result>
#include <bb/pim/message/MessageService>
#include <bb/pim/message/Message>
#include <bb/pim/message/MessageBuilder>
#include <bb/pim/message/ConversationBuilder>
#include <bb/pim/message/Keys>
#include <bb/pim/message/StatusReport>
#include <bb/pim/message/MessageUpdateType>
#include <bb/pim/message/MimeTypes>
#include <bb/pim/message/SmsTransportSendResult>

#include <QSettings>
#include <QDir>

using namespace bb::pim::account;
using namespace bb::pim::message;

const char *TEXT = "text/*";
const char *TEXT_HTML = "text/html";
const char *TEXT_PLAIN = "text/plain";

static QString KEY_PHONE_NUMBER="phone_number";


namespace webworks {

SMS_NDK::SMS_NDK(SMS_JS *parent) {

	_message_service = new MessageService();
	_account_service = new AccountService();
	QList<Account> account_list = _account_service->accounts(Service::Messages, "sms-mms");
	_sms_account_id = account_list.first().id();

	m_pParent = parent;
}

SMS_NDK::~SMS_NDK() {
}
std::string SMS_NDK::sendMessage(const QString & text, const QString & destination_addr, const QString & personName) {
	Json::FastWriter writer;
	Json::Value root;

	if (destination_addr.length() < 3) {
		Json::Value error;
		error["result"] = "Error: Destination address does not make sense. Recheck.";
		error["success"] = false;
	    std::string retErr = writer.write( error );
	    return retErr;
	}
	//qDebug() << "XXXX sendMessage:" << text << " to " << destination_addr;

    QSettings settings;
	settings.setValue(KEY_PHONE_NUMBER,destination_addr);

//	First create a conversation, as SMS messages are dealt with as a conversation.
	//qDebug() << "XXXX sendMessage: creating Conversation";
	ConversationBuilder* conversation_builder = ConversationBuilder::create();
	conversation_builder->accountId(_sms_account_id);
	QList<MessageContact> participants;

	//qDebug() << "XXXX sendMessage: creating MessageContact";
	MessageContact recipient = MessageContact(-1, MessageContact::To, destination_addr, personName);
	//qDebug() << "XXXX sendMessage: adding recipient";
	participants.append(recipient);
// indicate who the conversation is with
	conversation_builder->participants(participants);

//obtain the conversation and save it
	Conversation conversation = *conversation_builder;
	ConversationKey conversation_id = _message_service->save(_sms_account_id, conversation);

// make a Message object
	//qDebug() << "XXXX sendMessage: creating MessageBuilder";
	MessageBuilder* msg_builder = MessageBuilder::create(_sms_account_id);

	//qDebug() << "XXXX sendMessage: setting body";
	msg_builder->addAttachment(Attachment("text/plain", "", text));
	//qDebug() << "XXXX sendMessage: recipient=" << recipient.address();
	msg_builder->addRecipient(recipient);

	//qDebug() << "XXXX sendMessage: setting conversation ID";
	msg_builder->conversationId(conversation_id);

	Message message;
	message = *msg_builder;

// Send it using the SMS MMS account
	//qDebug() << "XXXX sendMessage: sending via account id " << _sms_account_id << " : " << message.recipientAt(0).address();
	QString recipientAddress = QString(message.recipientAt(0).address());

	MessageKey msg_id = _message_service->send(_sms_account_id, message);

	root["smsAccountID"] = _sms_account_id;
	root["success"]=true;
	root["recipientAddress"]= recipientAddress.toStdString();
	root["message"]= text.toStdString();
	root["conversationId"]= conversation_id.toStdString();
	root["messageId"]= QString::number(msg_id).toStdString();

	return writer.write(root);

}

// These methods are the true native code we intend to reach from WebWorks
std::string SMS_NDK::ping() {
	return "pong";
}

// Take in input and return a value
std::string SMS_NDK::udSMS_Sync(const std::string& inputString) {
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		Json::Value error;
		error["result"] = "Cannot parse JSON object. SMS Not sent! --> "+inputString;
		error["success"] = false;
		return writer.write(error);


	} else {
		QString msgQStr = QString(root["message"].asCString());
		QString destinationQStr = QString(root["destination"].asCString());
		QString personNameQStr = QString(root["personName"].asCString());
		return sendMessage(msgQStr, destinationQStr, personNameQStr);
	}
}

//

// Asynchronous callback with JSON data input and output
void SMS_NDK::udSMS_Async(const std::string& callbackId, const std::string& inputString) {
	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		Json::Value error;
		error["result"] = "Cannot parse JSON object. SMS Not sent! --> "+inputString;
		error["success"] = false;
		m_pParent->NotifyEvent(callbackId + " " + writer.write(error));
	} else {
		QString msgQStr = QString(root["message"].asCString());
		QString destinationQStr = QString(root["destination"].asCString());
		QString personNameQStr = QString(root["personName"].asCString());

		std::string sOutput =  sendMessage(msgQStr, destinationQStr, personNameQStr);
		m_pParent->NotifyEvent(callbackId + " " + sOutput);
	}
}




} /* namespace webworks */
