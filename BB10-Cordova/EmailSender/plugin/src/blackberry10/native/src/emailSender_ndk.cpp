/*
 * Copyright (c) 2013 - 2014 BlackBerry Limited
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
#include <bb/pim/message/MessageBuilder>
#include <QtCore/QObject>
#include "emailSender_ndk.hpp"
#include "emailSender_js.hpp"


namespace webworks {

EmailSenderNDK::EmailSenderNDK(EmailSenderJS *parent):
	m_pParent(parent){
}

EmailSenderNDK::~EmailSenderNDK() {
}

// Take in input and return a value
std::string EmailSenderNDK::sendEmail(const std::string& inputString) {
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value input;
	bool parse = reader.parse(inputString, input);
	if(parse){
		Json::Value From = input["From"];
		Json::Value To = input["To"];
		Json::Value Cc = input["Cc"];
		Json::Value Bcc = input["Bcc"];
		Json::Value subject = input["subject"];
		Json::Value body = input["body"];

		long id = atol(From.asString().c_str());
		Account account;

		if(id == -1){
			account = accountService.defaultAccount(Service::Messages);
		}
		else{
			account = accountService.account(id);
		}
		if(!account.isValid()) return "The account is not valid.";
		MessageBuilder *builder = MessageBuilder::create(account.id());

		if(To.isArray()){
			foreach(Json::Value v, To){
				QString email =  QString::fromStdString(v.asString());
				MessageContact rto = MessageContact(-1,MessageContact::To,  email, email);
				builder->addRecipient(rto);
			}
		}
		else{
			QString email =  QString::fromStdString(To.asString());
			MessageContact rto = MessageContact(-1,MessageContact::To, email, email);
			builder->addRecipient(rto);
		}

		if(Cc.isArray()){
			foreach(Json::Value v, Cc){
				QString email =  QString::fromStdString(v.asString());
				MessageContact rCc = MessageContact(-1,MessageContact::Cc, email, email);
				builder->addRecipient(rCc);
			}
		}
		else{
			QString email =  QString::fromStdString(Cc.asString());
			MessageContact rCc = MessageContact(-1,MessageContact::Cc, email, email);
			builder->addRecipient(rCc);
		}

		if(Bcc.isArray()){
			foreach(Json::Value v, Bcc){
				QString email =  QString::fromStdString(v.asString());
				MessageContact rBcc = MessageContact(-1,MessageContact::Bcc, email, email);
				builder->addRecipient(rBcc);
			}
		}
		else{
			QString email =  QString::fromStdString(Bcc.asString());
			MessageContact rBcc = MessageContact(-1,MessageContact::Bcc, email, email);
			builder->addRecipient(rBcc);
		}

		builder->subject(QString::fromStdString(subject.asString()));
		string stringBody = body.asString();
		QByteArray bodyData ;
		bodyData.append(QString::fromStdString(body.asString()));
		builder->body(MessageBody::PlainText,bodyData);
		Message m = *builder;
		MessageKey mk = messageService.send (m.accountId(), m );

		if(mk == 0){
			return "The email has not been sent.";
		}
		else{
			return "Sent.";
		}
	}
	else{
		return "The JSON can't be parse.";
	}

	return "An error as occurred.";
}

std::string EmailSenderNDK::getEmailAccounts(){
	QList<Account> accounts = accountService.accounts(Service::Messages);
	std::string accountsArray = "{" ;
	foreach(Account account, accounts){
		//To only select the email address.
		if(account.settingsProperty("email_address").value<QString>().toStdString().find("@") == std::string::npos) continue;
		//To convert the AccounKey to string
		stringstream idString;
		idString << account.id();
		accountsArray += "\"" + idString.str() + "\":\""+ account.settingsProperty("email_address").value<QString>().toStdString() + "\",";

	}
	accountsArray.resize(accountsArray.size()-1);
	return accountsArray + "}";
}

} /* namespace webworks */
