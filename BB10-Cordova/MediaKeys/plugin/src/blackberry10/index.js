/*
* Copyright (c) 2013 BlackBerry Limited.
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

var resultObjs = {},
	threadCallback = null,
	_utils = require("../../lib/utils");

module.exports = {
	bind: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		threadCallback = result.callbackId;
		var error;

		var validMediaKeys = ["fastForward", "pause", "play", "playPause", "rewind", "stop", "volumeDown", "volumeUp", "previous", "next"];
		var validKeyLength = ["short", "long"];

		args.mediakeys = decodeURIComponent(args.mediakeys);
		args.mediakeys = JSON.parse(args.mediakeys);

		// check that all keys are bindable
		args.mediakeys.some(function (obj) {
			if (typeof obj.mediakey !== 'string' || typeof obj.keylength !== 'string') {
				error = "Mediakey object has invalid fields";
			}

			if (!error &&
				validMediaKeys.indexOf(obj.mediakey) === -1 && 
				validKeyLength.indexOf(obj.keylength) === -1) {

				error = "Unable to bind mediakey '"+ obj.mediakey+"' of keylength '"+obj.keylength+"'";
			}

			return error;
		});

		if (!error) {
			args.mediakeys.some(function (obj) {
				error = mediaKeys.getInstance().bind(result.callbackId, obj);

				return error;
			});
		}

		if (error) {
			result.error(error, false);
			delete resultObjs[threadCallback];
		}
	}
};


///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.mediaKeys = function () {
	
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libMediaKeys")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libMediaKeys.MediaKeysJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);

		var views = qnx.webplatform.getWebViews();
		var windowGroup = null;
		var z = -1;
		for (var i = 0; i < views.length; i++) {
				if (views[i].visible && views[i].zOrder > z){
						z = views[i].zOrder;
						windowGroup = views[i].windowGroup;
				}
		}
		var initResult = JNEXT.invoke(self.m_id, "join " + threadCallback + " " + JSON.stringify(windowGroup));
		if (initResult) {
			return initResult;
		}
	};

	// ************************
	// Enter your methods here
	// ************************

	self.bind = function (callbackId, args) {
		var error = JNEXT.invoke(self.m_id, "bind " + callbackId + " " + JSON.stringify(args));

		return error;
	};

	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");
			data = JSON.parse(data);

		var isThread = (callbackId == threadCallback);

		if (data.result == "mediaKeyPressed") {
			result.callbackOk(data, isThread);
		} else {
			result.callbackError(data, isThread);
			delete resultObjs[callbackId];
		}
	};

	// ************************
	// End of methods to edit
	// ************************

	self.m_id = "";

	self.getInstance = function () {
		if (!hasInstance) {
			hasInstance = true;
			self.init();
		}
		return self;
	};

};

mediaKeys = new JNEXT.mediaKeys();
