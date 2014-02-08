/*
* Copyright (c) 2013 - 2014 BlackBerry Limited
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

var deviceInfo,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.Template which handles the
	// communication through the JNEXT plugin to template_js.cpp
	getModelNumber: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(deviceInfo.getInstance().getModelNumber(), false);
	},
	isSimulator: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(deviceInfo.getInstance().isSimulator(), false);
	},
	getRoamingStatus: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(deviceInfo.getInstance().getRoamingStatus(), false);
	},
	hasPhysicalKeyboard: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(deviceInfo.getInstance().hasPhysicalKeyboard(), false);
	},
	getNetwork: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(deviceInfo.getInstance().getNetwork(), false);
	},
	getMCC: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(deviceInfo.getInstance().getMCC(), false);
	},
	getMNC: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(deviceInfo.getInstance().getMNC(), false);
	},
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.DeviceInfo = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libDeviceinfo")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libDeviceinfo.DeviceinfoJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in template_js.cpp
	self.getModelNumber = function () {
		return JNEXT.invoke(self.m_id, "getModelNumber");
	};

	self.isSimulator = function () {
		return JNEXT.invoke(self.m_id, "isSimulator");
	};

	self.getRoamingStatus = function () {
		return JNEXT.invoke(self.m_id, "getRoamingStatus");
	};

	self.hasPhysicalKeyboard = function () {
		return JNEXT.invoke(self.m_id, "hasPhysicalKeyboard");
	};

	self.getNetwork = function () {
		return JNEXT.invoke(self.m_id, "getNetwork");
	};

	self.getMCC = function () {
		return JNEXT.invoke(self.m_id, "getMCC");
	};

	self.getMNC = function () {
		return JNEXT.invoke(self.m_id, "getMNC");
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

deviceInfo = new JNEXT.DeviceInfo();