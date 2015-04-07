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
    _ID = "com.blackberry.community.nowplaying",
    exec = cordova.require("cordova/exec");

	_self.NowPlayingConnectionTest = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingConnectionTest", null);
		return result;
	};


	_self.NowPlayingPlay = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingPlay", null);
		return result;
	};

	_self.NowPlayingStop = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingStop", null);
		return result;
	};

	_self.NowPlayingPause = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingPause", null);
		return result;
	};

	_self.NowPlayingGetSourceAddress = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingGetSourceAddress", null);
		return result;
	};

	_self.NowPlayingGetDuration = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingGetDuration", null);
		return result;
	};

	_self.NowPlayingGetPosition = function () {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingGetPosition", null);
		return result;
	};

	_self.NowPlayingSwitchMusic = function (input) {
		var result,
			success = function (data, response) {
				result = data;
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "NowPlayingSwitchMusic", { input: input });
		return result;
	};



	// Below are just for testing purpose
	_self.testAsync = function (input, callback) {
			var success = function (data, response) {
				var json = JSON.parse(data);
				callback(json);
			},
			fail = function (data, response) {
				console.log("Error: " + data);
			};
		exec(success, fail, _ID, "testAsync", { input: input });
	};




module.exports = _self;
