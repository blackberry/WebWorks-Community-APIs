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
	m_uuid = "555555",
    m_gaAccount = "UA-50848230-1", // default...
    m_appName = "Default_AppName",
   _utils = require("../../lib/utils");

module.exports = {

	// Object properties
	uuid: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			m_uuid = value;
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

// Send request to Google Anayltics based on tracking type
// Return request string sent to GA if no error
// Return empty string if error occurs
SendGARequest = function(trackType, args)
{
	var xmlhttp,
		status = "",
		message = "",
		optionString = "",
		jsonArgs,
		isOK = true;

	if (!XMLHttpRequest)
	{
		return '';
	}

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
	
	jsonArgs = JSON.parse(args);

	// TODO: should check if type of arguments are correct?

	switch (trackType)
	{
		case "pageview":
			optionString += "&t=pageview";
			if (jsonArgs.pageURL)
				optionString += ("&dp=" + jsonArgs.pageURL);
			if (jsonArgs.pageTitle)
				optionString += ("&dt=" + jsonArgs.pageTitle);
			if (jsonArgs.hostname)
				optionString += ("&dh=" + jsonArgs.hostname);

			break;

		case "event";
			optionString += "&t=event"
			// Category and Action are required for event tracking
			if (!jsonArgs.eventCategory || !jsonArgs.eventAction)
			{
				isOK = false;
				break;
			}
			else 
			{
				optionString += ("&ec=" + jsonArgs.eventCategory);
				optionString += ("&ea=" + jsonArgs.eventAction);
			}
			if (jsonArgs.eventLabel)
				optionString += ("&el=" + jsonArgs.eventLabel);
			if (jsonArgs.eventValue)
				optionString += ("&ev=" + jsonArgs.eventValue);

			break;

		case "transaction":
			optionString += "&t=transaction";
			// only ID is required
			if (!jsonArgs.tID)
			{
				isOK = false;
				break;
			}
			else
			{
				optionString += ("&ti=" + jsonArgs.tID);
			}
			if (jsonArgs.tAffil)
				optionString += ("&ta=" + jsonArgs.tAffil);
			if (jsonArgs.tRevenue)
				optionString += ("&tr=" + jsonArgs.tRevenue);
			if (jsonArgs.tShipn)
				optionString += ("&ts=" + jsonArgs.tShipn);
			if (jsonArgs.tTax)
				optionString += ("&tt=" + jsonArgs.tTax);
			if (jsonArgs.tCurr)
				optionString += ("&cu=" + tCurr);

			break;

		case "item";
			optionString += "&t=item";
			// tID and iName is required
			if (!jsonArgs.tID || !jsonArgs.iName)
			{
				isOK = false;
				break;
			}
			else
			{
				optionString += ("&ti=" + jsonArgs.tID);
				optionString += ("&in=" + jsonArgs.iName);
			}
			if (jsonArgs.iPrice)
				optionString += ("&ip=" + jsonArgs.iPrice);
			if (jsonArgs.iQuant)
				optionString += ("&iq=" + jsonArgs.iQuant);

			break;

		default:
			isOK = false;
			break;
	}
	
	if (isOK)
	{
		xmlhttp.open("POST","http://www.google-analytics.com/collect",true);
		xmlhttp.send(optionString);
	}

	// TODO: Add check for non-OK xtml status? But GA always return status OK unless there is connection timeout.
	if (isOK)
	{
		return optionString;
	}
	else
	{
		return '';
	}
};
///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

// don't need JNEXT and native