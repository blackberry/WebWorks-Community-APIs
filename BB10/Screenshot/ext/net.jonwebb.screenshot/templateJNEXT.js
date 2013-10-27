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

console.log('templateJNEXT');

var screenshot,
	_event = require("../../lib/event");

JNEXT.Template = function () {

	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libScreenshot")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libScreenshot.Screenshot");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// native methods
	self.hello = function (args) {
		return JNEXT.invoke(self.m_id, "hello "+JSON.stringify(args));
	};
	self.execute = function (args) {	
		return JNEXT.invoke(self.m_id, "execute "+JSON.stringify(args));
	};
	// end of native methods

	self.m_id = "";

	self.getInstance = function () {
		if (!hasInstance) {
			hasInstance = true;
			self.init();
		}
		return self;
	};

	// native events 
	self.onEvent = function (strData) {
		console.log(strData);
	};
	// end of native events

};

template = new JNEXT.Template();

module.exports = {
	template: template
};