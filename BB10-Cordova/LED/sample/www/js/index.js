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
		if (community && community.led) {

			app.writeOut("Plugin was found");
			app.writeOut("Blue LED is flashing");
			var id = community.led.startLed("0000FF", "0");
			//app.writeOut(community.led.stopLed(id));

		} else {
			app.writeOut("Plugin was not found");
		}
	},
	writeOut: function(message) {
		var output = document.getElementById('results');
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement('br'));
		console.log(message);
	},
	aSyncCallback: function(data) {
		if (data) {
			console.log(data);
			app.writeOut(data.value1 + " + " + data.value2 + " = " + data.result);
		}
	},
	threadCallback: function(data) {
		if (app.threadStarted) {
			console.log(data);
			var json = JSON.parse(data);
			app.writeOut("Thread Callback: " + json.threadCount);
			if (json.threadCount >= 10) {
				var end = community.led.stopThread();
				app.writeOut(end);
				app.threadStarted = false;
			}
		} else {
			app.threadStarted = true;
			app.writeOut(data);
		}
	}
};
