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

var template,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	startLed: function (success, fail, args, env) {
		var result = new PluginResult(args, env); 
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(template.getInstance().startLed(args), false); 
	},

	stopLed: function (success, fail, args, env) {
		var result = new PluginResult(args, env); 
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(template.getInstance().stopLed(args), false); 
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.Template = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libLed")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libLed.FlashLed");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	self.startLed = function (input) {
		var color = input.color; 
		var blinkCount = input.blinkCount; 
		return JNEXT.invoke(self.m_id, "flashLedStartNative " + color + " " + blinkCount); 
	};

	self.stopLed = function (input) {
		return JNEXT.invoke(self.m_id, "flashLedStopNative " + input);
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

template = new JNEXT.Template();