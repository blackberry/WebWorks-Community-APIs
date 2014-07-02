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

var audiorec,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	record: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(audiorec.getInstance().record(), false);
	},
	
	setOutput: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(audiorec.getInstance().setOutput(result.callbackId, args), false);
	},
	
	pause: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(audiorec.getInstance().pause(), false);
	},
	
	reset: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(audiorec.getInstance().reset(), false);
	},
	
	mediaState: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(audiorec.getInstance().mediaState(), false);
	},
	
	duration: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(audiorec.getInstance().duration(), false);
	}
};
	
	// These methods call into JNEXT.Template which handles the
	// communication through the JNEXT plugin to audiorec_js.cpp
	

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.Audiorec = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libAudio")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libAudio.AudiorecJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in audiorec_js.cpp
	
	self.record = function () {
		return JNEXT.invoke(self.m_id, "record");
	};
	
	self.setOutput = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "setOutput " + callbackId + " " + input);
	};
	
	self.pause = function () {
		return JNEXT.invoke(self.m_id, "pause");
	};
	
	self.reset = function () {
		return JNEXT.invoke(self.m_id, "reset");
	};
	
	self.mediaState = function () {
		return JNEXT.invoke(self.m_id, "mediaState");
	};
	
	self.duration = function () {
		return JNEXT.invoke(self.m_id, "duration");
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

audiorec = new JNEXT.Audiorec();