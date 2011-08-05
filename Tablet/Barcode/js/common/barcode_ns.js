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
(function () {

    function _handler(name, f) {
        return f ? blackberry.events.registerEventHandler(name, f) : undefined;
    }

    function Barcode (disp) {
        this.constructor.prototype.scan = function (onCaptured, onError, options) {
            if (!onCaptured) {
                throw "must provide an onCaptured callback";
            }

            var cap = _handler("onCaptured", onCaptured),
                error = _handler("onError", onError);

            disp.scan(cap, error, options);
        };
        this.constructor.prototype.generate = function (data, onCaptured, onError, options) {
            if (!data) {
                throw "must provide data";
            }

            if (!onCaptured) {
                throw "must provide an onCaptured callback";
            }

            var cap = _handler("onCaptured", onCaptured),
                error = _handler("onError", onError);

            disp.generate(data, cap, error, options);
        };
    }

    blackberry.Loader.javascriptLoaded("blackberry.media.barcode", Barcode);
})();
