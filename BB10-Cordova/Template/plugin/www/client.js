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

var _self = {},
	_ID = "???CORDOVA_PLUGIN_ID???",
	exec = cordova.require("cordova/exec");

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	// Simple Synchronous test function to get a string
	_self.???PROJECT_FUNCTION_START???Test = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "???PROJECT_FUNCTION_START???Test", null);
		return result;
	};
	_self.???PROJECT_FUNCTION_START???TestInput = function (input) {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "???PROJECT_FUNCTION_START???TestInput", { input: input });
		return result;
	};

	// Asynchronous with sending and returning a JSON object
	_self.???PROJECT_FUNCTION_START???TestAsync = function (input, callback) {
		var success = function (data, response) {
				var json = JSON.parse(data);
				callback(json);
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "???PROJECT_FUNCTION_START???TestAsync", { input: input });
	};

	// Define a property on the extension object
	// Omit the getter or setter as needed to restrict the property
	Object.defineProperty(_self, "???PROJECT_FUNCTION_START???Property", {
		get: function () {
			var result,
				success = function (data, response) {
					result = data;
				},
				fail = function (data, response) {
					console.log("Error: " + data);
				};
			exec(success, fail, _ID, "???PROJECT_FUNCTION_START???Property", null);
			return result;
		},
		set: function (arg) {
			var result,
				success = function (data, response) {
					result = data;
				},
				fail = function (data, response) {
					console.log("Error: " + data);
				};
			exec(success, fail, _ID, "???PROJECT_FUNCTION_START???Property", {"value": arg });
			return result;
		}
	});

	_self.???PROJECT_FUNCTION_START???StartThread = function (callback) {
		var success = function (data, response) {
				callback(data);
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "???PROJECT_FUNCTION_START???StartThread", null);
	};

	_self.???PROJECT_FUNCTION_START???StopThread = function (callback) {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "???PROJECT_FUNCTION_START???StopThread", null);
		return result;
	};

module.exports = _self;
