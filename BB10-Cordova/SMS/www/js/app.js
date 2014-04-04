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
 
 
var app = {
	// Application Constructor
	initialize: function() {
		this.bindEvents();
	},
	// Bind Event Listeners
	//
	// Bind any events that are required on startup. Common events are:
	// 'load', 'deviceready', 'offline', and 'online'.
	bindEvents: function() {
		document.addEventListener('deviceready', this.onDeviceReady, false);
	},
	// deviceready Event Handler
	//
	// The scope of 'this' is the event. In order to call the 'receivedEvent'
	// function, we must explicity call 'app.receivedEvent(...);'
	onDeviceReady: function() {
		app.receivedEvent('deviceready');
	},
	// Update DOM on a Received Event
	receivedEvent: function(id) {
	},
	sendSMS: function(){
		app.removeResults();
		var phoneNumber = document.getElementById('phoneNumber').value;
		var textMsg = document.getElementById('textMsg').value;	
		if(phoneNumber.length<3 || textMsg.length==0){
			document.getElementById("results").innerHTML="Error: Both the Phone Number and the Text Message needs to be entered";
			return;
		}
		
		
		console.log("Phone Number:"+phoneNumber);
		console.log("Message:"+textMsg);

		
		bbSMS.sendAsync(app.smsCallback, phoneNumber, textMsg); // <----- THIS Calls bbSMS.js file. Include bbSMS.js in your project. 
		
	},
	smsCallback: function(output){
		if(output===false){
			document.getElementById("results").innerHTML="Error: Could find the SMSPlugin. Re-Check your installation";
			return;
		}
		app.displayResult(output);
		
	},
	displayResult: function(result){
	
		document.getElementById("results").innerHTML=			 "<br><b>Success:</b> "+result.success	
																+"<br><b>Account ID:</b> "+result.smsAccountID
																+"<br><b>Recipient Address:</b> "+result.recipientAddress
																+"<br><b>Conversation ID:</b> "+result.conversationId				
																+"<br><b>Message ID:</b> "+result.messageId
																+"<br><b>Message:</b> "+result.message;
	},
	removeResults: function(){
		document.getElementById("results").innerHTML="Attempting to Send...";
	}
};