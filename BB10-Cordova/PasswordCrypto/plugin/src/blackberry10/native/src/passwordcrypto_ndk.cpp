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
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "passwordcrypto_ndk.hpp"
#include "passwordcrypto_js.hpp"
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>

namespace webworks {

PasswordCryptoNDK::PasswordCryptoNDK(PasswordCryptoJS *parent) {
	m_pParent = parent;
}

PasswordCryptoNDK::~PasswordCryptoNDK() {
}

std::string PasswordCryptoNDK::ping() {
	return "pong";
}

std::string uchar2hex(unsigned char inchar)
{
  ostringstream oss (ostringstream::out);
  oss << setw(2) << setfill('0') << hex << (int)(inchar);
  return oss.str();
}

std::string getStdStr(unsigned char *str, int keyLength){
    std::string result = "";
    int lengthOfString = strlen( reinterpret_cast<const char*>(str) );

    std::string s;
    for( int i = 0; i < keyLength; i++ ){
        s = uchar2hex(str[i]);

        // account for single-digit hex values (always must serialize as two digits)
        result.append( s );
    }

    return result;


}
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void PasswordCryptoNDK::pbkdf2_Async(const std::string& callbackId, const std::string& inputString) {
		const std::string sOutput=pbkdf2_Sync(inputString);
		m_pParent->NotifyEvent(callbackId + " " + sOutput);
}


std::string PasswordCryptoNDK::pbkdf2_Sync(const std::string& jsonArgs){
	clock_t start = clock(); // START Stopwatch - useful to determine iterations count


    Json::StyledWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(jsonArgs, root);

	// ----- Error parameters checking below ------
	if (!parse) {
		Json::Value error;
		error["result"] = "Error: Cannot parse JSON params object - check your params with jsonlint.com : "+jsonArgs;
		error["success"] = false;
	    std::string retErr = writer.write( error );

	    return retErr;
	}
	if(!root["password"] || !root["salt"] || !root["iterations"] || !root["keyLength"]){
		Json::Value error;
		error["result"] = "Error: Params missing. Required params are - password, salt, iterations, and keyLength";
		error["success"] = false;
	    std::string retErr = writer.write( error );

	    return retErr;
	}
	// ----- Error parameters checking above ------


	std::string passwdStr = root["password"].asString();
	std::string saltStr = root["salt"].asString();
	std::string strKdfValue; // this will be the output of the pbkdf2 hash

	// parse the iterations parameter - work with if it's a string or an int
	// but only parse string as long as what's passed is a number
	int iterations=-1;
	if(root["iterations"].isInt()){
		iterations=root["iterations"].asInt();
	}
	else if(root["iterations"].isString()){
		std::string iterationsStr = root["iterations"].asString();
		if(!is_number(iterationsStr)){
			Json::Value error;
			error["result"] = "Error: Was expecting a number for iterations.";
			error["success"] = false;
		    std::string retErr = writer.write( error );

		    return retErr;
		}
		iterations = atoi(iterationsStr.c_str());

	}

	// parse the keylength parameter - work with if it's a string or an int
	// but only parse string as long as what's passed is a number
	int keyLength=-1;
	if(root["keyLength"].isInt()){
		keyLength=root["keyLength"].asInt();
	}
	else if(root["keyLength"].isString()){
		std::string keyLengthStr = root["keyLength"].asString();
		if(!is_number(keyLengthStr)){
			Json::Value error;
			error["result"] = "Error: Was expecting a number for Key Length.";
			error["success"] = false;
		    std::string retErr = writer.write( error );

		    return retErr;
		}
		keyLength = atoi(keyLengthStr.c_str());
	}

	if(keyLength < 1 || iterations < 1){
		Json::Value error;
		error["result"] = "Error: parameters passed into keyLength or iterations are invalid.";
		error["success"] = false;
	    std::string retErr = writer.write( error );

	    return retErr;
	}

	try{

		int saltLength = saltStr.length();
		int passwordLength = passwdStr.length();

		const char* passwdCstr = passwdStr.c_str();
		const char* saltCstr = saltStr.c_str();
		unsigned char* saltUCStr;
		saltUCStr = (unsigned char*)saltCstr;
		unsigned char kdfValue[keyLength];

		PKCS5_PBKDF2_HMAC_SHA1(passwdCstr, // password
								passwordLength,  // password.Length
								saltUCStr, // salt
								saltLength, // salt length
								iterations, // iterations
								keyLength, // keylength
								kdfValue); // output of pkdf2

		kdfValue[keyLength]='\0';
		strKdfValue = getStdStr(kdfValue, keyLength);
	}
	catch (int x){
		Json::Value error;
		error["result"] = "Error: PBKDF2 computation failed. Recheck parameters.";
		error["success"] = false;
	    std::string retErr = writer.write( error );

	    return retErr;
	}

	//----------------- SUCCESS -------------
	root["pbkdf2"] = strKdfValue;
	root["success"]=true;
	clock_t runtime_t = clock()-start;
	int runtime = (int)((((float)runtime_t)/CLOCKS_PER_SEC)*1000);

	root["runtime"]= runtime;
	return writer.write(root);
}

}

