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

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

var template,
	_event = require("../../lib/event");

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
	self.testInput = function (input) {
		return JNEXT.invoke(self.m_id, "testStringInput " + input);
	};
	self.testAsync = function () {
		return JNEXT.invoke(self.m_id, "testAsync");
	};
	self.testAsyncJSON = function () {
		return JNEXT.invoke(self.m_id, "testAsyncJSON");
	};
	self.testAsyncJSONio = function (input) {
		return JNEXT.invoke(self.m_id, "testAsyncJSONio " + JSON.stringify(input));
	};
	self.templateProperty = function (value) {
		if (value) {
			return JNEXT.invoke(self.m_id, "templateProperty " + value);
		} else {
			return JNEXT.invoke(self.m_id, "templateProperty");
		}
	};
	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			strEventDesc = arData[0],
			jsonData;
		// Event names are set in native code when fired,
		// and must be checked here.
		if (strEventDesc === "community.templateExt.aSyncCallback") {
			_event.trigger("community.templateExt.aSyncCallback");
		} else if (strEventDesc === "community.templateExt.aSyncJSONCallback") {
			// Slice off the event name and the rest of the data is our JSON
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.templateExt.aSyncJSONCallback", JSON.parse(jsonData));
		} else if (strEventDesc === "community.templateExt.aSyncJSONCallbackResult") {
			// Slice off the event name and the rest of the data is our JSON
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.templateExt.aSyncJSONCallbackResult", JSON.parse(jsonData));
		} else if (strEventDesc === "community.templateExt.jsonThreadCallback") {
			// Slice off the event name and the rest of the data is our JSON
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.templateExt.jsonThreadEvent", JSON.parse(jsonData));
		}
	};

	// Thread methods
	self.startThread = function () {
		return JNEXT.invoke(self.m_id, "templateStartThread");
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

module.exports = {
	template: template
};
