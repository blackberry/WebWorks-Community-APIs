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

	// binding just one key
	bindShortVolumeUp: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "short",
			onPressed: function() { app.writeOut("button volume up (short) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindLongVolumeUp: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "long",
			onPressed: function() { app.writeOut("button volume up (long) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindShortVolumeDown: function() {
		var mediaKeyObj = {
			mediakey: "volumeDown",
			keylength: "short",
			onPressed: function() { app.writeOut("button volume down (short) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindLongVolumeDown: function() {
		var mediaKeyObj = {
			mediakey: "volumeDown",
			keylength: "long",
			onPressed: function() { app.writeOut("button volume down (long) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindShortPlayPause: function() {
		var mediaKeyObj = {
			mediakey: "playPause",
			keylength: "short",
			onPressed: function() { app.writeOut("button play pause (short) pressed"); }
		};
		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindLongPlayPause: function() {
		var mediaKeyObj = {
			mediakey: "playPause",
			keylength: "long",
			onPressed: function() { app.writeOut("button play pause (long) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindVolumeUpDefaultFailure: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "short",
			onPressed: function() { app.writeOut("button volume up (short) pressed"); }
		};

		community.mediakeys.bind(mediaKeyObj);
	},

	// binding more than one key
	bindMultiple: function() {
		var mediaKeyUpShort = {
			mediakey: "volumeUp",
			keylength: "short",
			onPressed: function() { app.writeOut("button volume up (short) pressed"); }
		};
		var mediaKeyDownShort = {
			mediakey: "volumeDown",
			keylength: "short",
			onPressed: function() { app.writeOut("button volume down (short) pressed"); }
		};
		var mediaKeyUpLong = {
			mediakey: "volumeUp",
			keylength: "long",
			onPressed: function() { app.writeOut("button volume up (long) pressed"); }
		};
		var mediaKeyDownLong = {
			mediakey: "volumeDown",
			keylength: "long",
			onPressed: function() { app.writeOut("button volume down (long) pressed"); }
		};
		var mediaKeyPlayPauseShort = {
			mediakey: "playPause",
			keylength: "short",
			onPressed: function() { app.writeOut("button play pause (short) pressed"); }
		};
		var mediaKeyPlayPauseLong = {
			mediakey: "playPause",
			keylength: "long",
			onPressed: function() { app.writeOut("button play pause (long) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		var keyBindings = [mediaKeyUpShort, mediaKeyDownShort,mediaKeyUpLong, mediaKeyDownLong, mediaKeyPlayPauseShort, mediaKeyPlayPauseLong];

		community.mediakeys.bind(keyBindings, generalFailure);
	},

	// binding all keys with short keylength
	// binded keys: fastForward, pause, play, playPause, rewind, stop, volumeDown, volumeUp, previous, next
	bindAllShort: function() {
		var mediaKeyFastForwardShort = {
			mediakey: "fastForward",
			keylength: "short",
			onPressed: function() { app.writeOut("button fastForward (short) pressed"); }
		};
		var mediaKeyPauseShort = {
			mediakey: "pause",
			keylength: "short",
			onPressed: function() { app.writeOut("button pause (short) pressed"); }
		};
		var mediaKeyPlayShort = {
			mediakey: "play",
			keylength: "short",
			onPressed: function() { app.writeOut("button play (short) pressed"); }
		};
		var mediaKeyPlayPauseShort = {
			mediakey: "playPause",
			keylength: "short",
			onPressed: function() { app.writeOut("button playPause (short) pressed"); }
		};
		var mediaKeyRewindShort = {
			mediakey: "rewind",
			keylength: "short",
			onPressed: function() { app.writeOut("button rewind (short) pressed"); }
		};
		var mediaKeyStopShort = {
			mediakey: "stop",
			keylength: "short",
			onPressed: function() { app.writeOut("button stop (short) pressed"); }
		};
		var mediaKeyVolumeDownShort = {
			mediakey: "volumeDown",
			keylength: "short",
			onPressed: function() { app.writeOut("button volumeDown (short) pressed"); }
		};
		var mediaKeyVolumeUpShort = {
			mediakey: "volumeUp",
			keylength: "short",
			onPressed: function() { app.writeOut("button volumeUp (short) pressed"); }
		};
		var mediaKeyPreviousShort = {
			mediakey: "previous",
			keylength: "short",
			onPressed: function() { app.writeOut("button previous (short) pressed"); }
		};
		var mediaKeyNextShort = {
			mediakey: "next",
			keylength: "short",
			onPressed: function() { app.writeOut("button next (short) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		var keyBindings = [
			mediaKeyFastForwardShort,
			mediaKeyPauseShort,
			mediaKeyPlayShort,
			mediaKeyPlayPauseShort,
			mediaKeyRewindShort,
			mediaKeyStopShort,
			mediaKeyVolumeDownShort,
			mediaKeyVolumeUpShort,
			mediaKeyPreviousShort,
			mediaKeyNextShort
		];

		community.mediakeys.bind(keyBindings, generalFailure);
	},

	// test potential errors
	bindInvalidKey: function() {
		var mediaKeyObj = {
			mediakey: "invalid",
			keylength: "long",
			onPressed: function() { app.writeOut("button invalid key (long) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindInvalidLength: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "invalid",
			onPressed: function() { app.writeOut("button volume up (invalid length) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindInvalidMediaKeyObj: function() {
		var mediaKeyObj = {
			invalidfield1: "volumeUp",
			invalidfield2: "short",
			onPressed: function() { app.writeOut("button volume up (short) pressed"); }
		};

		var generalFailure = function (error) { app.writeOut("unable to bind invalid media keys: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindInvalidOnPressedCallback: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "short",
			onPressed: "string"
		};

		var generalFailure = function (error) { app.writeOut("unable to call onPressed callback: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
	},
	bindNoOnPressedCallback: function() {
		var mediaKeyObj = {
			mediakey: "volumeUp",
			keylength: "short",
		};

		var generalFailure = function (error) { app.writeOut("unable to call onPressed callback: "+error); };

		community.mediakeys.bind(mediaKeyObj, generalFailure);
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
			document.getElementById("bindVolumeUpDefaultFailure").onclick = app.bindVolumeUpDefaultFailure;

			// bind multiple
			document.getElementById("bindMultiple").onclick = app.bindMultiple;
			document.getElementById("bindAllShort").onclick = app.bindAllShort;

			// error testing
			document.getElementById("bindInvalidKey").onclick = app.bindInvalidKey;
			document.getElementById("bindInvalidLength").onclick = app.bindInvalidLength;
			document.getElementById("bindInvalidMediaKeyObj").onclick = app.bindInvalidMediaKeyObj;
			document.getElementById("bindInvalidOnPressedCallback").onclick = app.bindInvalidOnPressedCallback;
			document.getElementById("bindNoOnPressedCallback").onclick = app.bindNoOnPressedCallback;
			

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
