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
    playbackRequested = false,
    stopped = true;

    /**
     * NowPlayingRequestPlayback
     * Sets up the app for playing music and binds callbacks to the app.
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
        var result = "";

        /* Bind callbacks */
        if (!playbackRequested) {
            var valid = _self.NowPlayingValidateCallback(input);

            if (valid) {
                _self.NowPlayingBindPlayCallback(input.play);
                _self.NowPlayingBindPauseCallback(input.pause);
                _self.NowPlayingBindStopCallback(input.stop);
                _self.NowPlayingBindNextCallback(input.next);
                _self.NowPlayingBindPreviousCallback(input.previous);
                _self.NowPlayingBindErrorCallback(input.error);

                var success = function (data, response) {
                        result = data;
                        playbackRequested = true;
                    },
                    fail = function (data, response) {
                        result = "Error: " + data;
                    };
                exec(success, fail, _ID, "NowPlayingRequestPlayback", null);
            } else {
                result = "Playback input was not valid.";
            }
        } else {
            result = "Playback already requested.";
        }

        console.log(result);
        return result;
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
                console.log(data);
                var json = JSON.parse(data);
                console.log(json);
                callback(json);
            },
            fail = function (data, response) {
                console.log("Error: " + data);
            };
        exec(success, fail, _ID, "NowPlayingBindErrorCallback", null);
    };

    /**
     * NowPlayingPlay
     * Plays a specified track in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     * Use this method to play the track if it isn't automatically restarted
     * after the app playback is no longer preempted.
     *
     * @param input: a json object with track details.
     * The details that can be specified are:
     * - trackURL: URL of the track to play (required).
     * - iconURL: URL of the icon to display on the volume overlay.
     * - metadata: a json object with metadata details to display on the volume overlay.
     *             Can include title, artist,
     * - nextEnabled: boolean used to set whether or not the Next button should be
     *                enabled on the volume overlay.
     * - previousEnabled: boolean used to set whether or not the Previous button should be
     *                    enabled on the volume overlay.
     * @returns String: whether the track played successfully.
     */
	_self.NowPlayingPlay = function (input) {
        var result = "";

        if (!playbackRequested) {
            result = "Need to request playback first.";
        } else {
            var validInput = _self.NowPlayingValidatePlayInput(input);

            if (validInput) {
                var success = function (data, response) {
        				result = data;
                        stopped = false;
        			},
        			fail = function (data, response) {
        				result = "Error: " + data;
                        _self.NowPlayingError("Error: " + data);
        			};
        		exec(success, fail, _ID, "NowPlayingPlay", { input: input });

            } else {
                result = "Play input was not valid";
            }
        }

        console.log(result);
        return result;
	};

    /**
     * NowPlayingPause
     * Pauses the track if there is any in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     *
     * @returns String: whether the track paused successfully.
     */
	_self.NowPlayingPause = function () {
        var result = "";

        if (!playbackRequested) {
            result = "Need to request playback first.";
        } else if (stopped) {
            result = "Track cannot be paused because it is stopped."
                + "Call the play, next, or previous methods to start playing a new track.";
            _self.NowPlayingError(result);
        } else if (! isConnectionAcquired()) {
            result = "Track cannot be paused because the NowPlayingConnection is no longer acquired."
                + "Call the play, next, or previous methods to start playing a new track.";
            _self.NowPlayingError(result);
        } else {
			var success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    result = "Error: " + data;
                    _self.NowPlayingError(result);
                };
            exec(success, fail, _ID, "NowPlayingPause", null);
        }

        console.log(result);
        return result;
	};

    /**
     * NowPlayingResume
     * Resumes the paused track if there is any in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     *
     * @returns String: whether the track resumed successfully.
     */
    _self.NowPlayingResume = function () {
        var result = "";

        if (!playbackRequested) {
            result = "Need to request playback first.";
        } else if (stopped) {
            result = "Track cannot be resumed because it is stopped."
                + "Call the play, next, or previous methods to start playing a new track.";
            _self.NowPlayingError(result);
        } else if (! isConnectionAcquired()) {
            result = "Track cannot be resumed because the NowPlayingConnection is no longer acquired."
                + "Call the play, next, or previous methods to start playing a new track.";
            _self.NowPlayingError(result);
        } else {
            var success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    result = "Error: " + data;
                    _self.NowPlayingError(result);
                };
            exec(success, fail, _ID, "NowPlayingResume", null);
        }

        console.log(result);
        return result;
    };

    function isConnectionAcquired() {
        return _self.NowPlayingGetState().indexOf("Acquired: True") > -1;
    }

    /**
     * NowPlayingStop
     * Stops the track if there is any in the background.
     * Executes the callback method given to NowPlayingRequestPlayback.
     *
     * @returns String: whether the track stopped successfully.
     */
    _self.NowPlayingStop = function () {
        var result = "";

		if (!playbackRequested) {
            result = "Need to request playback first.";
        } else {
            var success = function (data, response) {
    				result = data;
                    stopped = true;
                },
    			fail = function (data, response) {
    				result = "Error: " + data;
                    _self.NowPlayingError(result);
    			};
            exec(success, fail, _ID, "NowPlayingStop", null);
        }

        console.log(result);
        return result;
	};

    /**
     * NowPlayingNext
     * Plays the next track according to the callback method given to
     * NowPlayingRequestPlayback.
     *
     * @returns String: whether the track was changed to next one successfully.
     */
    _self.NowPlayingNext = function () {
        var result = "";

        if (!playbackRequested) {
            result = "Need to request playback first.";
        } else {
            var success = function (data, response) {
                    result = data;
                    stopped = false;
                },
                fail = function (data, response) {
                    result = "Error: " + data;
                    _self.NowPlayingError(result);
                };
            exec(success, fail, _ID, "NowPlayingNext", null);
        }

        console.log(result);
        return result;
    };

    /**
     * NowPlayingPrevious
     * Plays the previous track according to the callback method given to
     * NowPlayingRequestPlayback.
     *
     * @returns String: whether the track was changed to previous one successfully.
     */
    _self.NowPlayingPrevious = function () {
        var result = "";

        if (!playbackRequested) {
            result = "Need to request playback first.";
        } else {
            var success = function (data, response) {
                    result = data;
                    stopped = false;
                },
                fail = function (data, response) {
                    result ="Error: " + data;
                    _self.NowPlayingError(result);
                };
            exec(success, fail, _ID, "NowPlayingPrevious", null);
        }

        console.log(result);
        return result;
    };

    /**
     * NowPlayingError
     * Fires the error callback method given to NowPlayingRequestPlayback.
     * For debugging purposes only.
     *
     * @returns String: error information
     */
    _self.NowPlayingError = function (error) {
        var result = "";

        if (!playbackRequested) {
            result = "Need to request playback first.";
        } else {
            var success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    result = "Error: " + data;
                };
            exec(success, fail, _ID, "NowPlayingError", { input: error });
        }

        console.log(result);
        return result;
    };

    /**
     * NowPlayingValidateCallback
     * Validates the input to the NowPlayingRequestPlayback.
     *
     * @param input: a json object with methods that callback to the app.
     * The callback methods that should be specified are:
     * - play: Fired when the track is played.
     * - pause: Fired when the track is paused.
     * - stop: Fired when the track is stopped.
     * - next: Fired when the next track is invoked. Callback must invoke com.blackberry.community.nowplaying.play()
     *         with arguments for the next track.
     * - previous: Fired when the previous track is invoked. Callback must invoke com.blackberry.community.nowplaying.play()
     *             with arguments for the previous track.
     * - error: Fired when an internal error occurs.
     * @returns Boolean: whether the input is valid.
     */
    _self.NowPlayingValidateCallback = function(input) {
        var result = "";
        var valid = true;

        if (!('play' in input)) result += "Play callback was not provided\n";
        if (!('pause' in input)) result += "Pause callback was not provided\n";
        if (!('stop' in input)) result += "Stop callback was not provided\n";
        if (!('next' in input)) result += "Next callback was not provided\n";
        if (!('previous' in input)) result += "Previous callback was not provided\n";
        if (!('error' in input)) result += "Error callback was not provided\n";
        
        if (result !== "") {
            valid = false;

            // Log error to console since no error callback is defined at this point.
            console.log(result);
        }

        if (valid) {
            valid = _self.NowPlayingValidateCallbackAttributes(input);  
        }

        return valid;
    };

    _self.NowPlayingValidateCallbackAttributes = function(input) {
        var result = true;

        if(!(isFunction(input.play))) result += "Play attribute was not a function\n";
        if(!(isFunction(input.pause))) result += "Pause attribute was not a function\n";
        if(!(isFunction(input.stop))) result += "Stop attribute was not a function\n";
        if(!(isFunction(input.next))) result += "Next attribute was not a function\n";
        if(!(isFunction(input.previous))) result += "Previous attribute was not a function\n";
        if(!(isFunction(input.error))) result += "Error attribute was not a function\n";

        if (typeof(result) === "boolean" && result === true) {
            return true;
        } else {

            // Log error to console since no error callback is defined at this point.
            console.log(result);
            return false;
        }
    };

    function isFunction(functionToCheck) {
        var getType = {};
        return functionToCheck && getType.toString.call(functionToCheck) === '[object Function]';
    }

    /**
     * NowPlayingValidatePlayInput
     * Validates the input to the NowPlayingPlay.
     *
     * @param input: a json object with track details.
     * The detail that must be specified is the trackURL: URL of the track to play.
     * @returns boolean: whether the input is valid.
     */
    _self.NowPlayingValidatePlayInput = function(input) {
        if ('trackURL' in input) {
            return true;
        } else {
            _self.NowPlayingError("Track URL attribute was not provided in input to NowPlayingPlay.");
            return false;
        }
    };

    /**
     * NowPlayingGetState
     * Returns the state of the plugin.
     * Used for debugging purposes.
     *
     * @returns String: containing:
     * - the state of the MediaPlayer,
     * - whether the NowPlayingConnection is acquired,
     * - whether the NowPlayingConnection is preempted.
     */
	_self.NowPlayingGetState = function () {
		var result = "";

        if (!playbackRequested) {
            result = "Need to request playback first.";
        } else {
            var success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    result = "Error: " + data;
                    _self.NowPlayingError(result);
                };
            exec(success, fail, _ID, "NowPlayingGetState", null);
        }

        console.log(result);
		return result;
	};

module.exports = _self;
