/*
 Licensed to the Apache Software Foundation (ASF) under one
 or more contributor license agreements.  See the NOTICE file
 distributed with this work for additional information
 regarding copyright ownership.  The ASF licenses this file
 to you under the Apache License, Version 2.0 (the
 "License"); you may not use this file except in compliance
 with the License.  You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing,
 software distributed under the License is distributed on an
 "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND, either express or implied.  See the License for the
 specific language governing permissions and limitations
 under the License.
 */

var _self = {},
    _ID = "com.blackberry.ui.dialog",
    exec = cordova.require("cordova/exec");

function defineReadOnlyField(obj, field, value) {
    Object.defineProperty(obj, field, {
        "value": value,
        "writable": false
    });
}

_self.customAskAsync = function (message, buttons, callback, settings) {
    var args = { "message" : message, "buttons" : buttons, "callback" : callback};
    if (settings) {
        args.settings = settings;
    }
    exec(callback, function () {}, _ID, "customAskAsync", args);
};

_self.standardAskAsync = function (message, type, callback, settings) {
    var args = { "message" : message, "type" : type, "callback" : callback };
    if (settings) {
        args.settings = settings;
    }
    exec(callback, function () {}, _ID, "standardAskAsync", args);
};

defineReadOnlyField(_self, "D_OK", 0);
defineReadOnlyField(_self, "D_SAVE", 1);
defineReadOnlyField(_self, "D_DELETE", 2);
defineReadOnlyField(_self, "D_YES_NO", 3);
defineReadOnlyField(_self, "D_OK_CANCEL", 4);
defineReadOnlyField(_self, "D_PROMPT", 5);

module.exports = _self;
