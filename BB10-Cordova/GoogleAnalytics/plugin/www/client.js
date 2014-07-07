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
	_ID = "com.blackberry.community.googleanalyticsplugin",
	exec = cordova.require("cordova/exec");

	// Unique user ID for tracking
    Object.defineProperty(_self, "uuid", {
        get: function () {
            var result,
                success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                };
            exec(success, fail, _ID, "uuid", null);
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
            exec(success, fail, _ID, "uuid", {"value": arg });
            return result;
        }
    });

    // Google Analytics account
    Object.defineProperty(_self, "gaAccount", {
        get: function () {
            var result,
                success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                };
            exec(success, fail, _ID, "gaAccount", null);
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
            exec(success, fail, _ID, "gaAccount", {"value": arg });
            return result;
        }
    });

    // App name
    Object.defineProperty(_self, "appName", {
        get: function () {
            var result,
                success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                };
            exec(success, fail, _ID, "appName", null);
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
            exec(success, fail, _ID, "appName", {"value": arg });
            return result;
        }
    });
    
    // last payload string sent
    Object.defineProperty(_self, "lastPayload", {
        get: function () {
            var result,
                success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                };
            exec(success, fail, _ID, "lastPayload", null);
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
            exec(success, fail, _ID, "lastPayload", {"value": arg });
            return result;
        }
    });

	Object.defineProperty(_self, "googleanalyticsProperty", {
		get: function () {
			var result,
				success = function (data, response) {
					result = data;
				},
				fail = function (data, response) {
					console.log("Error: " + data);
				};
			exec(success, fail, _ID, "googleanalyticsProperty", null);
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
			exec(success, fail, _ID, "googleanalyticsProperty", {"value": arg });
			return result;
		}
	});

    // Different types of tracking for GA
    // All tracking functions return true if tracking request is successful.

    // Pageview, &t=pageview, pageURL required
    _self.trackPageview = function (pageURL, pageTitle, hostName) {
        var result = 'No pageURL';

        if (pageURL)
        {
            pageTitle = pageTitle || "";
            hostName = hostName || "";
            var success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                    result = "Error: " + data;
                };
            exec(success, fail, _ID, "trackPageview", {
                "pageURL": pageURL, 
                "pageTitle": pageTitle,
                "hostName": hostName 
            });
        }
        return result;
    };

module.exports = _self;