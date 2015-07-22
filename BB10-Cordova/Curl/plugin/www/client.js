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
    _ID = "cordova-plugin-curl",
    exec = cordova.require("cordova/exec");

    // These methods are called by your App's JavaScript
    // They make WebWorks function calls to the methods
    // in the index.js of the Extension

    _self.get = function (input, onDone) {
        var callback = function (data, response) {
            if (typeof onDone === 'function') {
                onDone(data, response, input);
            } else {
                // print data by default
                console.log(input);
                console.log(data);
            }
        };
        
        input = input || "";

        exec(callback, callback, _ID, "get", {input: input}, true);
    };

module.exports = _self;
