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
	_ID = "com.blackberry.community.distimo",
	exec = cordova.require("cordova/exec");

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	_self.debug = function (callback) {
		var success = fail = function (data, response) {
			callback(data);
		}
		exec(success, fail, _ID, "debug", null);
	};

	_self.start = function (sdkKey, callback) {
		var result,
			success = function (data, response) {
				result = data;
				callback("SDK started: " + data);
			},
			fail = function (data, response) {
				callback("SDK failed to start: " + data);
			};
		var input = { "sdkKey": sdkKey };
		exec(success, fail, _ID, "start", {input : input});
	};

	_self.version = function (callback) {
		var result,
			success = function (data, response) {
				result = data;
				callback("Success: version " + data);
			},
			fail = function (data, response) {
				callback("Error retrieving version: " + data);
			};
		exec(success, fail, _ID, "version", null);
	};

	_self.logUserRegistered = function (callback) {
		var result,
			success = function (data, response) {
				result = data;
				callback("Success: logUserRegistered");
			},
			fail = function (data, response) {
				callback("Error on logUserRegistered: " + data);
			};
		exec(success, fail, _ID, "logUserRegistered", null);
	};

	_self.logExternalPurchaseWithCurrency = function (productID, currencyCode, price, quantity, callback) {
		var result,
			success = function (data, response) {
				result = data;
				callback("Success: logExternalPurchaseWithCurrency");
			},
			fail = function (data, response) {
				callback("Error on logExternalPurchaseWithCurrency: " + data);
			};
		var input = { "productID": productID, "currencyCode": currencyCode, "price": price, "quantity": quantity };
		exec(success, fail, _ID, "logExternalPurchaseWithCurrency", {input : input});
	};

	_self.logBannerClick = function (publisher, callback) {
		var result,
			success = function (data, response) {
				result = data;
				callback("Success: logBannerClick");
			},
			fail = function (data, response) {
				callback("Error on logBannerClick: " + data);
			};
		var input = { "publisher": publisher };
		exec(success, fail, _ID, "logBannerClick", {input : input});
	};

	_self.setUserID = function (userID, callback) {
		var result,
			success = function (data, response) {
				result = data;
				callback("Success: setUserID");
			},
			fail = function (data, response) {
				callback("Error on setUserID: " + data);
			};
		var input = { "userID": userID };
		exec(success, fail, _ID, "setUserID", {input : input});
	};

	_self.openAppLink = function (applinkHandle, campaignHandle, callback) {
		var result,
			success = function (data, response) {
				result = data;
				callback("Success: openAppLink");
			},
			fail = function (data, response) {
				callback("Error on openAppLink: " + data);
			};
		var input = { "applinkHandle": applinkHandle, "campaignHandle": campaignHandle };
		exec(success, fail, _ID, "openAppLink", {input : input});
	};

module.exports = _self;