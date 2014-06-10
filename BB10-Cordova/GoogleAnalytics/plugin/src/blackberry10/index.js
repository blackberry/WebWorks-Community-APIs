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
   _utils = require("../../lib/utils"),
   gaUUID = "",
   gaGAaccount = "",
   gaAppName = "";

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// Object properties
	UUID: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			gaUUID = value;
			result.noResult(false);
		} else {
			result.ok(gaUUID, false);
		}
	},

	GAaccount: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			gaGAaccount = value;
			result.noResult(false);
		} else {
			result.ok(gaGAaccount, false);
		}
	},

	AppName: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			gaAppName = value;
			result.noResult(false);
		} else {
			result.ok(gaAppName, false);
		}
	},

	// Tracking functions
	trackPageview: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (SendGARequest("pageview", args))
			result.ok("Pageview tracking ok", false);
		else
			result.error("Pageview tracking error", false);
	},

	trackEvent: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (SendGARequest("event", args))
			result.ok("EVent tracking ok", false);
		else
			result.error("Event tracking error", false);
	},

	trackTransaction: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (SendGARequest("transaction", args))
			result.ok("Transaction tracking ok", false);
		else
			result.error("Transaction tracking error", false);
	}

	trackItem: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (SendGARequest("item", args))
			result.ok("Item hit tracking ok", false);
		else
			result.error("Item hit tracking error", false);
	},
};

SendGARequest = function(trackType, args)
{
	var xmlhttp,
		status = "",
		message = "",
		optionString = "",
		jsonArgs;
		isOK = true;

	// TODO: need to check if xmlhttprequest is available?
	xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function ()
		{
			if (xmlhttp.readyState == 4)
			{
				status = xmlhttp.status;
				message = xmlhttp.statusText;
			}
		};

	optionString = "v=1&tid=" + gaGAaccount + "&cid=" + gaUUID + "&an=" + gaAppName;
	
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
	return isOK;
};
///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

// don't need JNEXT and native