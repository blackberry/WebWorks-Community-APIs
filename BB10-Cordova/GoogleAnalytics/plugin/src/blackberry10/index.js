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

var googleanalytics,
    resultObjs = {},
    threadCallback = null,
    m_uuid = "555556",
    m_gaAccount = "UA-50848230-1", // default...
    m_appName = "Default_AppName",
   _utils = require("../../lib/utils");

module.exports = {

    // Code can be declared and used outside the module.exports object,
    // but any functions to be called by client.js need to be declared
    // here in this object.

    // These methods call into JNEXT.Googleanalytics which handles the
    // communication through the JNEXT plugin to googleanalytics_js.cpp

    // Object properties
    uuid: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value;
        if (args && args["value"]) {
            value = JSON.parse(decodeURIComponent(args["value"]));
            m_uuid = value;
            // if passed in value is empty string, generate random UUID
            if ("" == m_uuid)
            {
                m_uuid = Math.round(Math.random()*2147483647);
            }
            result.noResult(false);
        } else {
            result.ok(m_uuid, false);
        }
    },
    gaAccount: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value;
        if (args && args["value"]) {
            value = JSON.parse(decodeURIComponent(args["value"]));
            m_gaAccount = value;
            result.noResult(false);
        } else {
            result.ok(m_gaAccount, false);
        }
    },
    appName: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value;
        if (args && args["value"]) {
            value = JSON.parse(decodeURIComponent(args["value"]));
            m_appName = value;
            result.noResult(false);
        } else {
            result.ok(m_appName, false);
        }
    },

    // Tracking functions
    trackPageview: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var sRequest = sendGARequest("pageview", args);
        if (sRequest.length > 0)
            result.ok(sRequest, false);
        else
            result.error("Pageview tracking error", false);
    },

    trackEvent: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var sRequest = sendGARequest("event", args);
        if (sRequest.length > 0)
            result.ok(sRequest, false);
        else
            result.error("Event tracking error", false);
    },

    trackTransaction: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var sRequest = sendGARequest("transaction", args);
        if (sRequest.length > 0)
            result.ok(sRequest, false);
        else
            result.error("Transaction tracking error", false);
    },

    trackItem: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var sRequest = sendGARequest("item", args);
        if (sRequest.length > 0)
            result.ok(sRequest, false);
        else
            result.error("Item hit tracking error", false);
    },

    googleanalyticsProperty: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        var value;
        if (args && args["value"]) {
            value = JSON.parse(decodeURIComponent(args["value"]));
            googleanalytics.getInstance().googleanalyticsProperty(result.callbackId, value);
            result.noResult(false);
        } else {
            result.ok(googleanalytics.getInstance().googleanalyticsProperty(), false);
        }
    }
};

getParameter = function (oArgs, sParam)
{
    var output;

    if ((output = JSON.parse(decodeURIComponent(oArgs[sParam]))))
    //if ((output = oArgs[sParam]))
    {
        return output;
    }
    else
    {
        return "";
    }
};

// Send request to Google Anayltics based on tracking type
// Return request string sent to GA if no error
// Return empty string if error occurs
sendGARequest = function (trackType, args)
{
    var xmlhttp,
        status = "",
        message = "",
        optionString = "",
        jsonArgs = "default json args",
        isOK = true;

    // check if xmlhttprequest is available?
    if (XMLHttpRequest)
    {
        xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function ()
            {
                if (xmlhttp.readyState == 4)
                {
                    status = xmlhttp.status;
                    message = xmlhttp.statusText;
                }
            };

        optionString = "v=1&tid=" + m_gaAccount + "&cid=" + m_uuid + "&an=" + m_appName;
        
        switch (trackType)
        {
            case "pageview":
                optionString += "&t=pageview";

                optionString += ("&dp=" + getParameter(args, "pageURL"));
                //optionString += ("&dt=" + getParameter(args, "pageTitle"));
                //optionString += ("&dh=" + getParameter(args, "hostname"));

                break;

            default:
                isOK = false;
                break;
        }
    
        if (isOK)
        {
            xmlhttp.open("POST","http://www.google-analytics.com/collect",true);
            xmlhttp.send(optionString);
            //xmlhttp.send("v=1&tid=UA-50848230-1&cid=1244382521&an=myapp2&av=1.0&t=transaction&ti=1111&tr=19.99&cu=USD");
        }

        // TODO: Add check for non-OK xtml status? But GA always return status OK unless there is connection timeout.

    }
    else
    {
        isOK = false;
    }

    return ((isOK)? optionString: "");
};
///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

