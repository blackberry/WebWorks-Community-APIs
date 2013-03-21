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

	_self.hash = function (input) {
		return window.webworks.execSync(_ID, "hash", { input: input});
	};

	_self.generateKey = function (input) {
		return window.webworks.execSync(_ID, "generateKey", { input: input });
	};
	
	_self.encrypt = function (input) {
		return window.webworks.execSync(_ID, "encrypt", { input: input});
	};
	
	_self.decrypt = function (input) {
		return window.webworks.execSync(_ID, "decrypt", { input: input});
	};
	
	_self.random = function (input) {
		return window.webworks.execSync(_ID, "random", { input: input});
	};
	
	/*
	_self.sign = function (input) {
		return window.webworks.execSync(_ID, "sign", { input: input});
	};
	
	_self.verify = function (input) {
		return window.webworks.execSync(_ID, "verify", { input: input});
	};

*/

module.exports = _self;
