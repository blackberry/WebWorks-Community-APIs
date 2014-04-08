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

var extractZipFile,
	resultObjs = {},
	threadCallback = null,
   _utils = require("../../lib/utils");

module.exports = {
	extract: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		extractZipFile.getInstance().extractFile(result.callbackId, args);
		result.noResult(true);
	},
	
	compress: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		extractZipFile.getInstance().compressFile(result.callbackId, args);
		result.noResult(true);
	}
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.ExtractZipFile = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libExtractZipFile")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libExtractZipFile.ExtractZipFileJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in template_js.cpp
	self.extractFile = function (callbackId, input) {
		var args = decodeURIComponent(input[0]);
		return JNEXT.invoke(self.m_id, "extractFile " + callbackId + ' ' + args);
	};
	
	self.compressFile = function (callbackId, input) {
		var args = decodeURIComponent(input[0]);
		return JNEXT.invoke(self.m_id, "compressFile " + callbackId + ' ' + args);
	};
	
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");

		if (result) {
			if (callbackId != threadCallback) {
				// We do not use the default
				// sucess vs fail callbacks
				// instead the RAW api merges it
				result.callbackOk(data, false);
				delete resultObjs[callbackId];
			} else {
				result.callbackOk(data, true);
			}
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

extractZipFile = new JNEXT.ExtractZipFile();
