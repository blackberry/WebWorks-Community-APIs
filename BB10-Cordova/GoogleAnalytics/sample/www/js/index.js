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
		var sError;
		if (community && community.googleanalyticsplugin) {
			app.writeOut("Properties of this plugin:");
			community.googleanalyticsplugin.appName = 'BrandNew_App';
			app.writeOut('AppName: ' + community.googleanalyticsplugin.appName);
			community.googleanalyticsplugin.uuid = ""; // Setting UUID to empty string trigger a random UUID
			app.writeOut('UUID: ' + community.googleanalyticsplugin.uuid);
			app.writeOut('Setting GA_Account number UA-xxxxxxxx-x.');
			community.googleanalyticsplugin.gaAccount = "UA-50848230-1"; // Sign-up for your own account
			app.writeOut('GA_Account: ' + community.googleanalyticsplugin.gaAccount);
			app.writeOut('--------------');
			app.writeOut('Sending track pageview:')
			sError = community.googleanalyticsplugin.trackPageview('/home', '/andrew'); 
			// tracking calls return error message if any error is encountered.
			if ("" == sError)
				app.writeOut("No Error!!");
			else
				app.writeOut(sError);

			app.writeOut(community.googleanalyticsplugin.lastPayload);
		} else {
			app.writeOut("GA not found??");
		}
	},
	writeOut: function(message) {
		var output = document.getElementById('results');
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement('br'));
		console.log(message);
	},
};
