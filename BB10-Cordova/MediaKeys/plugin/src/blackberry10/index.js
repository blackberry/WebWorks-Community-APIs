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
// this section is executed first
	checkVolume: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(mediaKeys.getInstance().checkVolume(result.callbackId), false);
	},
	bind: function (success, fail, args, env) {
		var validMediaKeys = ["volumeUp", "volumeDown"];
		var validKeyLength = ["short", "long"];

		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		threadCallback = result.callbackId;
		var error;

		args.mediakey = decodeURIComponent(args.mediakey);
		args.mediakey = args.mediakey.replace(/^"|"$/g, "").replace(/\\"/g, '"').replace(/\\\\/g, '\\');

		args.keylength = decodeURIComponent(args.keylength);
		args.keylength = args.keylength.replace(/^"|"$/g, "").replace(/\\"/g, '"').replace(/\\\\/g, '\\');

		if (validMediaKeys.indexOf(args.mediakey) == -1 ||
				validKeyLength.indexOf(args.keylength) == -1) {
			error = "attempted to bind to invalid mediakey";
		}

		if (!error) {
			var returnVal = mediaKeys.getInstance().bind(result.callbackId, args);

			// temp
			error = returnVal;
		}

		if (error) {
			alert('ERROR: '+ error);

			result.error(error, false);
			delete resultObjs[threadCallback];
		}
	},
	show: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		alert(JSON.stringify(result));
		resultObjs[result.callbackId] = result;
		threadCallback = result.callbackId;
		var error;

		// args = { message:, buttons:, [settings] };

		args.message = decodeURIComponent(args.message);
		if (! JSON.parse(args.message) ) {
			// do not show message "null"/"undefined" when given null/undefined message
			args.message = "";
		} else {
			args.message = args.message.replace(/^"|"$/g, "").replace(/\\"/g, '"').replace(/\\\\/g, '\\');
		}

		args.buttons = JSON.parse(decodeURIComponent(args.buttons));
		if (args.buttons) {
			if (!Array.isArray(args.buttons)) {
				error = "buttons is not an array";
			}
		} else {
			error = "buttons is undefined";
		}

		if (args.settings) {
			args.settings = JSON.parse(decodeURIComponent(args.settings));
		} else {
			args.settings = {};
		}


		if (!error) {
			error = mediaKeys.getInstance().show(result.callbackId, args);
		}

		if (error) {
			result.error(error, false);
			// fail to create dialog, onEvent will not be called
			delete resultObjs[threadCallback]
		}
	}
};


///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.mediaKeys = function () {
// this section is executed from the above

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

		var views = qnx.webplatform.getWebViews()
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
		var val = JNEXT.invoke(self.m_id, "bind " + callbackId + " " + JSON.stringify(args));
		//var val = 'bind';
		return val;
	}

	self.checkVolume = function (callbackId) {
		return JNEXT.invoke(self.m_id, "checkVolume");
	};

	self.show = function(callbackId, args){

		var error;
		try {
			var id = JNEXT.invoke(self.m_id, "create " + callbackId + " " + JSON.stringify(args));
			if (id.indexOf(' ') >= 0) {
				// it's an error instead of real id
				return id;
			}

			error = JNEXT.invoke(self.m_id, "show " + callbackId + " " + id);
		} catch (e) {
			return e;
		}

		return error;
	};

	self.onEvent = function (strData) {

		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");
			data = JSON.parse(data);

		var isThread = (callbackId == threadCallback);

		if (data.result == "ButtonSelection") {
			result.callbackOk(data.select, isThread);
		} else {
			result.callbackError(data.error, isThread);
		}
		// dialog finished, don't need the resultObj
		delete resultObjs[callbackId];
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
