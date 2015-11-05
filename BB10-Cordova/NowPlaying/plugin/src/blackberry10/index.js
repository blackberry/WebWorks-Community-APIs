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
		args = JSON.parse(decodeURIComponent(args["input"]));
        result.ok(nowPlaying.getInstance().NowPlayingRequestPlayback(args), false);
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

    NowPlayingBindResumeCallback: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        resultObjs[result.callbackId] = result;
        nowPlaying.getInstance().NowPlayingBindResumeCallback(result.callbackId);
        result.noResult(true);
    },


    NowPlayingTrackChange: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(nowPlaying.getInstance().NowPlayingTrackChange(args), false);
	},


	NowPlayingPlay: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingPlay(), false);
	},

    NowPlayingPause: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingPause(), false);
    },

    NowPlayingStop: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingStop(), false);
    },

    NowPlayingResume: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingResume(), false);
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

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libNowPlaying")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libNowPlaying.NowPlayingJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************
	// calls into InvokeMethod(string command) in nowplaying_js.cpp

    self.NowPlayingRequestPlayback = function (input) {
        return JNEXT.invoke(self.m_id, "NowPlayingRequestPlayback {} " + JSON.stringify(input));
    };

    self.NowPlayingBindPlayCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindPlayCallback " + callbackId);
    };

    self.NowPlayingBindPauseCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindPauseCallback " + callbackId);
    };

    self.NowPlayingBindStopCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindStopCallback " + callbackId);
    };

    self.NowPlayingBindResumeCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindResumeCallback " + callbackId);
    };


	self.NowPlayingTrackChange = function (input) {
		return JNEXT.invoke(self.m_id, "NowPlayingTrackChange {} " + JSON.stringify(input));
	};


	self.NowPlayingPlay = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingPlay");
	};

    self.NowPlayingPause = function () {
    return JNEXT.invoke(self.m_id, "NowPlayingPause");
    };

    self.NowPlayingStop = function () {
    return JNEXT.invoke(self.m_id, "NowPlayingStop");
    };

	self.NowPlayingResume = function (callbackId) {
		return JNEXT.invoke(self.m_id, "NowPlayingResume " + callbackId);
	};


	self.NowPlayingGetState = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingGetState");
	};


	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");

		if (result) {
			//if (callbackId != threadCallback) {
			//	result.callbackOk(data, false);
			//	delete resultObjs[callbackId];
			//} else {
				result.callbackOk(data, true);
			//}
		}
	};

	// ************************
	// End of methods to edit
	// ************************
	self.m_id = "";

	self.getInstance = function () {
		if (!hasInstance) {
			hasInstance = true;
			self.init();
		}
		return self;
	};

};


nowPlaying = new JNEXT.NowPlaying();
