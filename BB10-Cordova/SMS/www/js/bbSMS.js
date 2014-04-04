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


var bbSMS = {

	// usage --> bbSMS.send("2123332222", "What's up doc?", "Elmer Fudd");
	send: function(phoneNumber, textMsg, personName){
		console.log('trying to send an sms');
		if(personName===undefined){
			personName=phoneNumber;
		}

		if (community && community.SMSPlugin) {
			var param = {
				"destination": phoneNumber,  
				"message": textMsg,
				"personName": personName // can be any string such as "Barack Obama" 
			};
			console.log(param);
			var output = community.SMSPlugin.udSMS_Sync(param); // <------- this is the call to C++ implementation. That's all!
			return output;
			
		} else {
			return false;
		}

	},
	sendAsync: function(callback, phoneNumber, textMsg, personName){
		console.log('trying to send an sms');
		if(personName===undefined){
			personName=phoneNumber;
		}

		if (community && community.SMSPlugin) {
			var param = {
				"destination": phoneNumber,  
				"message": textMsg,
				"personName": personName // can be any string such as "Barack Obama" 
			};
			console.log(param);
			
			var output = community.SMSPlugin.udSMS_Async(param, callback); // <------- this is the call to C++ implementation. That's all!
			return output;
			
		} else {
			return false;
		}

	}
	
} 
