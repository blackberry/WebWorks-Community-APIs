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
(function() {

    function _request(method, success, error, options, data) {
        var request = new blackberry.transport.RemoteFunctionCall("webworks/media/barcode/" + method);
        
        if(data) {
	        request.addParam("data", data);
        }

        request.addParam("onCapturedId", success);

        if (error !== undefined) {
            request.addParam("onErrorId", error);
        }

        if (options) {
            request.addParam("options", JSON.stringify(options));
        }
        return request;
    }

    function BarcodeDispatcher() {
    }

    BarcodeDispatcher.prototype.scan = function(onCapturedId, onErrorId, options) {
        if (options) {
            options.tryHarder = options.tryHarder === false || options.tryHarder ? options.tryHarder : true;
            options.format = options.format || ["1d", "2d"];
        }
        var request = _request("scan", onCapturedId, onErrorId, options);
        request.makeAsyncCall();
    };

    BarcodeDispatcher.prototype.generate = function(data, onCapturedId, onErrorId, options) {
        if (options) {
            options.width = options.width >= 0 ? options.width : 50;
            options.height = options.height >= 0 ? options.height : 50;
            options.format = options.format || "QR";
        }

        var request = _request("generate", onCapturedId, onErrorId, options, data);
        request.makeAsyncCall();
    };
    
    blackberry.Loader.javascriptLoaded("blackberry.media.barcode", BarcodeDispatcher);
})();
