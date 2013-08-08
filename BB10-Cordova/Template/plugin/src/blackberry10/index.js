/*
* Copyright 2013 Research In Motion Limited.
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

	// These methods call into JNEXT.Template which handles the
	// communication through the JNEXT plugin to template_js.cpp
	test: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(template.getInstance().test(), false);
	},
	testInput: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(template.getInstance().testInput(result.callbackId, args), false);
	},
	// Asynchronous function calls into the plugin and returns
	testAsync: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		args = JSON.parse(decodeURIComponent(args["input"]));
		template.getInstance().testAsync(result.callbackId, args);
		result.noResult(true);
	},
	templateProperty: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			template.getInstance().templateProperty(result.callbackId, value);
			result.noResult(false);
		} else {
			result.ok(template.getInstance().templateProperty(), false);
		}
	},
	// Thread methods to start and stop
	startThread: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!threadCallback) {
			threadCallback = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(template.getInstance().startThread(result.callbackId), true);
		} else {
			result.error(template.getInstance().startThread(result.callbackId), false);
		}
	},
	stopThread: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!threadCallback) {
			result.error("Thread is not running", false);
		} else {
			delete resultObjs[threadCallback];
			threadCallback = null;
			result.ok(template.getInstance().stopThread(), false);
		}
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
		if (!JNEXT.require("libTemplate")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libTemplate.TemplateJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in template_js.cpp
	self.test = function () {
		return JNEXT.invoke(self.m_id, "testString");
	};
	self.testInput = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "testStringInput " + callbackId + " " + input);
	};
	self.testAsync = function (callbackId, input) {
		return JNEXT.invoke(self.m_id, "testAsync " + callbackId + " " + JSON.stringify(input));
	};
	self.templateProperty = function (callbackId, value) {
		if (value) {
			return JNEXT.invoke(self.m_id, "templateProperty " + callbackId + " " + value);
		} else {
			return JNEXT.invoke(self.m_id, "templateProperty");
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
	self.startThread = function (callbackId) {
		return JNEXT.invoke(self.m_id, "templateStartThread " + callbackId);
	};
	self.stopThread = function () {
		return JNEXT.invoke(self.m_id, "templateStopThread");
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