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

var setlockscreenwallpaper,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.Setlockscreenwallpaper which handles the
	// communication through the JNEXT plugin to setlockscreenwallpaper_js.cpp
	ping: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(setlockscreenwallpaper.getInstance().ping(), false);
	},
	setlockscreenwallpaper: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		args = args.replace(/file:\/\//, '');
		result.ok(setlockscreenwallpaper.getInstance().setlockscreenwallpaper(result.callbackId, args), false);
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.Setlockscreenwallpaper = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libSetlockscreenwallpaper")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libSetlockscreenwallpaper.Setlockscreenwallpaper_JS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in setlockscreenwallpaper_js.cpp
	self.ping = function () {
		return JNEXT.invoke(self.m_id, "ping");
	};
	self.setlockscreenwallpaper = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "setlockscreenwallpaper " + callbackId + " " + input);
	};
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

setlockscreenwallpaper = new JNEXT.Setlockscreenwallpaper();
