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

var simpleBeaconPlugin,
	resultObjs = {},
	beaconCallbackId = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.SimpleBeaconPlugin which handles the
	// communication through the JNEXT plugin to simplebeaconplugin_js.cpp

	initialiseBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBeaconPlugin.getInstance().initialiseBluetooth(), false);
	},

	terminateBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBeaconPlugin.getInstance().terminateBluetooth(), false);
	},

	pluginVersion: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBeaconPlugin.getInstance().pluginVersion(), false);
	},

	startMonitoring: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!beaconCallbackId) {
			beaconCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleBeaconPlugin.getInstance().startMonitoring(result.callbackId), true);
		} else {
			result.error(simpleBeaconPlugin.getInstance().startMonitoring(result.callbackId), false);
		}
	},

	stopMonitoring: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (beaconCallbackId) {
			delete resultObjs[beaconCallbackId];
			beaconCallbackId = null;
			result.ok(simpleBeaconPlugin.getInstance().stopMonitoring(), false);
		} else {
			result.error("Stop Monitoring failure", false);
		}
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.SimpleBeaconPlugin = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libSimpleBeaconPlugin")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libSimpleBeaconPlugin.SimpleBeaconPlugin_JS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in simplebeaconplugin_js.cpp

	// start of SimpleBeaconPlugin apis

	self.initialiseBluetooth = function () {
		return JNEXT.invoke(self.m_id, "initialiseBluetooth");
	};

	self.terminateBluetooth = function () {
		return JNEXT.invoke(self.m_id, "terminateBluetooth");
	};

	self.pluginVersion = function () {
		return JNEXT.invoke(self.m_id, "pluginVersion");
	};

	self.startMonitoring = function (callbackId) {
		return JNEXT.invoke(self.m_id, "startMonitoring " + callbackId);
	};

	self.stopMonitoring = function () {
		return JNEXT.invoke(self.m_id, "stopMonitoring");
	};

	// end of SimpleBeaconPlugin apis


	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");

		if (result) {
			if (callbackId == beaconCallbackId) {
				result.callbackOk(data, true);
			} else {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
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

simpleBeaconPlugin = new JNEXT.SimpleBeaconPlugin();
