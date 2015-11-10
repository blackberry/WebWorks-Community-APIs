/*
* Copyright (c) 2013 BlackBerry Limited.
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

var resultObjs = {};
    //threadCallback = null,
    //_utils = require("../../lib/utils");

module.exports = {

	NowPlayingRequestPlayback: function(success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingRequestPlayback(), false);
	},

    NowPlayingBindPlayCallback: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        resultObjs[result.callbackId] = result;
        nowPlaying.getInstance().NowPlayingBindPlayCallback(result.callbackId);
        result.noResult(true);
    },

    NowPlayingBindPauseCallback: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        resultObjs[result.callbackId] = result;
        nowPlaying.getInstance().NowPlayingBindPauseCallback(result.callbackId);
        result.noResult(true);
    },

    NowPlayingBindStopCallback: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        resultObjs[result.callbackId] = result;
        nowPlaying.getInstance().NowPlayingBindStopCallback(result.callbackId);
        result.noResult(true);
    },

    NowPlayingBindNextCallback: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        resultObjs[result.callbackId] = result;
        nowPlaying.getInstance().NowPlayingBindNextCallback(result.callbackId);
        result.noResult(true);
    },

    NowPlayingBindPreviousCallback: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        resultObjs[result.callbackId] = result;
        nowPlaying.getInstance().NowPlayingBindPreviousCallback(result.callbackId);
        result.noResult(true);
    },

    NowPlayingBindErrorCallback: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        resultObjs[result.callbackId] = result;
        nowPlaying.getInstance().NowPlayingBindErrorCallback(result.callbackId);
        result.noResult(true);
    },

	NowPlayingPlay: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        args = JSON.parse(decodeURIComponent(args["input"]));
        result.ok(nowPlaying.getInstance().NowPlayingPlay(args), false);
	},

    NowPlayingPause: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingPause(), false);
    },

    NowPlayingResume: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingResume(), false);
    },

    NowPlayingStop: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingStop(), false);
    },

    NowPlayingNext: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingNext(), false);
    },

    NowPlayingPrevious: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingPrevious(), false);
    },

    NowPlayingError: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingError(), false);
    },

    NowPlayingGetState: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingGetState(), false);
    }
};


///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.NowPlaying = function(){
	var self = this,
	hasInstance = false;

    self.m_id = "";

	self.init = function () {
		if (!JNEXT.require("libNowPlaying")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libNowPlaying.NowPlayingJS");
		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);

        /* Find the window group the app resides in, so that it can be passed
         * as an argument and so the NowPlayingNDK application thread can run
         * in the same group. This enables system notifications, in
         * particular from the media notification area which is part of the
         * window group the app resides in, to be picked up and handled by the
         * application thread. The native layer should invoke subsequent
         * methods from the context of the application thread window group. */
        var views = qnx.webplatform.getWebViews();
        var windowGroup = null;
        var z = -1;
        for (var i = 0; i < views.length; i++) {
            if (views[i].visible && views[i].zOrder > z){
                z = views[i].zOrder;
                windowGroup = views[i].windowGroup;
            }
        }

        JNEXT.invoke(self.m_id, "joinSlot" + " " + JSON.stringify(windowGroup));
	};

    self.getId = function () {
        return self.m_id;
    };

    self.getInstance = function () {
        if (!hasInstance) {
            hasInstance = true;
            self.init();
        }
        return self;
    };

    // Fired by the Event framework (used by asynchronous callbacks)
    self.onEvent = function (strData) {
        var arData = strData.split(" "),
            callbackId = arData[0],
            result = resultObjs[callbackId],
            data = arData.slice(1, arData.length).join(" ");

        if (result) {
            // Keep the callbacks we set up:
            // don't delete them here as this commented out code would.
            //if (callbackId != threadCallback) {
            //	result.callbackOk(data, false);
            //	delete resultObjs[callbackId];
            //} else {
            result.callbackOk(data, true);
            //}
        }
    };

	/**
     * Below are calls into InvokeMethod(string command) in nowplaying_js.cpp
     * See how the command parameter is parsed to determine how to format its arguments.
     */

    self.NowPlayingRequestPlayback = function () {
        return JNEXT.invoke(self.m_id, "NowPlayingRequestPlayback");
    };

    self.NowPlayingBindPlayCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindPlayCallback" + " " + callbackId);
    };

    self.NowPlayingBindPauseCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindPauseCallback" + " " + callbackId);
    };

    self.NowPlayingBindStopCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindStopCallback" + " " + callbackId);
    };

    self.NowPlayingBindNextCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindNextCallback" + " " + callbackId);
    };

    self.NowPlayingBindPreviousCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindPreviousCallback" + " " + callbackId);
    };

    self.NowPlayingBindErrorCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindErrorCallback" + " " + callbackId);
    };

	self.NowPlayingPlay = function (input) {
		return JNEXT.invoke(self.m_id, "NowPlayingPlay" +  " {} " + JSON.stringify(input));
	};

    self.NowPlayingPause = function () {
        return JNEXT.invoke(self.m_id, "NowPlayingPause");
    };

    self.NowPlayingResume = function () {
        return JNEXT.invoke(self.m_id, "NowPlayingResume");
    };

    self.NowPlayingStop = function () {
        return JNEXT.invoke(self.m_id, "NowPlayingStop");
    };

    self.NowPlayingNext = function () {
        return JNEXT.invoke(self.m_id, "NowPlayingNext");
    };

    self.NowPlayingPrevious = function () {
        return JNEXT.invoke(self.m_id, "NowPlayingPrevious");
    };

    self.NowPlayingError = function () {
        return JNEXT.invoke(self.m_id, "NowPlayingError");
    };

	self.NowPlayingGetState = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingGetState");
	};
};

nowPlaying = new JNEXT.NowPlaying();
