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
		community.mediakeys.show(msg, buttons, settings, success, fail);
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
		community.mediakeys.show(msg, buttons, settings, success, fail);
	},

	testMultiDialog:function(){
		var showDialog = function( count ){
			var msg = "This test invokes 5 dialogs simultaneously; this is dialog " + count ;
			var buttons = ["button1", "but ton 2", "\"button3\""];
			var settings = null;
			var success = function(i){ app.writeOut("testMultiDialog " + count + " success: [" + buttons[i] + "] selected\n"); }
			var fail = function(err){ app.writeOut("testMultiDialog " + count + " fail: " + err + "\n"); }
			community.mediakeys.show(msg, buttons, settings, success, fail);
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
		community.mediakeys.show(msg, buttons, settings, success, fail);
	},

	testTitle: function(){
		var msg = "This button test the dialog title setting. Dialog should have title [test \"setTitle\" success] ";
		var buttons = ["button1", "but ton 2", "\"button3\""];
		var settings = {title: "test \"setTitle\" success"};
		var success = function(i){ app.writeOut("testTitle success: [" + buttons[i] + "] selected\n"); }
		var fail = function(err){ app.writeOut("testTitle fail: " + err + "\n"); }
		community.mediakeys.show(msg, buttons, settings, success, fail);
	},

	verifyGlobal: function () {

		var showDialog = function(){
			var msg = "This button verify that dialog is global. Dialog will show 5 sec after click button; Minimize the app after click, the dialog will show on homescreen";
			var buttons = ["button1", "but ton 2", "\"button3\""];
			var settings = {};
			var success = function(i){ app.writeOut("verifyGlobal success: [" + buttons[i] + "] selected\n"); }
			var fail = function(err){ app.writeOut("verifyGlobal fail: " + err + "\n"); }
			community.mediakeys.show(msg, buttons, settings, success, fail);

		};

		// confirm clicked
		document.getElementById("verifyGlobal").innerHTML += " (clicked)";
	    setTimeout(showDialog, 5000);
	},

	bindShortVolumeUp: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "short",
			success: function(i) { app.writeOut("bind volume up (short) succeeded"); },
			fail: function() { app.writeOut("bind volume up (short) failed"); }
		};

		community.mediakeys.bind(mediaKeyObj);
	},
	bindLongVolumeUp: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "long",
			success: function(i) { app.writeOut("bind volume up (long) succeeded"); },
			fail: function() { app.writeOut("bind volume up (long) failed"); }
		};

		community.mediakeys.bind(mediaKeyObj);
	},
	bindShortVolumeDown: function() {
		var mediaKeyObj = {
			mediakey: "volumeDown",
			keylength: "short",
			success: function(i) { app.writeOut("bind volume down (short) succeeded"); },
			fail: function() { app.writeOut("bind volume down (short) failed"); }
		};

		community.mediakeys.bind(mediaKeyObj);
	},
	bindLongVolumeDown: function() {
		var msg = "This button tests the volume up button";

		var mediaKeyObj = {
			mediakey: "volumeDown",
			keylength: "long",
			success: function(i) { app.writeOut("bind volume down (long) succeeded"); },
			fail: function() { app.writeOut("bind volume down (long) failed"); }
		};

		community.mediakeys.bind(mediaKeyObj);
	},
	bindShortPlayPause: function() {
		var mediaKeyObj = {
			mediakey: "playPause",
			keylength: "short",
			success: function(i) { app.writeOut("bind play pause (short) succeeded"); },
			fail: function() { app.writeOut("bind play pause (short) failed"); }
		};

		community.mediakeys.bind(mediaKeyObj);
	},
	bindLongPlayPause: function() {
		var mediaKeyObj = {
			mediakey: "playPause",
			keylength: "long",
			success: function(i) { app.writeOut("bind play pause (long) succeeded"); },
			fail: function() { app.writeOut("bind play pause (long) failed"); }
		};

		community.mediakeys.bind(mediaKeyObj);
	},
	bindMultiple: function() {
		var mediaKeyUpShort = {
			mediakey: "volumeUp",
			keylength: "short",
			success: function() { app.writeOut("bind volume up (short) succeeded"); },
			fail: function() { app.writeOut("bind volume up (short) failed"); }
		};
		var mediaKeyDownShort = {
			mediakey: "volumeDown",
			keylength: "short",
			success: function() { app.writeOut("bind volume down (short) succeeded"); },
			fail: function() { app.writeOut("bind volume down (short) failed"); }
		};
		var mediaKeyUpLong = {
			mediakey: "volumeUp",
			keylength: "long",
			success: function() { app.writeOut("bind volume up (long) succeeded"); },
			fail: function() { app.writeOut("bind volume up (long) failed"); }
		};
		var mediaKeyDownLong = {
			mediakey: "volumeDown",
			keylength: "long",
			success: function() { app.writeOut("bind volume down (long) succeeded"); },
			fail: function() { app.writeOut("bind volume down (long) failed"); }
		};
		var mediaKeyPlayPauseShort = {
			mediakey: "playPause",
			keylength: "short",
			success: function(i) { app.writeOut("bind play pause (short) succeeded"); },
			fail: function() { app.writeOut("bind play pause (short) failed"); }
		};
		var mediaKeyPlayPauseLong = {
			mediakey: "playPause",
			keylength: "long",
			success: function(i) { app.writeOut("bind play pause (long) succeeded"); },
			fail: function() { app.writeOut("bind play pause (long) failed"); }
		};

		var keyBindings = [mediaKeyUpShort, mediaKeyDownShort,mediaKeyUpLong, mediaKeyDownLong, mediaKeyPlayPauseShort, mediaKeyPlayPauseLong];

		community.mediakeys.bind(keyBindings);
	},

	testPluginCalls: function() {
		if (community && community.mediakeys) {

			app.writeOut("Plugin was found\n");

			document.getElementById("shortVolumeUp").onclick = app.bindShortVolumeUp;
			document.getElementById("shortVolumeDown").onclick = app.bindShortVolumeDown;
			document.getElementById("shortPlayPause").onclick = app.bindShortPlayPause;
			document.getElementById("longVolumeUp").onclick = app.bindLongVolumeUp;
			document.getElementById("longVolumeDown").onclick = app.bindLongVolumeDown;
			document.getElementById("longPlayPause").onclick = app.bindLongPlayPause;
			document.getElementById("bindMultiple").onclick = app.bindMultiple;

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
