/*
* Copyright (c) 2013-2014 BlackBerry Limited
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

var vibration,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.vibration which handles the
	// communication through the JNEXT plugin to vibration_js.cpp
	
	// Request vibration function
	
	vibration_request: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(vibration.getInstance().vibration_request(result.callbackId, args), true);
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.Vibration = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libVibration")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libVibration.VibrationJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in vibration_js.cpp
	self.vibration_request = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "vibration_request " + callbackId + " " + JSON.stringify(input));
	};
// Fired by the Event framework (used by asynchronous callbacks)
    self.onEvent = function (strData) {
        var arData = strData.split(" "),
            callbackId = arData[0],
			result = resultObjs[callbackId],
            data = arData.slice(1, arData.length).join(" ");
 
			result.callbackOk(data, false);
			delete resultObjs[callbackId];
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

vibration = new JNEXT.Vibration();