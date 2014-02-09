cordova.define("community.led.client", function(require, exports, module) {/*
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

var _self = {},
	_ID = "community.led",
	exec = cordova.require("cordova/exec");

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension
	
	_self.startLed = function (color, blinkCount) {
		var result, 
			success = function(data, response) {
				result = data; 
			}, 
			fail = function(data, response) {
				console.log("Error: " + data); 
			}; 
		var input = { "color": color, "blinkCount": blinkCount};
		exec(success, fail, _ID, "startLed", {input : input});
		return result; 
	};

	_self.stopLed = function (input) {
		var result, 
			success = function(data, repsonse) {
				result = data; 
			}, 
			fail = function(data, response) {
				console.log("Error: " + data); 
			}; 
		exec(success, fail, _ID, "stopLed", {input : input});
		return result; 
	};

module.exports = _self;});
