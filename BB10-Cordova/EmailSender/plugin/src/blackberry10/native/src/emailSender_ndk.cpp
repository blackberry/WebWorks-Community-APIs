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
#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include <bb/pim/message/MessageBuilder>
#include <QtCore/QObject>
#include <QFileInfo>
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
		Json::Value Type = input["Type"];
		Json::Value From = input["From"];
		Json::Value To = input["To"];
		Json::Value Cc = input["Cc"];
		Json::Value Bcc = input["Bcc"];
		Json::Value subject = input["subject"];
		Json::Value body = input["body"];
		Json::Value signature = input["signature"];
		Json::Value attachment = input["attachment"];

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

        std:string msgBody = body.asString();

		if(signature.asString().compare("true") == 0){
		    std::string senderName = account.settingsProperty("email_address").value<QString>().toStdString();
		    std::string sn = senderName.substr(0, senderName.find("@"));

		    std::string signat = "./app/native/res/signature/" + sn + ".vcf";
		    QString vCard = QString::fromStdString(signat);
            QUrl filepath(vCard);
            QFileInfo fileinfo(vCard);

            if(fileinfo.exists()){
                QString filename = fileinfo.fileName();
                QString filetype = fileinfo.completeSuffix();
                Attachment msgAttach(filetype, filename, filepath);
                builder->addAttachment(msgAttach);
            }
            else{
                signat = "./app/native/res/signature/" + sn + ".txt";
                QString eSig = QString::fromStdString(signat);
                QUrl filepath(eSig);
                QFileInfo fileinfo(eSig);

                if(fileinfo.exists()){
                    std::string line = "";
                    msgBody += '\n';
                    std::ifstream file(signat.c_str());
                    while(getline(file, line)){
                        msgBody += line + '\n';
                    }
                    file.close();
                }
                else{
                    signat = "The signature file does not exist.";
                    char * cnofile = new char[signat.length()+1];
                    std::strcpy(cnofile, signat.c_str());
                    m_pParent->getLog()->info(cnofile);
                }
            }
		}
		
		QByteArray bodyData ;
		bodyData.append(QString::fromStdString(msgBody));
        if(Type=="html"){
            builder->body(MessageBody::Html, bodyData);
        }
        else{
            builder->body(MessageBody::PlainText, bodyData);
        }

        if (!attachment.empty()){
            foreach(Json::Value v, attachment){
				// remove any leading or trailing spaces from a file path
                string checkpath = v.asString();
                char ws = ' ';
                checkpath = checkpath.erase(0, checkpath.find_first_not_of(ws));
                checkpath = checkpath.erase(checkpath.find_last_not_of(ws) + 1);

                // allow for full or partial path to be entered
				QString forChecking = QString::fromStdString(checkpath);
				QString path;

				if(forChecking.startsWith(QString::fromStdString("file:///")) || forChecking.startsWith(QString::fromStdString("./"))){
					path = forChecking;
				}
				else if(forChecking.startsWith(QString::fromStdString("/accounts/"))){
					path = QString::fromStdString("file://") + forChecking;
				}
				else if(forChecking.startsWith(QString::fromStdString("/res/"))){
					path = QString::fromStdString("./app/native/") + forChecking;;
				}
				else{
					path = forChecking;
				}

                QUrl filepath(path);
                QFileInfo fileinfo(path);

				if(!fileinfo.exists()){
					string nofile = "The file " + checkpath + " cannot be found";
					char * cnofile = new char[nofile.length()+1];
					std::strcpy(cnofile, nofile.c_str());
					m_pParent->getLog()->info(cnofile);
					continue;
				}
				else{
					QString filename = fileinfo.fileName();
					QString filetype = fileinfo.completeSuffix();
					Attachment msgAttach(filetype, filename, filepath);
					builder->addAttachment(msgAttach);
				}
            }
        }

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
		return "The JSON can't be parsed.";
	}

	return "An error as occurred.";
}

std::string EmailSenderNDK::getEmailAccounts(){
	QList<Account> accounts = accountService.accounts(Service::Messages);
	std::string accountsArray = "{";
	foreach(Account account, accounts){
		//To only select the email address.
		if(account.settingsProperty("email_address").value<QString>().toStdString().find("@") == std::string::npos) continue;
		//To convert the AccounKey to string
		stringstream idString;
		idString << account.id();
		accountsArray += "\"" + idString.str() + "\":\"" + account.settingsProperty("email_address").value<QString>().toStdString() + "\",";
	}
	accountsArray.resize(accountsArray.size()-1);
	return accountsArray + "}";
}

} /* namespace webworks */
