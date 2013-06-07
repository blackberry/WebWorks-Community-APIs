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

var _self = {},
	_ID = require("./manifest.json").namespace;

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	// get Model Number
	_self.getModelNumber = function () {
		return window.webworks.execSync(_ID, "getModelNumber", null);
	};
	
	//get Mobile Carrier Code
	_self.getMCC = function () {
        return window.webworks.execSync(_ID, "getMCC", null);
    };
    
    //get Mobile Network Code
    _self.getMNC = function () {
        return window.webworks.execSync(_ID, "getMNC", null);
    };
module.exports = _self;
