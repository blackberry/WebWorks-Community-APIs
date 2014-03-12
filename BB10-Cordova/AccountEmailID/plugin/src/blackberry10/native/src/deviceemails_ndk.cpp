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

#include <string>
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "deviceemails_ndk.hpp"
#include "deviceemails_js.hpp"
#include <QDir>
#include <QObject>
#include <qobjectdefs.h>
#include <bb/pim/account/AccountService.hpp>
#include <bb/pim/account/Account.hpp>


using namespace bb::pim::account;


namespace webworks {

DeviceEmailsNDK::DeviceEmailsNDK(DeviceEmailsJS *parent):
	m_pParent(parent),
	templateProperty(50),
	templateThreadCount(1),
	threadHalt(true),
	m_thread(0) {
		pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		m_pParent->getLog()->info("Template Created");
		m_accountService = new AccountService();

}

DeviceEmailsNDK::~DeviceEmailsNDK() {
}

std::string DeviceEmailsNDK::ping(){
	// simple test to ensure we have connectivity to the extension
	return "pong!";
}

std::string DeviceEmailsNDK::version(){
	// simple test to ensure we have connectivity to the extension
	return "0.0.2";
}


std::string DeviceEmailsNDK::getEmailAddresses() {
	Json::StyledWriter writer;
	Json::Reader reader;
	Json::Value root;

	try{
		QList<Account> accountList = m_accountService->accounts(Service::Messages);

		QListIterator<Account> iter(accountList);
		Json::Value jsonAccountList;


		int i=0;
		while(iter.hasNext()){
			Json::Value val;
			Account eachAccount = iter.next();
			std::string emailAddress = eachAccount.settingsProperty("email_address").value<QString>().toStdString();

			val["emailAddress"]=emailAddress;
			val["isEnterprise"]=(eachAccount.isEnterprise())?true:false;

			std::size_t found = emailAddress.find('@'); // this is necessary to strip out PIN address
			if (found!=std::string::npos) {
				jsonAccountList.append(val);
				i++;
			}
		}
		if(i>0){
			root["accounts"]=jsonAccountList;
			root["success"]=true;
		}
		else{
			root["success"]=false; // no emails exist
		}
	}
	catch(int e){
		root["success"]=false;
	}
    std::string myJson = writer.write( root );
    return myJson;

}


// These methods are the true native code we intend to reach from WebWorks
std::string DeviceEmailsNDK::getDefaultEmailAddress() {
	m_pParent->getLog()->debug("DeviceEmailsNDK.getDefaultEmailAddress");

	Json::StyledWriter writer;
	Json::Reader reader;
	Json::Value root;

	Account dAccount = m_accountService->defaultAccount(Service::Messages);
	QString emailAddress = dAccount.settingsProperty("email_address").value<QString>();
	std::string sEmail = emailAddress.toStdString();

	std::size_t found = sEmail.find('@');


	if(sEmail.length()>0 && found!=std::string::npos){
		root["success"]= true;
		root["defaultemail"]=sEmail;
		root["isEnterprise"]=(dAccount.isEnterprise())?true:false;
	}
	else{
		root["success"]= false;
	}
    std::string myJson = writer.write( root );
    return myJson;

}



} /* namespace webworks */
