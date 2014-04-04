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
	_ID = "com.blackberry.community.PasswordCrypto",
	exec = cordova.require("cordova/exec");


	// this method is simply used to verify connectivity with the
	// PasswordCrypto Cordova Plugin
	_self.ping = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "ping", null);
		return result;
	};
	
	// This function is used to connect to index.js function with same name
	_self.pbkdf2_Sync = function (input) {
		var result,
			success = function (data, response) {
				result = JSON.parse(data);
			},
			fail = function (data, response) {
				console.log("Client.js Error: " + data);
			};
		exec(success, fail, _ID, "pbkdf2_Sync", { input: input });
		return result;
	};
	
	_self.pbkdf2_Async = function (input, callback) {
		var success = function (data, response) {
				var json = JSON.parse(data);
				callback(json);
			},
			fail = function (data, response) {
				console.log("Client Error: " + data);
			};
		exec(success, fail, _ID, "pbkdf2_Async", { input: input });
	};
	
module.exports = _self;