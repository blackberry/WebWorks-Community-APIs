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
	_ID = "cordova-plugin-beacon",
	exec = cordova.require("cordova/exec");

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	// start of Bluetooth apis

	_self.addBeaconUuidToMonitor = function (uuid, successCallback, errorCallback) {
		var	success = function (data, response) {
				if (successCallback  && typeof successCallback === 'function') {
					if (data && data.length > 0) {
						successCallback(data);
					}
				}
			},
			fail = function (data, response) {
				console.log("Error: " + data);
				if (errorCallback  && typeof errorCallback === 'function') {
					errorCallback(data);
				}
			};
		exec(success, fail, _ID, "addBeaconUuidToMonitor", [uuid]);
	};

	_self.removeBeaconUuidToMonitor = function (uuid, successCallback, errorCallback) {
		var	success = function (data, response) {
				if (successCallback  && typeof successCallback === 'function') {
					if (data && data.length > 0) {
						successCallback(data);
					}
				}
			},
			fail = function (data, response) {
				console.log("Error: " + data);
				if (errorCallback  && typeof errorCallback === 'function') {
					errorCallback(data);
				}
			};
		exec(success, fail, _ID, "removeBeaconUuidToMonitor", [uuid]);
	};

	_self.initialiseBluetooth = function (successCallback, errorCallback) {
		var	success = function (data, response) {
				if (successCallback  && typeof successCallback === 'function') {
					if (data && data.length > 0) {
						successCallback(data);
					}
				}
			},
			fail = function (data, response) {
				console.log("Error: " + data);
				if (errorCallback  && typeof errorCallback === 'function') {
					errorCallback(data);
				}
			};
		exec(success, fail, _ID, "initialiseBluetooth", []);
	};

	_self.terminateBluetooth = function () {
		var	success = function (data, response) {
				if (successCallback  && typeof successCallback === 'function') {
					if (data && data.length > 0) {
						successCallback(data);
					}
				}
			},
			fail = function (data, response) {
				console.log("Error: " + data);
				if (errorCallback  && typeof errorCallback === 'function') {
					errorCallback(data);
				}
			};
		exec(success, fail, _ID, "terminateBluetooth", []);
	};

	_self.pluginVersion = function (successCallback, errorCallback) {
		var	success = function (data, response) {
				if (successCallback  && typeof successCallback === 'function') {
					if (data && data.length > 0) {
						successCallback(data);
					}
				}
			},
			fail = function (data, response) {
				console.log("Error: " + data);
				if (errorCallback  && typeof errorCallback === 'function') {
					errorCallback(data);
				}
			};
		exec(success, fail, _ID, "pluginVersion", []);
	};

	_self.startMonitoring = function (successCallback, errorCallback) {
		var	success = function (data, response) {
				if (successCallback  && typeof successCallback === 'function') {
					if (data && data.length > 0) {
						successCallback(data);
					}
				}
			},
			fail = function (data, response) {
				console.log("Error: " + data);
				if (errorCallback  && typeof errorCallback === 'function') {
					errorCallback(data);
				}
			};
		exec(success, fail, _ID, "startMonitoring", []);
	};

	_self.stopMonitoring = function (successCallback, errorCallback) {
		var	success = function (data, response) {
				if (successCallback  && typeof successCallback === 'function') {
					if (data && data.length > 0) {
						successCallback(data);
					}
				}
			},
			fail = function (data, response) {
				console.log("Error: " + data);
				if (errorCallback  && typeof errorCallback === 'function') {
					errorCallback(data);
				}
			};
		exec(success, fail, _ID, "stopMonitoring", []);
	};

module.exports = _self;
