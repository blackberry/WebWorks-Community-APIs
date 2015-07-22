/*
* Copyright (c) 2014 BlackBerry Limited
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
	_ID = "cordova-plugin-deviceemails",
	exec = cordova.require("cordova/exec");

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	// Simple Synchronous test function to get a string
	_self.ping = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Client Error: " + data);
			};
		exec(success, fail, _ID, "ping", null);
		return result;
	};

	_self.version = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Client Error: " + data);
			};
		exec(success, fail, _ID, "version", null);
		return result;
	};


	// Simple Synchronous test function to get a string
	_self.getDefaultEmailAddress = function () {
		var result,
			success = function (data, response) {
				result = JSON.parse(data);
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "getDefaultEmailAddress", null);
		return result;
	};
	
	// Simple Synchronous test function to get a string
	_self.getEmailAddresses = function () {
		var result,
			success = function (data, response) {
				result = JSON.parse(data);
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "getEmailAddresses", null);
		return result;
	};
	

module.exports = _self;