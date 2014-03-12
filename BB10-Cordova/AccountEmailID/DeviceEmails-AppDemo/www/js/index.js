/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
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
		var output= "";
		if (community && community.deviceemails) {
			var pluginDefaultEmail = community.deviceemails.getDefaultEmailAddress();
			var pluginAccountEmails = community.deviceemails.getEmailAddresses();

			if(pluginDefaultEmail.success==true){
				console.log(pluginDefaultEmail.defaultemail);
				
				document.getElementById('results').innerHTML=pluginDefaultEmail.defaultemail;
			}
			else if(pluginDefaultEmail.success==false){
				var error = "I could not find any default email address. Is this because there are no emails accounts setup on this device?";
				console.log(error);
				document.getElementById('results').innerHTML=error;				
			}					

			output = JSON.stringify(pluginDefaultEmail);

		} else {
			output = "Plugin was not found! ";
		}
		document.getElementById('defaultemailJson').innerHTML=output;
		document.getElementById('AccountsJson').innerHTML= JSON.stringify(pluginAccountEmails);
	}
};
