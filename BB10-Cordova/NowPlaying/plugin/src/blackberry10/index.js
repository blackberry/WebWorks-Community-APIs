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

var ???PROJECT_FUNCTION_START???,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.???PROJECT_NAME??? which handles the
	// communication through the JNEXT plugin to ???PROJECT_NAME_LOWERCASE???_js.cpp
	???PROJECT_FUNCTION_START???Test: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???Test(), false);
	},
	???PROJECT_FUNCTION_START???TestInput: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???TestInput(result.callbackId, args), false);
	},
	// Asynchronous function calls into the plugin and returns
	???PROJECT_FUNCTION_START???TestAsync: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		args = JSON.parse(decodeURIComponent(args["input"]));
		???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???TestAsync(result.callbackId, args);
		result.noResult(true);
	},
	???PROJECT_FUNCTION_START???Property: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???Property(result.callbackId, value);
			result.noResult(false);
		} else {
			result.ok(???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???Property(), false);
		}
	},
	// Thread methods to start and stop
	???PROJECT_FUNCTION_START???StartThread: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!threadCallback) {
			threadCallback = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???StartThread(result.callbackId), true);
		} else {
			result.error(???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???StartThread(result.callbackId), false);
		}
	},
	???PROJECT_FUNCTION_START???StopThread: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!threadCallback) {
			result.error("Thread is not running", false);
		} else {
			delete resultObjs[threadCallback];
			threadCallback = null;
			result.ok(???PROJECT_FUNCTION_START???.getInstance().???PROJECT_FUNCTION_START???StopThread(), false);
		}
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.???PROJECT_NAME??? = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("lib???PROJECT_NAME???")) {
			return false;
		}

		self.m_id = JNEXT.createObject("lib???PROJECT_NAME???.???PROJECT_NAME???_JS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in ???PROJECT_NAME_LOWERCASE???_js.cpp
	self.???PROJECT_FUNCTION_START???Test = function () {
		return JNEXT.invoke(self.m_id, "???PROJECT_FUNCTION_START???Test");
	};
	self.???PROJECT_FUNCTION_START???TestInput = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "???PROJECT_FUNCTION_START???TestInput " + callbackId + " " + input);
	};
	self.???PROJECT_FUNCTION_START???TestAsync = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "???PROJECT_FUNCTION_START???TestAsync " + callbackId + " " + JSON.stringify(input));
	};
	self.???PROJECT_FUNCTION_START???Property = function (callbackId, value) {
		if (value) {
			return JNEXT.invoke(self.m_id, "???PROJECT_FUNCTION_START???Property " + callbackId + " " + value);
		} else {
			return JNEXT.invoke(self.m_id, "???PROJECT_FUNCTION_START???Property");
		}
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

	// Thread methods
	self.???PROJECT_FUNCTION_START???StartThread = function (callbackId) {
		return JNEXT.invoke(self.m_id, "???PROJECT_FUNCTION_START???StartThread " + callbackId);
	};
	self.???PROJECT_FUNCTION_START???StopThread = function () {
		return JNEXT.invoke(self.m_id, "???PROJECT_FUNCTION_START???StopThread");
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

???PROJECT_FUNCTION_START??? = new JNEXT.???PROJECT_NAME???();
