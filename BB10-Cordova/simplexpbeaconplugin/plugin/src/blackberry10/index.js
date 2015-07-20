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

var simpleXpBeaconPlugin,
	resultObjs = {},
	beaconCallbackId = null,
	pluginVersionCallbackId = null,
	initialiseBluetoothCallbackId = null,
	terminateBluetoothCallbackId = null,
	stopMonitoringCallbackId = null,
	removeBeaconUuidToMonitorCallbackId = null,
	addBeaconUuidToMonitorCallbackId = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.SimpleXpBeaconPlugin which handles the
	// communication through the JNEXT plugin to simpleXpbeaconplugin_js.cpp

	initialiseBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!initialiseBluetoothCallbackId) {
			initialiseBluetoothCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleXpBeaconPlugin.getInstance().initialiseBluetooth(result.callbackId), true);
		} else {
			result.error("Initialise Bluetooth failure", false);
		}
	},

	terminateBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!terminateBluetoothCallbackId) {
			terminateBluetoothCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleXpBeaconPlugin.getInstance().terminateBluetooth(result.callbackId), true);
		} else {
			result.error("Terminate Bluetooth failure", false);
		}
	},

	pluginVersion: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!pluginVersionCallbackId) {
			pluginVersionCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleXpBeaconPlugin.getInstance().pluginVersion(result.callbackId), true);
		} else {
			result.error("Plugin Version failure", false);
		}
	},

	startMonitoring: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!beaconCallbackId) {
			beaconCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleXpBeaconPlugin.getInstance().startMonitoring(result.callbackId), true);
		} else {
			result.error(simpleXpBeaconPlugin.getInstance().startMonitoring(result.callbackId), false);
		}
	},

	stopMonitoring: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!stopMonitoringCallbackId) {
			stopMonitoringCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleXpBeaconPlugin.getInstance().stopMonitoring(result.callbackId), true);
		} else {
			result.error("Stop Monitoring failure", false);
		}
	},

	addBeaconUuidToMonitor: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
	    uuid = JSON.parse(decodeURIComponent(args[0]));
		if (!addBeaconUuidToMonitorCallbackId) {
			addBeaconUuidToMonitorCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleXpBeaconPlugin.getInstance().addBeaconUuidToMonitor(result.callbackId, uuid), true);
		} else {
			result.error("Add Beacon UUID failure", false);
		}
	},

	removeBeaconUuidToMonitor: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
	    uuid = JSON.parse(decodeURIComponent(args[0]));
		if (!removeBeaconUuidToMonitorCallbackId) {
			removeBeaconUuidToMonitorCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleXpBeaconPlugin.getInstance().removeBeaconUuidToMonitor(result.callbackId, uuid), true);
		} else {
			result.error("Add Beacon UUID failure", false);
		}
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.SimpleXpBeaconPlugin = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libSimpleXpBeaconPlugin")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libSimpleXpBeaconPlugin.SimpleXpBeaconPlugin_JS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in simplexpbeaconplugin_js.cpp

	// start of SimpleXpBeaconPlugin apis

	self.initialiseBluetooth = function (callbackId) {
		return JNEXT.invoke(self.m_id, "initialiseBluetooth " + callbackId);
	};

	self.terminateBluetooth = function (callbackId) {
		return JNEXT.invoke(self.m_id, "terminateBluetooth " + callbackId);
	};

	self.pluginVersion = function (callbackId) {
		return JNEXT.invoke(self.m_id, "pluginVersion " + callbackId);
	};

	self.startMonitoring = function (callbackId) {
		return JNEXT.invoke(self.m_id, "startMonitoring " + callbackId);
	};

	self.stopMonitoring = function (callbackId) {
		return JNEXT.invoke(self.m_id, "stopMonitoring " + callbackId);
	};

	self.addBeaconUuidToMonitor = function (callbackId, uuid) {
		return JNEXT.invoke(self.m_id, "addBeaconUuidToMonitor " + callbackId + " " + uuid);
	};
	
	self.removeBeaconUuidToMonitor = function (callbackId, uuid) {
		return JNEXT.invoke(self.m_id, "removeBeaconUuidToMonitor " + callbackId + " " + uuid);
	};

	// end of SimpleXpBeaconPlugin apis


	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");

		if (result) {
			if (callbackId == beaconCallbackId) {
				result.callbackOk(data, true);
			} else if (callbackId == pluginVersionCallbackId) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
				pluginVersionCallbackId = null;
			} else if (callbackId == initialiseBluetoothCallbackId) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
				initialiseBluetoothCallbackId = null;
			} else if (callbackId == terminateBluetoothCallbackId) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
				terminateBluetoothCallbackId = null;
			} else if (callbackId == stopMonitoringCallbackId) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
				stopMonitoringCallbackId = null;
				resultObjs[beaconCallbackId].callbackOk("", false)
				delete resultObjs[beaconCallbackId];
				beaconCallbackId = null;
			} else if (callbackId == addBeaconUuidToMonitorCallbackId) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
				addBeaconUuidToMonitorCallbackId = null;
			} else if (callbackId == removeBeaconUuidToMonitorCallbackId) {
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
				removeBeaconUuidToMonitorCallbackId = null;
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

simpleXpBeaconPlugin = new JNEXT.SimpleXpBeaconPlugin();
