/*
* Copyright 2010-2011 Research In Motion Limited.
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

(function (parent) {
    var _plugins = {},
        _fail = function () {
            throw "ummmm.... you really should be mocking this";
        };

    parent.blackberry =  {
        Loader: {
            javascriptLoaded: function (key, func) {
                _plugins[key] = _plugins[key] || [];
                _plugins[key].push(func);
            },

            getJavascript: function (key) {
                return _plugins[key];
            },

            clear: function(key) {
                delete _plugins[key];
            }
        },
        events: {
            registerEventHandler: _fail
        },
        transport: {
            RemoteFunctionCall: function (url) {
                blackberry.transport.last = {
                    _url: url,
                    addParam: jasmine.createSpy("RemoteFunctionCall.addParam"),
                    makeAsyncCall: jasmine.createSpy("RemoteFunctionCall.makeAsyncCall"),
                    makeSyncCall: jasmine.createSpy("RemoteFunctionCall.makeSyncCall")   
                };

                return blackberry.transport.last;
            },
            last: null
        }
    };
})(window);
