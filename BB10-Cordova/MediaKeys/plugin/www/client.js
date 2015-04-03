/*
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
    _ID = "com.blackberry.community.mediakeys",
    exec = cordova.require("cordova/exec");

    _self.bind = function (mediaKeysObj, fail) {

        // if it is an list, then it is a list of media key JSON objects
        // otherwise its just an individual JSON object
        //
        // if it is an individual JSON object, convert it to a one element list
        if (typeof mediaKeysObj === 'object' && !Array.isArray(mediaKeysObj)) {
            mediaKeysObj = [mediaKeysObj];
        }

        if (fail === null || typeof fail !== 'function') {
            fail = function(error) { console.log("Failed to bind media keys: "+error); };
        }

        var success = function (data, response, args) {
            var mediaKeyObj = null;

            mediaKeysObj.some(function (obj) {
                if (obj.mediakey == data.mediakey && obj.keylength == data.keylength) {
                    mediaKeyObj = obj;
                }

                return mediaKeyObj;
            });

            if (mediaKeyObj.onPressed === null || typeof mediaKeyObj.onPressed !== 'function') {
                fail('Invalid onPressed callback');
            } else {
                mediaKeyObj.onPressed();
            }
        };

        var formattedMediaKeyObject = {
            'mediakeys': mediaKeysObj
        };

        // asynchronously bind keys
        exec(success, fail, _ID, "bind", formattedMediaKeyObject, false);
    };

    _self.show = function(message, buttons, settings, onOptionSelected, onFail) {
        var args = {"message" : message, "buttons" : buttons };
        if (settings) {
            args.settings = settings;
        }
        var defaultCallback = function (data, response, args) {
            console.log("data: " + data);
            console.log("response: " + response);
            console.log("input: " + args);
        };
        var success = function (data, response, args) {
            if (typeof onOptionSelected === 'function') {
                onOptionSelected(data);
            } else {
                console.log("mediakeys.show() success: buttons[" + data + "] is selected." );
                defaultCallback(data, response, args);
            }
        };
        var fail = function (data, response, args) {
            if (typeof onFail === 'function') {
                onFail(data);
            } else {
                console.log("mediakeys.show() fail: " + data);
                defaultCallback(data, response, args);
            }
        };

        // makeAsyncCall
        exec(success, fail, _ID, "show", args, false);
    };

module.exports = _self;
