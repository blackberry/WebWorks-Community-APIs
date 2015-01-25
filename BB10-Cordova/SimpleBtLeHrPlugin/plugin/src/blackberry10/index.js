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

var simpleBtLeHrPlugin,
	resultObjs = {},
	scanCallbackId = null,
	gattCallbackId = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.SimpleBtLeHrPlugin which handles the
	// communication through the JNEXT plugin to simplebtlehrplugin_js.cpp

	initialiseBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBtLeHrPlugin.getInstance().initialiseBluetooth(), false);
	},

	terminateBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBtLeHrPlugin.getInstance().terminateBluetooth(), false);
	},

	bluetoothAddress: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			simpleBtLeHrPlugin.getInstance().bluetoothAddress(result.callbackId, value);
			result.noResult(false);
		} else {
			result.ok(simpleBtLeHrPlugin.getInstance().bluetoothAddress(), false);
		}
	},

	startDeviceScan: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!scanCallbackId) {
			scanCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleBtLeHrPlugin.getInstance().startDeviceScan(result.callbackId), true);
		} else {
			result.error(simpleBtLeHrPlugin.getInstance().startDeviceScan(result.callbackId), false);
		}
	},

	pluginVersion: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBtLeHrPlugin.getInstance().pluginVersion(), false);
	},

	startMonitoringHr: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!gattCallbackId) {
			gattCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleBtLeHrPlugin.getInstance().startMonitoringHr(result.callbackId), true);
		} else {
			result.error(simpleBtLeHrPlugin.getInstance().startMonitoringHr(result.callbackId), false);
		}
	},

	stopMonitoringHr: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (gattCallbackId) {
			delete resultObjs[gattCallbackId];
			gattCallbackId = null;
			result.ok(simpleBtLeHrPlugin.getInstance().stopMonitoringHr(), false);
		} else {
			result.error("Stop Monitoring failure", false);
		}
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.SimpleBtLeHrPlugin = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libSimpleBtLeHrPlugin")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libSimpleBtLeHrPlugin.SimpleBtLeHrPlugin_JS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in simpleBtLeHrPlugin_js.cpp

	// start of SimpleBtLeHrPlugin apis

	self.initialiseBluetooth = function () {
		return JNEXT.invoke(self.m_id, "initialiseBluetooth");
	};

	self.terminateBluetooth = function () {
		return JNEXT.invoke(self.m_id, "terminateBluetooth");
	};

	self.bluetoothAddress = function (callbackId, value) {
		if (value) {
			return JNEXT.invoke(self.m_id, "bluetoothAddress " + callbackId + " " + value);
		} else {
			return JNEXT.invoke(self.m_id, "bluetoothAddress");
		}
	};

	self.startDeviceScan = function (callbackId) {
		return JNEXT.invoke(self.m_id, "startDeviceScan " + callbackId);
	};

	self.pluginVersion = function () {
		return JNEXT.invoke(self.m_id, "pluginVersion");
	};

	self.startMonitoringHr = function (callbackId) {
		return JNEXT.invoke(self.m_id, "startMonitoringHr " + callbackId);
	};

	self.stopMonitoringHr = function () {
		return JNEXT.invoke(self.m_id, "stopMonitoringHr");
	};

	// end of simpleBtLeHrPlugin apis

	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");

		if (result) {
			if (callbackId == gattCallbackId) {
				result.callbackOk(data, true);
			} else if (callbackId == scanCallbackId) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
				scanCallbackId = null;
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

simpleBtLeHrPlugin = new JNEXT.SimpleBtLeHrPlugin();
