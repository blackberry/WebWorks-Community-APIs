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
		app.receivedEvent('deviceready');
	},
	// Update DOM on a Received Event
	receivedEvent: function(id) {
		var parentElement = document.getElementById(id);
		var listeningElement = parentElement.querySelector('.listening');
		var receivedElement = parentElement.querySelector('.received');

		listeningElement.setAttribute('style', 'display:none;');
		receivedElement.setAttribute('style', 'display:block;');

		console.log('Received Event: ' + id);
		app.testPluginCalls();
	},
	testPluginCalls: function() {
		if (community && community.emailsenderplugin) {
			var statusSpan = document.getElementById('status');
			var emailSelect = document.getElementById('emailSelect');
			
			var accounts = JSON.parse(community.emailsenderplugin.getEmailAccounts());
			var accountsOptions = "";
			for(id in accounts){
				accountsOptions += "<option value=\"" + id + "\">" + accounts[id] + "</option>";
			}
			emailSelect.innerHTML = accountsOptions;

			document.getElementById('send').onclick=function(){

				statusSpan.innerHTML = "Sending...";

				if(document.getElementById('removeTags').checked)
					document.getElementById('removeTags').value = "true";
				else
					document.getElementById('removeTags').value = "false";

				if(document.getElementById('vCard').checked)
					document.getElementById('vCard').value = "true";
				else
					document.getElementById('vCard').value = "false";

				if(document.getElementById('signature').checked)
					document.getElementById('signature').value = "true";
				else
					document.getElementById('signature').value = "false";

				if(document.getElementById('attachment').checked)
					document.getElementById('attachment').value = "true";
				else
					document.getElementById('attachment').value = "false";

				if(document.getElementById('nonfile').checked)
					document.getElementById('nonfile').value = "true";
				else
					document.getElementById('nonfile').value = "false";

				var jsonEmail =
				{
					"Type": document.getElementById('emailType').value,
					"tags": document.getElementById('removeTags').value,
					"From": document.getElementById('emailSelect').value, 
					"To": document.getElementById('To').value,
					"Cc": document.getElementById('Cc').value,
					"Bcc": document.getElementById('Bcc').value,
					"subject" : document.getElementById('Subject').value,
					"body": "<pre>" + document.getElementById('Body').value + "</pre>",
					"vCard": document.getElementById('vCard').value,
					"vCardLocation": document.getElementById('vCardLocation').value.split(','),
					"signature": document.getElementById('signature').value,
					"signatureLocation": document.getElementById('signatureLocation').value.split(','),
					"attachment": document.getElementById('attachment').value,
					"attachmentLocation": document.getElementById('attachmentLocation').value.split(','),
					"nonfile": document.getElementById('nonfile').value,
					"nonfilename": document.getElementById('nonfilename').value,
					"nonfiledata": document.getElementById('nonfiledata').value
				};
				statusSpan.innerHTML = community.emailsenderplugin.sendEmail(jsonEmail);
			};
		} else {
			app.writeOut("Plugin was not found");
		}
	}
};
