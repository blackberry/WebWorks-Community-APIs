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

    /***********************************
     * App logic variables and methods
     ***********************************/

    // Pointer to current song
    var mySongPointer = -1;

    // Playlist of songs
    var myPlaylist = [
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

    // Helper method to play a specified song in a given playlist.
    var play = function(playlist, songPointer) {
        var jsonData = {
            songURL: playlist[songPointer].songURL,
            iconURL: playlist[songPointer].iconURL,
            metadata: playlist[songPointer].metadata,
            nextEnabled: songPointer < playlist.length - 1,
            prevEnabled: songPointer > 0
        };

        // Play the song.
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingPlay(jsonData));
    };

    /**********************
     * Setup methods
     *********************/

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


    /**********************
     * Callbacks
     *********************/

	var sampleAsyncCallback = function(data) {
		if (data) {
			console.log(data);
			app.writeOut(data.result);
		}
	};

    var nextCallback = function(data) {
        sampleAsyncCallback(data);

        // Move song pointer.
        if (mySongPointer >= myPlaylist.length - 1) {
            mySongPointer++;

            // Play the next song.
            play(myPlaylist, mySongPointer);
        } else {
            app.writeOut("At last song: can't go next.");
            console.log("At last song: can't go next.");
        }
    };

    var previousCallback = function(data) {
        sampleAsyncCallback(data);

        // Move song pointer.
        if (mySongPointer <= 0) {
            mySongPointer--;

            // Play the previous song.
            play(myPlaylist, mySongPointer);
        } else {
            app.writeOut("At first song: can't go previous.");
            console.log("At first song: can't go previous.");
        }
    };


    /**********************
     * Buttons
     *********************/

	var bindButtons = function() {
		if (com && com.blackberry.community.nowplaying)
		{
			app.writeOut("NowPlaying Result:");
            document.getElementById("clearResultsButton").onclick = app.clearResultsButtonClick;

            document.getElementById("requestPlaybackButton").onclick = app.requestPlaybackButtonClick;

            document.getElementById("play0Button").onclick = app.play0ButtonClick;
            document.getElementById("play1Button").onclick = app.play1ButtonClick;
            document.getElementById("pauseButton").onclick = app.pauseButtonClick;
            document.getElementById("resumeButton").onclick = app.resumeButtonClick;
            document.getElementById("stopButton").onclick = app.stopButtonClick;
            document.getElementById("nextButton").onclick = app.nextButtonClick;
            document.getElementById("previousButton").onclick = app.previousButtonClick;

            document.getElementById("errorButton").onclick = app.errorButtonClick;

			document.getElementById("getStateButton").onclick = app.getStateButtonClick;
		}
		else
		{
			app.writeOut("Plugin was not found");
		}
	};

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
            play: sampleAsyncCallback,
            pause: sampleAsyncCallback,
            stop: sampleAsyncCallback,
            next: nextCallback,
            previous: previousCallback,
            error: sampleAsyncCallback
        };

		app.writeOut(com.blackberry.community.nowplaying.NowPlayingRequestPlayback(jsonData));
	};

    var play0ButtonClick = function() {

        // Update the song pointer.
        mySongPointer = 0;

        // Play the song.
        play(myPlaylist, mySongPointer);
    };

    var play1ButtonClick = function() {

        // Update the song pointer.
        mySongPointer = 1;

        // Play the song.
        play(myPlaylist, mySongPointer);
    };

    var pauseButtonClick = function() {
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingPause());
    };

    var resumeButtonClick = function() {
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingResume());
    };

    var stopButtonClick = function() {
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingStop());
    };

    var nextButtonClick = function() {
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingNext());
    };

    var previousButtonClick = function() {
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingPrevious());
    };

    var errorButtonClick = function() {
        app.writeOut(com.blackberry.community.nowplaying.NowPlayingError());
    };

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

        "play0ButtonClick": play0ButtonClick,
        "play1ButtonClick": play1ButtonClick,
        "pauseButtonClick": pauseButtonClick,
        "resumeButtonClick": resumeButtonClick,
        "stopButtonClick": stopButtonClick,
        "nextButtonClick": nextButtonClick,
        "previousButtonClick": previousButtonClick,

        "errorButtonClick": errorButtonClick,

        "getStateButtonClick": getStateButtonClick
    };
}());

app.init();
