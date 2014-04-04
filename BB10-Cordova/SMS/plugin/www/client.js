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
	_ID = "com.blackberry.community.SMSPlugin",
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
	

	_self.udSMS_Sync = function (input) {
		var result,
			success = function (data, response) {
				result = JSON.parse(data);
			},
			fail = function (data, response) {
				console.log("Client Error: " + data);
			};
		exec(success, fail, _ID, "udSMS_Sync", { input: input });
		return result;
	};
	

	// Asynchronous with sending and returning a JSON object
	_self.udSMS_Async = function (input, callback) {
		var success = function (data, response) {
				var json = JSON.parse(data);
				callback(json);
			},
			fail = function (data, response) {
				console.log("Client Error: " + data);
			};
		exec(success, fail, _ID, "udSMS_Async", { input: input });
	};



module.exports = _self;