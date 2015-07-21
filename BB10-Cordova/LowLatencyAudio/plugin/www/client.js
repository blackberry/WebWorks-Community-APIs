/*
* Copyright 2013-2014 BlackBerry Limited.
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

var service = "cordova-plugin-bb-nativeaudio",
    exec = cordova.require("cordova/exec");

module.exports = {
    preloadAudio: function (data, path, voices, success, fail) {
        exec(success, fail, service, "preloadAudio", { data: data, path: path, voices: voices }, false);
    },
    unload: function (data, success, fail) {
        exec(success, fail, service, "unload", { data: data }, false);
    },
    play: function (data, success, fail) {
        exec(success, fail, service, "play", { data: data }, false);
    },
    stop: function (data, success, fail) {
        exec(success, fail, service, "stop", { data: data }, false);
    },
    getDuration: function (data, success, fail) {
        exec(success, fail, service, "getDuration", { data: data }, false);
    },
    loop: function (data, success, fail) {
        exec(success, fail, service, "loop", { data: data }, false);
    }

};
