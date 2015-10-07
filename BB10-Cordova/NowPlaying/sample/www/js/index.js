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
	initialize: function () {
		this.bindEvents();
	},

	// Bind Event Listeners
	//
	// Bind any events that are required on startup. Common events are:
	// "load", "deviceready", "offline", and "online".
	bindEvents: function () {
		document.addEventListener("deviceready", this.onDeviceready, false);
	},

	// deviceready Event Handler
	//
	// The scope of 'this' is the event. In order to call the 'receivedEvent'
	// function, we must explicitly call 'app.receivedEvent(...);'
	onDeviceready: function () {
		app.receivedEvent("deviceready");
	},

	// Update DOM on a Received Event
	receivedEvent: function (id) {
		var parentElement = document.getElementById(id);
		var listeningElement = parentElement.querySelector(".listening");
		var receivedElement = parentElement.querySelector(".received");

		listeningElement.setAttribute("style", "display:none;");
		receivedElement.setAttribute("style", "display:block;");

		console.log("Received Event: " + id);
		app.testPluginCalls();
	},

    // Write out to screen and console
	writeOut: function(message) {
		var output = document.getElementById("results");
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement("br"));
		console.log(message);
	},

	sampleAsyncCallback: function (data) {
		if (data) {
			console.log(data);
			app.writeOut(data.result);
		}
	},

    // Test plugin calls
	testPluginCalls: function () {
		if (com && com.blackberry.community.nowplaying)
		{
			app.writeOut("NowPlaying Result:");
			document.getElementById("setMusicButton").onclick = app.setMusicButtonClick;
			document.getElementById("setMusic2Button").onclick = app.setMusic2ButtonClick;
			document.getElementById("setMetadataButton").onclick = app.setMetadataButtonClick;
			document.getElementById("setMetadata2Button").onclick = app.setMetadata2ButtonClick;
			document.getElementById("setIconButton").onclick = app.setIconButtonClick;
			document.getElementById("setIcon2Button").onclick = app.setIcon2ButtonClick;

			document.getElementById("changeTrackButton").onclick = app.changeTrackButtonClick;
			document.getElementById("changeTrack2Button").onclick = app.changeTrack2ButtonClick;

			document.getElementById("enablePreviousNextButton").onclick = app.enablePreviousNextButtonClick;
			document.getElementById("disablePreviousNextButton").onclick = app.disablePreviousNextButtonClick;

			document.getElementById("playButton").onclick = app.playButtonClick;
			document.getElementById("pauseButton").onclick = app.pauseButtonClick;
			document.getElementById("resumeButton").onclick = app.resumeButtonClick;
			document.getElementById("stopButton").onclick = app.stopButtonClick;

			document.getElementById("getStateButton").onclick = app.getStateButtonClick;
		}
		else
		{
			app.writeOut("Plugin was not found");
		}
	},

	/* Plugin calls */
	setMusicButtonClick: function() {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingSetMusic(
			"http://www.pch.gc.ca/DAMAssetPub/DAM-hymChs-antSgn/STAGING/audio-audio/o-canada_1359474460106_eng.MP3"));
	},

	setMusic2ButtonClick: function() {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingSetMusic("sounds/highhat.mp3"));
	},

	setMetadataButtonClick: function() {
		var jsonData = {
			"Title": "MyTitleSetMetadata1",
			"Artist": "MyArtistSetMetadata1",
			"Album": "MyAlbumSetMetadata1"
		};

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingSetMetadata(jsonData));
	},

	setMetadata2ButtonClick: function() {
		var jsonData = {
			"Title": "MyTitleSetMetadata2",
			"Artist": "MyArtistSetMetadata2",
			"Album": "MyAlbumSetMetadata2"
		};

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingSetMetadata(jsonData));
	},

	setIconButtonClick: function() {
		var jsonData = {
			"Title": "MyTitleSetIcon1",
			"Artist": "MyArtistSetIcon1",
			"Album": "MyAlbumSetIcon1"
		};

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingSetIcon(jsonData));
	},

	setIcon2ButtonClick: function() {
		var jsonData = {
			"Title": "MyTitleSetIcon2",
			"Artist": "MyArtistSetIcon2",
			"Album": "MyAlbumSetIcon2"
		};

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingSetIcon(jsonData));
	},


	changeTrackButtonClick: function() {
		var jsonData = {
			"Title": "MyTitleChangeTrack1",
			"Artist": "MyArtistChangeTrack1",
			"Album": "MyAlbumChangeTrack1"
		};

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingChangeTrack(app.sampleAsyncCallback, jsonData));
	},

	changeTrack2ButtonClick: function() {
		var jsonData = {
			"Title": "MyTitleChangeTrack2",
			"Artist": "MyArtistChangeTrack2",
			"Album": "MyAlbumChangeTrack2"
		};

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingChangeTrack(app.sampleAsyncCallback, jsonData));
	},


	enablePreviousNextButtonClick: function() {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingEnableNextPrevious());
	},

	disablePreviousNextButtonClick: function() {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingDisableNextPrevious());
	},


	playButtonClick: function () {
		var jsonData = {
			"Title": "MyTitlePlay",
			"Artist": "MyArtistPlay",
			"Album": "MyAlbumPlay"
		};

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingPlay(app.sampleAsyncCallback, jsonData));
	},

	pauseButtonClick: function () {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingPause(app.sampleAsyncCallback));
	},

	resumeButtonClick: function () {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingResume(app.sampleAsyncCallback));
	},

	stopButtonClick: function () {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingStop(app.sampleAsyncCallback));
	},


	getStateButtonClick: function () {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingGetState());
	}
};
