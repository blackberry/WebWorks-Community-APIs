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

var _self = {},
	_ID = "cordova-plugin-preventsleep",
  	_isAwake = false,
    exec = cordova.require("cordova/exec");

function invokeCallback(callback, args) {
    if (callback && typeof callback === "function") {
        callback(args);
    }
}

_self.setPreventSleep = function (option, onSuccess, onFail) {
    exec(function (result) {
        invokeCallback(onSuccess, result);
    }, function (error) {
        invokeCallback(onFail, error);
    }, _ID, "setPreventSleep", {input: option});
};

_self.getPreventSleepStatus = function (onSuccess, onFail) {
    exec(function (result) {
        invokeCallback(onSuccess, result);
    }, function (error) {
        invokeCallback(onFail, error);
    }, _ID, "isSleepPrevented", null);
};

module.exports = _self;