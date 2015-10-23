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
var app = (function() {

    // Playlist of songs (json data)
    var playlist = [
        {
            songURL: "http://www.pch.gc.ca/DAMAssetPub/DAM-hymChs-antSgn/STAGING/audio-audio/o-canada_1359474460106_eng.MP3",
            iconURL: "http://flaglane.com/download/canadian-flag/canadian-flag-small.jpg",
            metadata: {
                Title: "O Canada",
                Artist: "Canada",
                Album: "Canada's Favorites"
            }
        },
        {
            songURL: "sounds/highhat.mp3",
            iconURL: "img/Hi-hat.jpg",
            metadata: {
                Title: "High Hat",
                Artist: "Drum Kit",
                Album: "Instruments"
            }
        }
    ];

	// Application Constructor
	var init = function() {
		app.bindEvents();
	};

	// Bind Event Listeners
	//
	// Bind any events that are required on startup. Common events are:
	// "load", "deviceready", "offline", and "online".
	var bindEvents = function() {
		document.addEventListener("deviceReady",
            function() { app.receivedEvent("deviceReady") },
            false);
	};

	// Update DOM on a Received Event
	var receivedEvent = function(id) {
		var parentElement = document.getElementById(id);
		var listeningElement = parentElement.querySelector(".listening");
		var receivedElement = parentElement.querySelector(".received");

		listeningElement.setAttribute("style", "display:none;");
		receivedElement.setAttribute("style", "display:block;");

		console.log("Received Event: " + id);
		app.bindButtons();
	};

    // Write out to screen and console
	var writeOut = function(message) {
		var output = document.getElementById("results");
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement("br"));
		console.log(message);
	};

	var sampleAsyncCallback = function(data) {
		if (data) {
			console.log(data);
			app.writeOut(data.result);
		}
	};

	var bindButtons = function() {
		if (com && com.blackberry.community.nowplaying)
		{
			app.writeOut("NowPlaying Result:");
            document.getElementById("clearResultsButton").onclick = app.clearResultsButtonClick;

            document.getElementById("requestPlaybackButton").onclick = app.requestPlaybackButtonClick;

            //document.getElementById("trackChangeButton").onclick = app.trackChangeButtonClick;

            document.getElementById("playButton").onclick = app.playButtonClick;
            //document.getElementById("stopButton").onclick = app.stopButtonClick;
            //document.getElementById("pauseButton").onclick = app.pauseButtonClick;
            //document.getElementById("resumeButton").onclick = app.resumeButtonClick;

			document.getElementById("getStateButton").onclick = app.getStateButtonClick;
		}
		else
		{
			app.writeOut("Plugin was not found");
		}
	};

    /* Button actions */

    var clearResultsButtonClick = function() {
        var output = document.getElementById("results");
        output.innerText = "";
        while (output.firstChild) {
            output.removeChild(output.firstChild);
        }
        app.writeOut("NowPlaying Result:");
    };


	var requestPlaybackButtonClick = function() {
        var jsonData = {
            songURL: playlist[0].songURL,
            iconURL: playlist[0].iconURL,
            metadata: playlist[0].metadata,
            nextEnabled: true,
            prevEnabled: true,
            callbacks: {
                play: sampleAsyncCallback,
                stop: sampleAsyncCallback,
                pause: sampleAsyncCallback,
                resume: sampleAsyncCallback,
                error: sampleAsyncCallback,
                next: sampleAsyncCallback,
                previous: sampleAsyncCallback
            }
        };

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingRequestPlayback(jsonData));
	};


    //var trackChangeButtonClick = function() {
    //    var jsonData = {
    //        "Title": "MyTitleChangeTrack1",
    //        "Artist": "MyArtistChangeTrack1",
    //        "Album": "MyAlbumChangeTrack1"
    //    };
    //
    //    app.writeOut(com.blackberry.community.nowplaying.NowPlayingChangeTrack(app.sampleAsyncCallback, jsonData));
    //};

    var playButtonClick = function() {
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingPlay());
    };

    //var stopButtonClick = function() {
     //   app.writeOut(com.blackberry.community.nowplaying.NowPlayingStop(app.sampleAsyncCallback));
    //};
    //
    //var pauseButtonClick = function() {
	//	app.writeOut(com.blackberry.community.nowplaying.NowPlayingPause(app.sampleAsyncCallback));
	//};
    //
	//var resumeButtonClick = function() {
	//	app.writeOut(com.blackberry.community.nowplaying.NowPlayingResume(app.sampleAsyncCallback));
	//};


	var getStateButtonClick = function() {
		app.writeOut(com.blackberry.community.nowplaying.NowPlayingGetState());
	};


    return {
        "init": init,
        "bindEvents": bindEvents,
        "receivedEvent": receivedEvent,
        "writeOut": writeOut,
        "sampleAsyncCallback": sampleAsyncCallback,
        "bindButtons": bindButtons,
        "clearResultsButtonClick": clearResultsButtonClick,
        "requestPlaybackButtonClick": requestPlaybackButtonClick,
        //"trackChangeButtonClick": trackChangeButtonClick,
        "playButtonClick": playButtonClick,
        //"pauseButtonClick": pauseButtonClick,
        //"resumeButtonClick": resumeButtonClick,
        //"stopButtonClick": stopButtonClick,
        "getStateButtonClick": getStateButtonClick
    };
}());

app.init();
