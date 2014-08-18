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

    // Init function, first 2 args are required
    // Without UUID arg, server side will attempt to retrieve from storage
    // If none found in storage, will generate a random UUID
    // Newly generated random UUID will be stored
    // Return: error if any
    _self.initializeGA = function (gaAccount, appName, uuid, bUseQueue) {
        var result = '';

        if (!gaAccount) {
            result = 'Missing GA account number';
        }
        else if (!appName) {
            result = 'Missing app name';
        }

        if (!result) {
             var success = function (data, response) {
                    //result = data;
                },
                fail = function (data, response) {
                    console.log("Init: " + data);
                    result = "Init error: " + data;
                };
            exec(success, fail, _ID, "initializeGA", {
                "arg_gaAccount": gaAccount,
                "arg_appName": appName, 
                "arg_uuid": (uuid || ""),
                "arg_bUseQueue": (bUseQueue || "")
            });

        }

        return result;
    };

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

    // Check if queue is enabled
    Object.defineProperty(_self, "useQueue", {
        get: function () {
            var result,
                success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                };
            exec(success, fail, _ID, "useQueue", null);
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
            exec(success, fail, _ID, "useQueue", {"value": arg });
            return result;
        }
    });

    // Random UUID feature
    Object.defineProperty(_self, "randomUuid", {
        get: function () {
            var result,
                success = function (data, response) {
                    result = data;
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                };
            exec(success, fail, _ID, "randomUuid", null);
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
            exec(success, fail, _ID, "randomUuid", {"value": arg });
            return result;
        }
    });

    _self.getDelay = function () {
        var result = '';

        var success = function (data, response) {
                result = data;
            },
            fail = function (data, response) {
                console.log("Error: " + data);
                result = "Error: " + data;
            };

        exec(success, fail, _ID, "getDelay", "");
        return result;
    };

    // Different types of tracking for GA
    // All tracking functions return empty string if tracking request is successful,
    // or return error message if any error occurs.

    // Pageview, &t=pageview, pageURL required
    _self.trackPageview = function (pageURL, pageTitle, hostName) {
        var result = 'Missing pageURL parameter';

        if (pageURL)
        {
            pageTitle = pageTitle || "";
            hostName = hostName || "";
            var success = function (data, response) {
                    //result = data;
                    result = "";
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                    result = "Error: " + data;
                };
            exec(success, fail, _ID, "trackAll", {
                "trackType": "pageview",
                "pageURL": pageURL, 
                "pageTitle": pageTitle,
                "hostName": hostName 
            });
        }
        return result;
    };

    // Event tracking, &t=event, Category and action are required
    _self.trackEvent = function (eventCategory, eventAction, eventLabel, eventValue) {
        var result = 'Missing category or action parameter';

        if (eventCategory && eventAction)
        {
            eventLabel = eventLabel || "";
            eventValue = eventValue || "";
            var success = function (data, response) {
                    //result = data;
                    result = "";
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                    result = "Error: " + data;
                };
            exec(success, fail, _ID, "trackAll", { 
                "trackType": "event",
                "eventCategory": eventCategory,
                "eventAction": eventAction,
                "eventLabel": eventLabel,
                "eventValue": eventValue
            });
        }
        return result;
    };

    // Transaction tracking
    // ID is required, others optional
    _self.trackTransaction = function (tID, tAffil, tRevenue, tShipn, tTax, tCurr) {
        var result = 'Missing transaction ID';

        if (tID)
        {
            tAffil = tAffil || "";
            tRevenue = tRevenue || "";
            tShipn = tShipn || "";
            tTax = tTax || "";
            tCurr = tCurr || "";
            
            var success = function (data, response) {
                    //result = data;
                    result = "";
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                    result = "Error: " + data;
                };
            exec(success, fail, _ID, "trackAll", { 
                    "trackType": "transaction",
                    "tID": tID,
                    "tAffil": tAffil,
                    "tRevenue": tRevenue,
                    "tShipn": tShipn,
                    "tTax": tTax,
                    "tCurr": tCurr
                });
        }
        return result;
    };

    // Item hit tracking,
    // tID & iName is required, others optional
    _self.trackItem = function (tID, iName, iPrice, iQuant) {
        var result = 'Missing transaction ID or item name';

        if (tID && iName)
        {
            iPrice = iPrice || "";
            iQuant = iQuant || "";
            
            var success = function (data, response) {
                    //result = data;
                    result = "";
                },
                fail = function (data, response) {
                    console.log("Error: " + data);
                    result = "Error: " + data;
                };
            exec(success, fail, _ID, "trackAll", { 
                    "trackType": "item",
                    "tID": tID,
                    "iName": iName,
                    "iPrice": iPrice,
                    "iQuant": iQuant
                });
        }
        return result;
    };

module.exports = _self;