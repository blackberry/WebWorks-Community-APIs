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

    show: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
		resultObjs[result.callbackId] = result;
		threadCallback = result.callbackId;

        // args = { message:, buttons:, settings };
        if (args.message) {
            args.message = decodeURIComponent(args.message);
            args.message = args.message.replace(/^"|"$/g, "").replace(/\\"/g, '"').replace(/\\\\/g, '\\');
        }

        if (args.buttons) {
            args.buttons = JSON.parse(decodeURIComponent(args.buttons));
        }

        if (args.settings) {
            args.settings = JSON.parse(decodeURIComponent(args.settings));
        }

        // try{ 		
        result.ok(sysDialog.getInstance().show(result.callbackId, args), false); 

		// if (error) {
			// result.error(error, false);
		// } else {
			// result.ok(true, false);
		// }
    }
};


// sysDialog = (function() {



//     return {

//         show: function() {


//     }



///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.sysDialog = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libSysDialog")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libSysDialog.SysDialogJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	self.show = function(callbackId, args){
		return JNEXT.invoke(self.m_id, "show " + callbackId + " " + JSON.stringify(args)); 
	};

	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			callbackId = arData[0],
			result = resultObjs[callbackId],
			data = arData.slice(1, arData.length).join(" ");
		
		if (result) {
			if (callbackId != threadCallback) {
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

sysDialog = new JNEXT.sysDialog();
