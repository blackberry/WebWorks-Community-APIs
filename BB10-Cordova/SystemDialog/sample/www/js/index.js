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

 dialog = null;

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

	testButtonsNotArray:function(){
		var msg = "This is a negative test and this dialog is not supposed to show!";
		var buttons = "buttons";
		var settings = null;
		var success = function(i){ app.writeOut("testButtonsNotArray fail: This test is supposed to fail but got no error\n"); }
		var fail = function(err){ 
			if ("buttons is not an array" == err) {
				app.writeOut("testButtonsNotArray success: got expected error [" + err + "]\n");
			} else {
				app.writeOut("testButtonsNotArray fail: expected error [buttons is not an array], but got[" + err + "]\n" );
			}
		};
		community.sysdialog.show(msg, buttons, settings, success, fail);
	},

	testButtonsUndefined:function(){
		var msg = "This is a negative test and this dialog is not supposed to show!";
		var buttons = null;
		var settings = null;
		var success = function(i){ app.writeOut("testButtonsUndefined fail: This test is supposed to fail but got no error\n"); }
		var fail = function(err){ 
			if ("buttons is undefined" == err) {
				app.writeOut("testButtonsUndefined success: got expected error [" + err + "]\n");
			} else {
				app.writeOut("testButtonsUndefined fail: expected error [buttons is undefined], but got[" + err + "]\n" );
			}
		};
		community.sysdialog.show(msg, buttons, settings, success, fail);
	},

	testMultiDialog:function(){
		var showDialog = function( count ){
			var msg = "This test invokes 5 dialogs simultaneously; this is dialog " + count ;
			var buttons = ["button1", "but ton 2", "\"button3\""];
			var settings = null;
			var success = function(i){ app.writeOut("testMultiDialog " + count + " success: [" + buttons[i] + "] selected\n"); }
			var fail = function(err){ app.writeOut("testMultiDialog " + count + " fail: " + err + "\n"); }
			community.sysdialog.show(msg, buttons, settings, success, fail);
		};

		for (var i = 0; i < 5; ++i) {
			showDialog(i);
		}
	},

	testRepeat:function(){
		var msg = "This button test the dialog repeat setting. Effects of the notification (LED flash & vibrate) should repeat ";
		var buttons = ["button1", "but ton 2", "\"button3\""];
		var settings = {repeat: true};
		var success = function(i){ app.writeOut("testRepeat success: [" + buttons[i] + "] selected\n"); }
		var fail = function(err){ app.writeOut("testRepeat fail: " + err + "\n" ); }
		community.sysdialog.show(msg, buttons, settings, success, fail);
	},

	testTitle: function(){
		var msg = "This button test the dialog title setting. Dialog should have title [test \"setTitle\" success] ";
		var buttons = ["button1", "but ton 2", "\"button3\""];
		var settings = {title: "test \"setTitle\" success"};
		var success = function(i){ app.writeOut("testTitle success: [" + buttons[i] + "] selected\n"); }
		var fail = function(err){ app.writeOut("testTitle fail: " + err + "\n"); }
		community.sysdialog.show(msg, buttons, settings, success, fail);
	},

	verifyGlobal: function () {

		var showDialog = function(){
			var msg = "This button verify that dialog is global. Dialog will show 5 sec after click button; Minimize the app after click, the dialog will show on homescreen";
			var buttons = ["button1", "but ton 2", "\"button3\""];
			var settings = {};
			var success = function(i){ app.writeOut("verifyGlobal success: [" + buttons[i] + "] selected\n"); }
			var fail = function(err){ app.writeOut("verifyGlobal fail: " + err + "\n"); }
			community.sysdialog.show(msg, buttons, settings, success, fail);

		};

		// confirm clicked
		document.getElementById("verifyGlobal").innerHTML += " (clicked)";
	    setTimeout(showDialog, 5000);
	},

	testPluginCalls: function() {
		if (community && community.sysdialog) {

			app.writeOut("Plugin was found\n");

			document.getElementById("testButtonsNotArray").onclick = app.testButtonsNotArray;
			document.getElementById("testButtonsUndefined").onclick = app.testButtonsUndefined;
			document.getElementById("testMultiDialog").onclick = app.testMultiDialog;
			document.getElementById("testRepeat").onclick = app.testRepeat;
			document.getElementById("testTitle").onclick = app.testTitle;
			document.getElementById("verifyGlobal").onclick = app.verifyGlobal;
		} else {
			app.writeOut("Plugin was not found");
		}
	},
	writeOut: function(message) {
		var output = document.getElementById('results');
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement('br'));
		console.log(message);
	}
};
