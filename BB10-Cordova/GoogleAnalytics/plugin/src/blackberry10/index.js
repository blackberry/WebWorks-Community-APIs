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

var _utils = require("../../lib/utils");

module.exports = {

    // Object properties
    uuid: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value;
        if (args && args["value"])
        {
            value = JSON.parse(decodeURIComponent(args["value"]));
            result.ok(ga.gauuid(value), false);
        }
        else
        {
            result.ok(ga.gauuid(), false);
        }
    },

    gaAccount: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value;
        if (args && args["value"]) 
        {
            value = JSON.parse(decodeURIComponent(args["value"]));
            var error;
            error = ga.account(value);
            if (error) {
                result.error(error, false);
            }
            else {
                result.noResult(false);
            }
        } 
        else 
        {
            result.ok(ga.account(), false);
        }
    },

    appName: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value;
        if (args && args["value"]) 
        {
            value = JSON.parse(decodeURIComponent(args["value"]));
            var error;
            error = ga.appname(value);
            if (error) {
                reslut.error(error, false);
            }
            else {
                result.noResult(false);
            }
        } 
        else 
        {
            result.ok(ga.appname(), false);
        }
    },

    lastPayload: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        if (args && args["value"]) 
        {
            result.error("Cannot set lastPayload property", false);
        } 
        else 
        {
            result.ok(ga.lastpayload(), false);
        }
    },

    randomUuid: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value = "";
        if (args && args["value"])
        {
            var error;
            value = JSON.parse(decodeURIComponent(args["value"]));
            error = ga.randomuuid(value);
            if (error) {
                result.error(error, false);
            }
            else {
                result.noResult(false);
            }
        }
        else
            result.ok(ga.randomuuid(), false);
    },

    // All-in-one function setting up account, uuid and appName
    initializeGA: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value, 
            error;
        // args in the order: gaAccount, [UUID], [appName]
        if (!args || !args["arg_gaAccount"]) {
            error = "GA account number is required";
        }
        if (!error) {
            value = JSON.parse(decodeURIComponent(args["arg_gaAccount"]));
            if ("" == value) {
                error = "GA account number is required";
            }
        }
        if (!error) {
            error = ga.account(value);
        }

        if (!error && args["arg_uuid"]) {
            value = JSON.parse(decodeURIComponent(args["arg_uuid"]));
            error = ga.gauuid(value);
        }

        if (!error && args["arg_appName"]) {
            value = JSON.parse(decodeURIComponent(args["arg_appName"]));
            error = ga.appname(value);
        }

        if (error) {
            result.error(error, false);
        }
        else {
            result.ok("GA initialized", false);
        }
    },

    trackAll: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var sTrackType,
            error;

        if (!args || !args["trackType"]){
            error = "Need track type argument";
        }

        if (!error) {
            sTrackType = JSON.parse(decodeURIComponent(args["trackType"]));
            error = ga.processtracking(sTrackType, args);
        }

        if (error) {
            result.error(sTrackType + " tracking error: " + error, false);
        }
        else {
            result.ok("Finished " + sTrackType + " tracking", false);
        }
    }
};


// GA module
var ga = (function() {
    var m_uuid = "",
        m_account = "", 
        m_appName = "Default_AppName",
        m_lastPayload = "",
        bAccountSet = false,
        bRandomUuid = false;

    //***********************************
    // Functions for setting properties
    //***********************************
    var account = function(value) {
        if (undefined != value) {
            // UA-xxxxxxxx-x
            // 0123456789012 <-index
            if ((value.length != 13) || (value.substr(0,3) != "UA-") || (value.charAt(11) != '-')) {
                return "Invalid GA account, should be in the format UA-xxxxxxxx-x";
            }
            m_account = value;
            bAccountSet = true;
            return "";
        }
        else {
            return m_account;
        }
    };

    var appname = function(value) {
        if (undefined != value) {
            if (value.length == 0) {
                return "AppName cannot be empty";
            }
            m_appName = value;
            return "";
        }
        else {
            return m_appName;
        }
    };

    var gauuid = function(value) {
        if (undefined != value) { 
            m_uuid = value;
            if ("" == m_uuid) {
                m_uuid = randomizeUuid();
            }
            return "";
        }
        return m_uuid;
    };

    var lastpayload = function() {
        return m_lastPayload;
    };

    var randomuuid = function(value) {
        var error = "";
        if (undefined != value) {
            value = value.toLowerCase();
            switch (value) {
                case ("true"):
                    bRandomUuid = true;
                    break;
                case ("false"):
                    bRandomUuid = false;
                    break;
                default:
                    error = "True/False only for setting random UUID feature";
                    break;
            }

            return error;
        }
        else {
            return bRandomUuid;
        }
    };

    //***********************************
    // Utilities
    //***********************************

    // oArgs = json arg object
    // sParam = name of GA parameter
    // sKey = key to the value of sParam in oArgs
    var getParameter = function (oArgs, sParam, sKey) {
        var output;
        output = JSON.parse(decodeURIComponent(oArgs[sKey]));

        // All optional parameters, if not supplied by user, is set to "" in client.js
        if ("" != output)
        {
            output = "&" + sParam + "=" + output;
        }

        return output;
    };

    var randomizeUuid = function() {
        //Version4(random) UUID:
        //xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx where x is any hexadecimal digit and y is one of 8, 9, A, or B
        //012345678901234567890123456789012345 - index
        function _p8(split) {
            var p = (Math.random().toString(16)+"000000000").substr(2,8);
            return split? "-" + p.substr(0,4) + "-" + p.substr(4,4) : p ;
        }
        var ret = _p8() + _p8(true) + _p8(true) + _p8();
        ret = ret.substr(0,14) + "4" + ret.substr(15);
        var ch = ret.charAt(19);
        if ('8' != ch || '9' != ch || 'A' != ch || 'B' != ch)
        {
            ret = ret.substr(0,19) + "A" + ret.substr(20);
        }
        return ret;
    };

    //***********************************
    // Core functions
    //***********************************

    // Main interface to process tracking requests
    // Return error if any
    var processtracking = function (trackType, args) {
        var xmlhttp,
            status = "",
            message = "",
            optionString = "",
            jsonArgs = "",
            error = "",
            isOK = true;

        if (bRandomUuid) {
            m_uuid = randomizeUuid();
        }

        if (!bAccountSet) {
            error = "Need GA account number first";
        }

        // check if xmlhttprequest is available
        if (!XMLHttpRequest) {
            error = "XMLHttpRequest not found";
        }

        if (!error) {
            xmlhttp = new XMLHttpRequest();
            // TODO need to check for timeout and network connection
            xmlhttp.onreadystatechange = function () {
                    if (xmlhttp.readyState == 4)
                    {
                        status = xmlhttp.status;
                        message = xmlhttp.statusText;
                    }
                };

            optionString = "v=1&tid=" + m_account + "&cid=" + m_uuid + "&an=" + m_appName;
            
            switch (trackType)
            {
                case "pageview":
                    optionString += "&t=pageview";
                    optionString += getParameter(args, "dp", "pageURL");
                    optionString += getParameter(args, "dt", "pageTitle");
                    optionString += getParameter(args, "dh", "hostName");
                    break;

                case "event":
                    optionString += "&t=event";
                    optionString += getParameter(args, "&ec", "eventCategory");
                    optionString += getParameter(args, "&ea", "eventAction");
                    optionString += getParameter(args, "&el", "eventLabel");
                    optionString += getParameter(args, "&ev", "eventValue");
                    break;

                case "transaction":
                    optionString += "&t=transaction";
                    optionString += getParameter(args, "&ti", "tID");
                    optionString += getParameter(args, "&ta", "tAffil");
                    optionString += getParameter(args, "&tr", "tRevenue");
                    optionString += getParameter(args, "&ts", "tShipn");
                    optionString += getParameter(args, "&tt", "tTax");
                    optionString += getParameter(args, "&cu", "tCurr");
                    break;

                case "item":
                    optionString += "&t=item";
                    optionString += getParameter(args, "&ti", "tID");
                    optionString += getParameter(args, "&in", "iName");
                    optionString += getParameter(args, "&ip", "iPrice");
                    optionString += getParameter(args, "&iq", "iQuant");
                    break;

                default:
                    error = "Invalid track type";
                    break;
            }
        
            xmlhttp.open("POST","http://www.google-analytics.com/collect",true);
            xmlhttp.send(optionString);
            m_lastPayload = optionString;
        }

        return error;
    };

    // export
    return {
        account: account,
        appname: appname,
        gauuid: gauuid,
        lastpayload: lastpayload,
        randomuuid: randomuuid,
        processtracking: processtracking
    };

})();
