/*
 * Copyright 2011-2012 Research In Motion Limited.
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
var overlayWebView;

qnx.webplatform.getController().addEventListener('overlayWebView.initialized', function (webviewObj) {
    overlayWebView = webviewObj;
});

function trigger(result, type, arg) {
    var keepCallback = type === "invoke";

    result.callbackOk({
        "type": type,
        "result": arg
    }, keepCallback);
}

module.exports = {
    invokeMediaPlayer: function (success, fail, args, env) {
        var options = JSON.parse(decodeURIComponent(args.options)),
            result = new PluginResult(args, env),
            done = function (data) {
                trigger(result, "done", data);
            },
            cancel = function (reason) {
                trigger(result, "cancel", reason);
            },
            invokeCallback = function (error) {
                trigger(result, "invoke", error);
            };

        window.qnx.webplatform.getApplication().cards.mediaplayerPreviewer.open(options, done, cancel, invokeCallback);
        result.noResult(true);
    },

    invokeCamera: function (success, fail, args, env) {
        var mode = JSON.parse(decodeURIComponent(args.mode)),
            result = new PluginResult(args, env),
            done = function (path) {
                trigger(result, "done", path);
            },
            cancel = function (reason) {
                trigger(result, "cancel", reason);
            },
            invokeCallback = function (error) {
                trigger(result, "invoke", error);
            };
        if (mode === "") {
            mode = "photo";
        }
        window.qnx.webplatform.getApplication().cards.camera.open(mode, done, cancel, invokeCallback);
        result.noResult(true);
    },

    invokeFilePicker: function (success, fail, args, env) {
        var options = JSON.parse(decodeURIComponent(args.options)),
            result = new PluginResult(args, env),
            done = function (path) {
                trigger(result, "done", path);
            },
            cancel = function (reason) {
                trigger(result, "cancel", reason);
            },
            invokeCallback = function (error) {
                trigger(result, "invoke", error);
            };

        window.qnx.webplatform.getApplication().cards.filePicker.open(options, done, cancel, invokeCallback);
        result.noResult(true);
    },

    invokeIcsViewer: function (success, fail, args, env) {
        var options = JSON.parse(decodeURIComponent(args.options)),
            result = new PluginResult(args, env),
            done = function (path) {
                trigger(result, "done", path);
            },
            cancel = function (reason) {
                trigger(result, "cancel", reason);
            },
            invokeCallback = function (error) {
                trigger(result, "invoke", error);
            };

        window.qnx.webplatform.getApplication().cards.icsViewer.open(options, done, cancel, invokeCallback);
        result.noResult(true);
    },


    invokeCalendarPicker: function (success, fail, args, env) {
        var options = JSON.parse(decodeURIComponent(args.options)),
            result = new PluginResult(args, env),
            done = function (file) {
                trigger(result, "done", file);
            },
            cancel = function (reason) {
                trigger(result, "cancel", reason);
            },
            invokeCallback = function (error) {
                trigger(result, "invoke", error);
            };

        window.qnx.webplatform.getApplication().cards.calendar.picker.open(options, done, cancel, invokeCallback);
        result.noResult(true);
    },

    invokeCalendarComposer: function (success, fail, args, env) {
        var options = JSON.parse(decodeURIComponent(args.options)),
            result = new PluginResult(args, env),
            done = function (data) {
                trigger(result, "done", data);
            },
            cancel = function (reason) {
                trigger(result, "cancel", reason);
            },
            invokeCallback = function (error) {
                trigger(result, "invoke", error);
            };

        window.qnx.webplatform.getApplication().cards.calendar.composer.open(options, done, cancel, invokeCallback);
        result.noResult(true);
    },

    invokeEmailComposer: function (success, fail, args, env) {
        var options = JSON.parse(decodeURIComponent(args.options)),
            result = new PluginResult(args, env),
            done = function (data) {
                trigger(result, "done", data);
            },
            cancel = function (reason) {
                trigger(result, "cancel", reason);
            },
            invokeCallback = function (error) {
                trigger(result, "invoke", error);
            };

        window.qnx.webplatform.getApplication().cards.email.composer.open(options, done, cancel, invokeCallback);
        result.noResult(true);
    },

    invokeTargetPicker: function (success, fail, args, env) {
        var title = JSON.parse(decodeURIComponent(args.title)),
            pluginResult = new PluginResult(args, env),
            request = JSON.parse(decodeURIComponent(args.request)),
            invocation = window.qnx.webplatform.getApplication().invocation,
            onError,
            onSuccess;

        onError = function (error) {
            pluginResult.callbackOk({
                "status": "error",
                "result": error
            }, false);
        };

        onSuccess = function (result) {
            pluginResult.callbackOk({
                "status": "success",
                "result": result
            }, false);
        };

        // Pulled from the query code, we should probably keep a consistent API
        // allows the developers to define APPLICATION, VIEWER etc in an array
        if (request.target_type && Array.isArray(request.target_type)) {

            request.target_type = request.target_type.filter(function (element) {
                var result = false;
                switch (element)
                {
                case "APPLICATION":
                    request.target_type_mask |= invocation.TARGET_TYPE_MASK_APPLICATION;
                    break;
                case "CARD":
                    request.target_type_mask |= invocation.TARGET_TYPE_MASK_CARD;
                    break;
                case "VIEWER":
                    request.target_type_mask |= invocation.TARGET_TYPE_MASK_VIEWER;
                    break;
                case "SERVICE":
                    request.target_type_mask |= invocation.TARGET_TYPE_MASK_SERVICE;
                    break;

                default:
                    result = true;
                    break;
                }
                return result;
            });

            delete request.target_type;
        }

        if (request.hasOwnProperty('metadata')) {
            //Let's stringify it for them
            request.metadata = JSON.stringify(request.metadata);
        }

        overlayWebView.invocationlist.show(request, title, onSuccess, onError);
        pluginResult.noResult(true);
    }
};

