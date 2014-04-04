/*
* index.js
*
* Defines the JS server side of WebWorks BB10 screenshot extension
*
* https://github.com/jonwebb/webworks-bb10-screenshot
*
* Author: Jon Webb, jon@jonwebb.net
*
* Portions Copyright 2013 Innovatology.
* Portions Copyright 2012 Research In Motion Limited.
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

var template = require("./templateJNEXT").template,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {
	// These methods call into templateJNEXT.js which handles the
	// communication through the JNEXT plugin to template_js.cpp
	
	/**
	* hello() returns version info
	*/
	hello: function (success, fail, args) {
		var userargs = JSON.parse(decodeURIComponent(args["userargs"]));
		success(template.getInstance().hello({userargs:userargs}));
	},

	/**
	* execute() finds the window handle, processes userargs then performs the screenshot
	*/
	execute: function (success, fail, args) {
		// userargs are encoded and need decoding & parsing into a json object
		var userargs = JSON.parse(decodeURIComponent(args["userargs"]));
		// which window handle should we use?
		var wv = qnx.webplatform.getWebViews();
		for(var i=0;i<wv.length;i++){
			if (wv[i].zOrder==0) {
				// found window handle.
				success(
					template.getInstance().execute(
						{
							handle:wv[i].jsScreenWindowHandle, 
							userargs:userargs
						}
					)
				);
				return;
			}
		}
		fail("error:no window handle");
	}
};