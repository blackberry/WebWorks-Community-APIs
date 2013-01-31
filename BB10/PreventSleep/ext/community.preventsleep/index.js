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

var preventsleep = require("./preventSleepJNEXT").preventsleep,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// Code can be declared and used outside the module.exports object,
	// but any functions to be called by client.js need to be declared
	// here in this object.

	// These methods call into preventsleepJNEXT.js which handles the
	// communication through the JNEXT plugin to preventsleep_js.cpp
	setPreventSleep: function (success, fail, args) {
		args = JSON.parse(decodeURIComponent(args["input"]));
		// look for the UIWebView in the set of WebViews.
		var views = qnx.webplatform.getWebViews();
		var handle = null;
		for (var i = 0; i < views.length; i++) {
			if (views[i].constructor.name == "UIWebView"){
				handle = views[i].jsScreenWindowHandle;
			}
		}
		if (handle !== null) {
			var values = { value: args, handle: handle };
			success(preventsleep.getInstance().setPreventSleep(values));
		} else {
			success("Unable to get window handle");
		}
	},
	isSleepPrevented: function (success, fail, args) {
		// Values from JNEXT are 0 or 1, and we want booleans
        success(preventsleep.getInstance().isSleepPrevented() === "1");
	}
};