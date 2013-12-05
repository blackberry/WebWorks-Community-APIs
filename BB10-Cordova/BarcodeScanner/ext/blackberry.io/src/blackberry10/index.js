/*
 * Copyright 2012 Research In Motion Limited.
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
var _util = require("./../../lib/utils"),
    _webview;

function getHomeDir() {
    var homeDir = window.qnx.webplatform.getApplication().getEnv("HOME");

    // HOME environment variable starts with two slashes for some reason, get rid of the extra slash
    if (homeDir.match("^//")) {
        return homeDir.substring(1);
    } else {
        return homeDir;
    }
}

module.exports = {
    sandbox: function (success, fail, args, env) {
        var value,
            result = new PluginResult(args, env);

        _webview = _util.requireWebview();

        if (args && args["sandbox"]) {
            value = JSON.parse(decodeURIComponent(args["sandbox"]));
            _webview.setSandbox(JSON.parse(value));
            result.ok(null, false);
        } else {
            value = _webview.getSandbox();
            result.ok(value === "1", false); // always return "0" or "1" even after explicitly setting value to true or false
        }
    },

    home: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok(getHomeDir(), false);
    },

    sharedFolder: function (success, fail, args, env) {
        var home = getHomeDir(),
            result = new PluginResult(args, env);
        result.ok(home + "/../shared", false);
    },

    SDCard: function (success, fail, args, env) {
        var result = new PluginResult(args, env);
        result.ok("/accounts/1000/removable/sdcard", false);
    }
};
