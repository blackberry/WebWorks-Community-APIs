/*
* Copyright 2013 Research In Motion Limited.
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

var gseCrypto = require("./gseCryptoJNEXT").gseCrypto,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into templateJNEXT.js which handles the
	// communication through the JNEXT plugin to template_js.cpp
	test: function (success, fail) {
		success(gseCrypto.getInstance().test());
	},
	testInput: function (success, fail, args) {
		alert("index.js s: " + args + "\n" + args.hex);
		args = JSON.parse(decodeURIComponent(args["input"]));
		alert("index.js t: " + args + "\n" + args.hex);
		success(gseCrypto.getInstance().testInput(args));
	},
	// Asynchronous function calls into the plugin and returns
	testAsync: function (success, fail) {
		gseCrypto.getInstance().testAsync();
		success();
	},
	testAsyncJSON: function (success, fail) {
		gseCrypto.getInstance().testAsyncJSON();
		success();
	},
	testAsyncJSONio: function (success, fail, args) {
		args = JSON.parse(decodeURIComponent(args["input"]));
		gseCrypto.getInstance().testAsyncJSONio(args);
		success();
	},
	templateProperty: function (success, fail, args) {
		var value;
		if (args && args["value"]) {
            value = JSON.parse(decodeURIComponent(args["value"]));
			gseCrypto.getInstance().templateProperty(value);
            success();
        } else {
            success(gseCrypto.getInstance().templateProperty());
        }
	},
	// Thread methods to start and stop
	startThread: function (success, fail) {
		success(gseCrypto.getInstance().startThread());
	},
	stopThread: function (success, fail) {
		success(gseCrypto.getInstance().stopThread());
	}
};