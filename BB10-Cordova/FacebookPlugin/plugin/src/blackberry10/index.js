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

var template,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {

	init: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(template.getInstance().test(), false);
	},
	
	testInput: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(template.getInstance().testInput(result.callbackId, args), false);
	},
	
	// Asynchronous function calls into the plugin and returns
	testAsync: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		args = JSON.parse(decodeURIComponent(args["input"]));
		template.getInstance().testAsync(result.callbackId, args);
		result.noResult(true);
	},
	
	templateProperty: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			template.getInstance().templateProperty(result.callbackId, value);
			result.noResult(false);
		} else {
			result.ok(template.getInstance().templateProperty(), false);
		}
	},
	
	// Thread methods to start and stop
	startThread: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!threadCallback) {
			threadCallback = result.callbackId;
			resultObjs[result.callbackId] = result;
			result.ok(template.getInstance().startThread(result.callbackId), true);
		} else {
			result.error(template.getInstance().startThread(result.callbackId), false);
		}
	},
	stopThread: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		if (!threadCallback) {
			result.error("Thread is not running", false);
		} else {
			delete resultObjs[threadCallback];
			threadCallback = null;
			result.ok(template.getInstance().stopThread(), false);
		}
	}
};