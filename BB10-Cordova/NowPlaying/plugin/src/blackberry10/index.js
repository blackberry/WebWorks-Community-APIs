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

    //NowPlayingSetMusic: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	args = JSON.parse(decodeURIComponent(args["input"]));
	//	result.ok(nowPlaying.getInstance().NowPlayingSetMusic(args), false);
	//},
    //
	//NowPlayingSetMetadata: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	args = JSON.parse(decodeURIComponent(args["input"]));
	//	result.ok(nowPlaying.getInstance().NowPlayingSetMetadata(args), false);
	//},
    //
	//NowPlayingSetIcon: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	args = JSON.parse(decodeURIComponent(args["input"]));
	//	result.ok(nowPlaying.getInstance().NowPlayingSetIcon(args), false);
	//},
    //
    //
	//NowPlayingChangeTrack: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	resultObjs[result.callbackId] = result;
	//	args = JSON.parse(decodeURIComponent(args["input"]));
	//	result.ok(nowPlaying.getInstance().NowPlayingChangeTrack(result.callbackId, args), false);
	//},
    //
    //
	//NowPlayingEnableNextPrevious: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	result.ok(nowPlaying.getInstance().NowPlayingEnableNextPrevious(), false);
	//},
    //
	//NowPlayingDisableNextPrevious: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	result.ok(nowPlaying.getInstance().NowPlayingDisableNextPrevious(), false);
	//},

	NowPlayingPlay: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(nowPlaying.getInstance().NowPlayingPlay(), false);
	},

	//NowPlayingPause: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	resultObjs[result.callbackId] = result;
	//	result.ok(nowPlaying.getInstance().NowPlayingPause(result.callbackId), false);
	//},
    //
	//NowPlayingResume: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
	//	resultObjs[result.callbackId] = result;
	//	result.ok(nowPlaying.getInstance().NowPlayingResume(result.callbackId), false);
	//},
    //
	//NowPlayingStop: function (success, fail, args, env) {
	//	var result = new PluginResult(args, env);
     //   resultObjs[result.callbackId] = result;
	//	result.ok(nowPlaying.getInstance().NowPlayingStop(result.callbackId), false);
	//},


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
        return JNEXT.invoke(self.m_id, "NowPlayingRequestPlayback " + "{}" + " " + JSON.stringify(input));
    };

    self.NowPlayingBindPlayCallback = function (callbackId) {
        return JNEXT.invoke(self.m_id, "NowPlayingBindPlayCallback " + callbackId);
    };

    //self.NowPlayingSetMusic = function (input) {
	//	return JNEXT.invoke(self.m_id, "NowPlayingSetMusic " + "{}" + " " + input);
	//};
    //
	//self.NowPlayingSetMetadata = function (input) {
	//	return JNEXT.invoke(self.m_id, "NowPlayingSetMetadata " + "{}" + " " + JSON.stringify(input));
	//};
    //
	//self.NowPlayingSetIcon = function (input) {
	//	return JNEXT.invoke(self.m_id, "NowPlayingSetIcon " + "{}" + " " + input);
	//};
    //
    //
	//self.NowPlayingChangeTrack = function (callbackId, input) {
	//	return JNEXT.invoke(self.m_id, "NowPlayingChangeTrack " + callbackId + " " + JSON.stringify(input));
	//};
    //
    //
	//self.NowPlayingEnableNextPrevious = function () {
	//	return JNEXT.invoke(self.m_id, "NowPlayingEnableNextPrevious");
	//};
    //
	//self.NowPlayingDisableNextPrevious = function () {
	//	return JNEXT.invoke(self.m_id, "NowPlayingDisableNextPrevious");
	//};


	self.NowPlayingPlay = function () {
		return JNEXT.invoke(self.m_id, "NowPlayingPlay");
	};

	//self.NowPlayingPause = function (callbackId) {
	//	return JNEXT.invoke(self.m_id, "NowPlayingPause " + callbackId);
	//};
    //
	//self.NowPlayingResume = function (callbackId) {
	//	return JNEXT.invoke(self.m_id, "NowPlayingResume " + callbackId);
	//};
    //
	//self.NowPlayingStop = function (callbackId) {
	//	return JNEXT.invoke(self.m_id, "NowPlayingStop " + callbackId);
	//};


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
