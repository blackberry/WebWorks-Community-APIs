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

var resultObjs = {},
    threadCallback = null,
    _utils = require("../../lib/utils");

module.exports = {

	NowPlayingConnectionTest: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(nowPlaying.getInstance().NowPlayingConnectionTest(), false);
	},

	NowPlayingPlay: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(nowPlaying.getInstance().NowPlayingPlay(), false);
	},

	NowPlayingStop: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(nowPlaying.getInstance().NowPlayingStop(), false);
	},

	NowPlayingPause: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(nowPlaying.getInstance().NowPlayingPause(), false);
	},

	NowPlayingGetSourceAddress: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(nowPlaying.getInstance().NowPlayingGetSourceAddress(), false);
	},

	NowPlayingGetDuration: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(nowPlaying.getInstance().NowPlayingGetDuration(), false);
	},

	NowPlayingGetPosition: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(nowPlaying.getInstance().NowPlayingGetPosition(), false);
	},

	NowPlayingSwitchMusic: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(nowPlaying.getInstance().NowPlayingSwitchMusic(result.callbackId, args), false);
	},

	testAsync:  function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		args = JSON.parse(decodeURIComponent(args["input"]));
		nowPlaying.getInstance().testAsync(result.callbackId, args);
		result.noResult(true);
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
	self.NowPlayingConnectionTest = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingConnectionTest");
	};
	
	self.NowPlayingPlay = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingPlay");
	};

	self.NowPlayingStop = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingStop");
	};

	self.NowPlayingPause = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingPause");
	};

	self.NowPlayingGetSourceAddress = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingGetSourceAddress");
	};

	self.NowPlayingGetDuration = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingGetDuration");
	};

	self.NowPlayingGetPosition = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingGetPosition");
	};

	self.NowPlayingSwitchMusic = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "NowPlayingSwitchMusic " + callbackId + " " + input);
	};

	self.testAsync = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "testAsync " + callbackId + " " + JSON.stringify(input));
	};

	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");

		if (result) {
			if (callbackId != threadCallback) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
			} else {
				result.callbackOk(data, true);
			}
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
