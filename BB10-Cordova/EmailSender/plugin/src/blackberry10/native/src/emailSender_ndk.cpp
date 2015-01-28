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
#include <QtCore/QObject>
#include <QFileInfo>
#include <QFile>
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
		Json::Value tags = input["tags"];
		Json::Value From = input["From"];
		Json::Value To = input["To"];
		Json::Value Cc = input["Cc"];
		Json::Value Bcc = input["Bcc"];
		Json::Value subject = input["subject"];
		Json::Value body = input["body"];
		Json::Value vCard = input["vCard"];
		Json::Value vCardLocation = input["vCardLocation"];
		Json::Value signature = input["signature"];
        Json::Value signatureLocation = input["signatureLocation"];
		Json::Value attachment = input["attachment"];
        Json::Value attachmentLocation = input["attachmentLocation"];
        Json::Value nonfile = input["nonfile"];
        Json::Value nonfilename = input["nonfilename"];
        Json::Value nonfiledata = input["nonfiledata"];

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

        std::string msgBody = body.asString();

		if(Type != "html" && tags.asString().compare("true") == 0){
		    msgBody = stripHtml(msgBody); // strip tags for text messages
		}

		QByteArray bodyData ;
		bodyData.append(QString::fromStdString(msgBody));
        if(Type=="html"){
            builder->body(MessageBody::Html, bodyData);
        }
        else{
            builder->body(MessageBody::PlainText, bodyData);
        }

        if (vCard.asString().compare("true") == 0){
            if (!vCardLocation.empty()){
                foreach(Json::Value v, vCardLocation){
                    string checkpath = v.asString();
                    QString path = checkPath(checkpath);
                    attachFile(*builder, path);
                }
            }
            else{
                m_pParent->getLog()->info("No file path entered");
            }
        }

        if (signature.asString().compare("true") == 0){
            if (!signatureLocation.empty()){
                foreach(Json::Value v, signatureLocation){
                    string checkpath = v.asString();
                    QString path = checkPath(checkpath);
                    attachFile(*builder, path);
                }
            }
            else{
                m_pParent->getLog()->info("No file path entered");
            }
        }

        if (attachment.asString().compare("true") == 0){
            if (!attachmentLocation.empty()){
                foreach(Json::Value v, attachmentLocation){
                    string checkpath = v.asString();
                    QString path = checkPath(checkpath);
                    attachFile(*builder, path);
                }
            }
            else{
                m_pParent->getLog()->info("No file path entered");
            }
        }

        if (nonfile.asString().compare("true") == 0){
            QString nofilename;
            QString nofiletype;
            QByteArray nofiledata;
            if (!nonfiledata.empty()){
                nofiledata.append(QString::fromStdString(nonfiledata.asString()));
                if (!nonfilename.empty()){
                    QFile thisfilename(QString::fromStdString(nonfilename.asString()));
                    QFileInfo thisfileinfo(thisfilename);
                    nofilename = thisfileinfo.fileName();
                    nofiletype = thisfileinfo.completeSuffix();
                }
                else{
                    nofilename = "filename";
                    nofiletype = "";
                }
                Attachment msgAttach(nofiletype, nofilename, nofiledata);
                builder->addAttachment(msgAttach);
            }
            else{
                m_pParent->getLog()->info("No data received");
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

std::string EmailSenderNDK::stripHtml(std::string msgBody){
    std::vector<std::string> stripped;
    for(;;){
        std::string::size_type startpos;
        startpos = msgBody.find('<');
        if(startpos == std::string::npos){
            stripped.push_back(msgBody);
            break;
        }
        if(0 != startpos){
            stripped.push_back(msgBody.substr(0, startpos));
            msgBody = msgBody.substr(startpos, msgBody.size() - startpos);
            startpos = 0;
        }
        std::string::size_type endpos;
        for(endpos = startpos; endpos < msgBody.size() && msgBody[endpos] != '>'; ++endpos){
            if(msgBody[endpos] == '"'){
                endpos++;
                while(endpos < msgBody.size() && msgBody[endpos] != '"'){
                    endpos++;
                }
            }
        }
        if(endpos == msgBody.size()){
            msgBody = msgBody.substr(endpos, msgBody.size() - endpos);
            break;
        }
        else{
            endpos++;
            msgBody = msgBody.substr(endpos, msgBody.size() - endpos);
        }
    }
    msgBody="";
    for(size_t i=0; i < stripped.size(); i++){
        msgBody += stripped[i];
    }
    return msgBody;
}

QString EmailSenderNDK::checkPath(std::string checkpath){
    // remove any leading or trailing spaces from a file path
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
    return path;
}


void EmailSenderNDK::attachFile(MessageBuilder& builder, QString path){
    QUrl filepath(path);
    QFileInfo fileinfo(path);

    if(!fileinfo.exists()){
        string nofile = "The file " + path.toStdString() + " cannot be found";
        char * cnofile = new char[nofile.length()+1];
        std::strcpy(cnofile, nofile.c_str());
        m_pParent->getLog()->info(cnofile);
    }
    else{
        QString filename = fileinfo.fileName();
        QString filetype = fileinfo.completeSuffix();
        Attachment msgAttach(filetype, filename, filepath);
        builder.addAttachment(msgAttach);
    }
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
