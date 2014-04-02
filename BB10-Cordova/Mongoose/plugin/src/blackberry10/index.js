/*
* Copyright (c) 2014 BlackBerry Limited
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

var mongoose,
resultObjs = {},
threadCallback = null,
_utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into JNEXT.mongoose which handles the
	// communication through the JNEXT plugin to mongoose_js.cpp
	start: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		args = JSON.parse(decodeURIComponent(args["input"]));
		result.ok(mongoose.getInstance().start(args), false);
	},
	stop: function (success, fail, args, env) {
		var result = new PluginResult(args, env);
		result.ok(mongoose.getInstance().stop(), false);
	},
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.Mongoose = function () {
	var self = this,
	hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libMongoose")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libMongoose.MongooseJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// calls into InvokeMethod(string command) in mongoose_js.cpp
	self.start = function (input) {
		return JNEXT.invoke(self.m_id, "start " + JSON.stringify(input));
	};

	self.stop = function () {
		return JNEXT.invoke(self.m_id, "stop");
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

mongoose = new JNEXT.Mongoose();