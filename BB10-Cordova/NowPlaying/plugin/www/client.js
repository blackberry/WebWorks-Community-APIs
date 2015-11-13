/*
* Copyright (c) 2013 BlackBerry Limited
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

var _self = {},
    _ID = "com.blackberry.community.nowplaying",
    exec = cordova.require("cordova/exec"),
    playbackRequested = false;

    /**
     * NowPlayingRequestPlayback
     * Set up the app for playing music and bind methods that callback to the app.
     *
     * @param input: a json object with methods that callback to the app.
     * The callback methods that must be specified are:
     * - play: Fired when the track is played.
     * - pause: Fired when the track is paused.
     * - stop: Fired when the track is stopped.
     * - next: Fired when the next track is invoked. Callback must invoke com.blackberry.community.nowplaying.play()
     *         with arguments for the next track.
     * - previous: Fired when the previous track is invoked. Callback must invoke com.blackberry.community.nowplaying.play()
     *             with arguments for the previous track.
     * - error: Fired when an internal error occurs.
     * @returns String: whether playback was requested (set up) successfully.
     */
	_self.NowPlayingRequestPlayback = function(input) {

        // TODO: verify json input.

        /* Bind callbacks */
        if (!playbackRequested) {
            var response = _self.NowPlayingValidateCallback(input);

            if((typeof(response) === "boolean") && response){
                _self.NowPlayingBindPlayCallback(input.play);
                _self.NowPlayingBindPauseCallback(input.pause);
                _self.NowPlayingBindStopCallback(input.stop);
                _self.NowPlayingBindNextCallback(input.next);
                _self.NowPlayingBindPreviousCallback(input.previous);
                _self.NowPlayingBindErrorCallback(input.error);

                var result,
                success = function (data, response) {
                    result = data;
                    playbackRequested = true;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                };
                exec(success, fail, _ID, "NowPlayingRequestPlayback");
                return result;
            } else {
                // passes response into error callback
                return response;
            }
        	
        } else {
          return "Playback already requested.";
        }
	};

    _self.NowPlayingBindPlayCallback = function (callback) {
        var success = function (data, response) {
                var json = JSON.parse(data);
                callback(json);
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingBindPlayCallback", null);
    };

    _self.NowPlayingBindPauseCallback = function (callback) {
        var success = function (data, response) {
                var json = JSON.parse(data);
                callback(json);
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingBindPauseCallback", null);
    };

    _self.NowPlayingBindStopCallback = function (callback) {
        var success = function (data, response) {
                var json = JSON.parse(data);
                callback(json);
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingBindStopCallback", null);
    };

    _self.NowPlayingBindNextCallback = function (callback) {
        var success = function (data, response) {
                var json = JSON.parse(data);
                callback(json);
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingBindNextCallback", null);
    };

    _self.NowPlayingBindPreviousCallback = function (callback) {
        var success = function (data, response) {
                var json = JSON.parse(data);
                callback(json);
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingBindPreviousCallback", null);
    };

    _self.NowPlayingBindErrorCallback = function (callback) {
        var success = function (data, response) {
                var json = JSON.parse(data);
                callback(json);
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingBindErrorCallback", null);
    };

    /**
     * NowPlayingPlay
     * Play a specified music in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     *
     * @param input: a json object with music details.
     * The details that must be specified are:
     * - trackURL: URL of the track to play.
     * - iconURL: URL of the icon to display on the volume overlay.
     * - metadata: a json object with metadata details to display on the volume overlay.
     *             Can include title, artist,
     * - nextEnabled: boolean used to set whether or not the Next button should be
     *                enabled on the volume overlay.
     * - previousEnabled: boolean used to set whether or not the Previous button should be
     *                    enabled on the volume overlay.
     * @returns String: whether the music played successfully.
     */
	_self.NowPlayingPlay = function (input) {

        // TODO: verify json input.
        var validInput = _self.NowPlayingValidatePlayInput(input);
        if(typeof(response) === "boolean") && response) {
            var result,
    			success = function (data, response) {
    				result = data;
    			},
    			fail = function (data, response) {
    				console.log("Error: " + data);
    			};
    		exec(success, fail, _ID, "NowPlayingPlay", { input: input });
    		return result;
        } else {
            return "Play input was not valid";
        }

	};

    /**
     * NowPlayingPause
     * Pause the music if there is any in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     *
     * @returns String: whether the music paused successfully.
     */
	_self.NowPlayingPause = function () {
			success = function (data, response) {
				// do nothing, pause callback handle it
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingPause", null);
	};

    /**
     * NowPlayingResume
     * Resume the paused music if there is any in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     *
     * @returns String: whether the music resumed successfully.
     */
    _self.NowPlayingResume = function () {
        var result,
            success = function (data, response) {
                result = data;
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingResume", null);
        return result;
    };

    /**
     * NowPlayingStop
     * Stop the music if there is any in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     *
     * @returns String: whether the music stopped successfully.
     */
    _self.NowPlayingStop = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
        exec(success, fail, _ID, "NowPlayingStop", null);
		return result; 
	};

    /**
     * NowPlayingNext
     * Play the next track according to the callback method given to
     * NowPlayingRequestPlayback.
     *
     * @returns String: whether the music was changed to next one successfully.
     */
    _self.NowPlayingNext = function () {
        var result,
            success = function (data, response) {
                result = data;
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingNext", null);
        return result;
    };

    /**
     * NowPlayingPrevious
     * Play the previous track according to the callback method given to
     * NowPlayingRequestPlayback.
     *
     * @returns String: whether the music was changed to previous one successfully.
     */
    _self.NowPlayingPrevious = function () {
        var result,
            success = function (data, response) {
                result = data;
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingPrevious", null);
        return result;
    };

    /**
     * NowPlayingError
     * Fire the error callback method given to NowPlayingRequestPlayback.
     * For debugging purposes only.
     *
     * @returns String: error information
     */
    _self.NowPlayingError = function () {
        var result,
            success = function (data, response) {
                result = data;
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingError", null);
        return result;
    };

    // Validate play input


    /* We expect the following callbacks to be present:
    play, pause, stop, next, previous, error
    */
    _self.NowPlayingValidateCallback = function(input) {
        if(!('play' in input)) return "Play callback was not provided";
        if(!('pause' in input)) return "Pause callback was not provided";
        if(!('stop' in input)) return "Stop callback was not provided";
        if(!('next' in input)) return "Next callback was not provided";
        if(!('previous' in input)) return "Previous callback was not provided";
        if(!('error' in input)) return "Error callback was not provided";

        if(!(isFunction(input.play))) return "Play attribute was not a function";
        if(!(isFunction(input.pause))) return "Pause attribute was not a function";
        if(!(isFunction(input.stop))) return "Stop attribute was not a function";
        if(!(isFunction(input.next))) return "Next attribute was not a function";
        if(!(isFunction(input.previous))) return "Previous attribute was not a function";
        if(!(isFunction(input.error))) return "Error attribute was not a function";

        // if all attributes present and are function, valid callback
        return true;

    };

    function isFunction(functionToCheck) {
        var getType = {};
        return functionToCheck && getType.toString.call(functionToCheck) === '[object Function]';
    };

    /* 
     trackURL: "http://www.pch.gc.ca/DAMAssetPub/DAM-hymChs-antSgn/STAGING/audio-audio/o-canada_1359474460106_eng.MP3",
            iconURL: "img/canadian-flag-small.jpg",
            metadata: {
                Title: "O Canada",
                Artist: "Canada",
                Album: "Canada's Favorites"
            }
    */

    _self.NowPlayingValidatePlayInput = function(input) {
        var response = true;
        if(!('iconURL' in input)) {
            response = "iconURL attribute was not provided";
            // trigger the error and pass it in as a input
            return response;
        } 
        if(!('trackURL' in input)) {
            response =  "trackURL attribute was not provided";

            return response;
        } 

        if(!('metadata' in input)) {
            response = "metadata attribute was not provided";

            return response;
        } 

        if(!('Title' in input.metadata)) {
            response = "Title field in metadata was not provided";

            return response;
        }

        if(!('Artist') in input.metadata) {
            response = "Artist field in metadata was not provided";

            return response;
        }

        if(!('Album' in input.metadata)) {
            response = "Album field in metadata was not provided";

            return response;
        }

        return response;
    }


    
    /**
     * NowPlayingGetState
     * Get the state of the music being played.
     * For debugging purposes only.
     *
     * @returns String: containing:
     * - the state of the MediaPlayer,
     * - whether the NowPlayingConnection is acquired,
     * - whether the NowPlayingConnection is preempted.
     */
	_self.NowPlayingGetState = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingGetState", null);
		return result;
	};

module.exports = _self;
