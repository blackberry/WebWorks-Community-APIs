/*
 * Copyright (c) 2014 BlackBerry Limited
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
 
var simpleBtSppPlugin,
	resultObjs = {},
	scanCallbackId = null,
	connectCallbackId = null,
	listenCallbackId = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.SimpleBtSppPlugin which handles the
	// communication through the JNEXT plugin to simplebtsppplugin_js.cpp
	
	initialiseBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBtSppPlugin.getInstance().initialiseBluetooth(), false);
	},

	terminateBluetooth: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBtSppPlugin.getInstance().terminateBluetooth(), false);
	},

	bluetoothAddress: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			simpleBtSppPlugin.getInstance().bluetoothAddress(result.callbackId, value);
			result.noResult(false);
		} else {
			result.ok(simpleBtSppPlugin.getInstance().bluetoothAddress(), false);
		}
	},

	startDeviceScan: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!scanCallbackId) {
			scanCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleBtSppPlugin.getInstance().startDeviceScan(result.callbackId), true);
		} else {
			result.error(simpleBtSppPlugin.getInstance().startDeviceScan(result.callbackId), false);
		}
	},

	connectToDevice: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!connectCallbackId) {
			connectCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleBtSppPlugin.getInstance().connectToDevice(result.callbackId), true);
		} else {
			result.error(simpleBtSppPlugin.getInstance().connectToDevice(result.callbackId), false);
		}
	},

	disconnectFromDevice: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (connectCallbackId) {
			delete resultObjs[connectCallbackId];
			connectCallbackId = null;
			result.ok(simpleBtSppPlugin.getInstance().disconnectFromDevice(), false);
		} else if (listenCallbackId) {
			result.ok(simpleBtSppPlugin.getInstance().disconnectFromDevice(), false);
		} else {
			result.error("Disconnect failure", false);
		}
	},

    sendMessage: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        args = JSON.parse(decodeURIComponent(args["input"]));
        result.ok(simpleBtSppPlugin.getInstance().sendMessage(result.callbackId, args), false);
    },

	listenForConnection: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!listenCallbackId) {
			listenCallbackId = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(simpleBtSppPlugin.getInstance().listenForConnection(result.callbackId), true);
		} else {
			result.error(simpleBtSppPlugin.getInstance().listenForConnection(result.callbackId), false);
		}
	},

	stopListening: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!listenCallbackId) {
			result.error("Listener is not running", false);
		} else {
			delete resultObjs[listenCallbackId];
			listenCallbackId = null;
			result.ok(simpleBtSppPlugin.getInstance().stopListening(), false);
		}
	},

	pluginVersion: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(simpleBtSppPlugin.getInstance().pluginVersion(), false);
	},

	sppServiceUuid: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			simpleBtSppPlugin.getInstance().sppServiceUuid(result.callbackId, value);
			result.noResult(false);
		} else {
			result.ok(simpleBtSppPlugin.getInstance().sppServiceUuid(), false);
		}
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.SimpleBtSppPlugin = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libSimpleBtSppPlugin")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libSimpleBtSppPlugin.SimpleBtSppPlugin_JS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in simplebtsppplugin_js.cpp
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

	self.connectToDevice = function (callbackId) {
		return JNEXT.invoke(self.m_id, "connectToDevice " + callbackId);
	};

	self.disconnectFromDevice = function () {
		return JNEXT.invoke(self.m_id, "disconnectFromDevice");
	};

	self.sendMessage = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "sendMessage " + callbackId + " " + input);
	};
	
	self.listenForConnection = function (callbackId) {
		return JNEXT.invoke(self.m_id, "listenForConnection " + callbackId);
	};

	self.stopListening = function () {
		return JNEXT.invoke(self.m_id, "stopListening");
	};

	self.pluginVersion = function () {
		return JNEXT.invoke(self.m_id, "pluginVersion");
	};

	self.sppServiceUuid = function (callbackId, value) {
		if (value) {
			return JNEXT.invoke(self.m_id, "sppServiceUuid " + callbackId + " " + value);
		} else {
			return JNEXT.invoke(self.m_id, "sppServiceUuid");
		}
	};

	// end of bluetooth apis

	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");

		if (result) {
			if (callbackId == connectCallbackId) {
				result.callbackOk(data, true);
			} else if (callbackId == listenCallbackId) {
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

simpleBtSppPlugin = new JNEXT.SimpleBtSppPlugin();
