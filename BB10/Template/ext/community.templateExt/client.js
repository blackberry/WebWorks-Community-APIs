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

var _self = {},
	_ID = require("./manifest.json").namespace;

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	// Simple Synchronous test function to get a string
	_self.test = function () {
		return window.webworks.execSync(_ID, "test", null);
	};
	_self.testInput = function (input) {
		return window.webworks.execSync(_ID, "testInput", { input: input });
	};

	// Basic Asynchronous test function
	// Sets up a one time call back, and then calls into the index
	_self.testAsync = function (callback) {
		window.webworks.event.once(_ID, "community.templateExt.aSyncCallback", callback);
		return window.webworks.execAsync(_ID, "testAsync", null);
	};

	// Asynchronous with returning a JSON object
	_self.testAsyncJSON = function (callback) {
		window.webworks.event.once(_ID, "community.templateExt.aSyncJSONCallback", callback);
		return window.webworks.execAsync(_ID, "testAsyncJSON", null);
	};

	// Asynchronous with sending and returning a JSON object
	_self.testAsyncJSONio = function (input, callback) {
		window.webworks.event.once(_ID, "community.templateExt.aSyncJSONCallbackResult", callback);
		return window.webworks.execAsync(_ID, "testAsyncJSONio", { input: input });
	};

	// Define a property on the extension object
	// Omit the getter or setter as needed to restrict the property
	Object.defineProperty(_self, "templateProperty", {
		get: function () {
			return window.webworks.execSync(_ID, "templateProperty", null);
		},
		set: function (arg) {
			window.webworks.execSync(_ID, "templateProperty", {"value": arg });
		}
	});

	_self.startThread = function (callback) {
		window.webworks.event.add(_ID, "community.templateExt.jsonThreadEvent", callback);
		return window.webworks.execSync(_ID, "startThread", null);
	};

	_self.stopThread = function (callback) {
		window.webworks.event.remove(_ID, "community.templateExt.jsonThreadEvent", callback);
		return window.webworks.execSync(_ID, "stopThread", null);
	};

module.exports = _self;
