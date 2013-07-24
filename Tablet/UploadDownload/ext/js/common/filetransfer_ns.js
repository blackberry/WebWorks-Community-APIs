/*
* Copyright 2010-2011 Research In Motion Limited.
*
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
    function _validateArgs(remotePath, localPath, onProgress) {
        if (!remotePath) {
            throw "remotePath is needed";
        }

        if (!localPath) {
            throw "localPath is needed";
        }

        if (!onProgress) {
            throw "onProgress is needed";
        }
        else {
            if (typeof onProgress != "function") {
                throw "onProgress must be a function"
            }
        }
    }

	function FileTransfer(disp) {
           
		this.constructor.prototype.download = function(remotePath, localPath, onProgress, onError, options) {
            _validateArgs.apply(null, arguments);
            disp.download(remotePath, localPath, onProgress, onError, options);
        };
        
		this.constructor.prototype.upload = function(remotePath, localPath, onProgress, onError, options) {
            _validateArgs.apply(null, arguments);
            disp.upload(remotePath, localPath, onProgress, onError, options);
        };
        
        this.constructor.prototype.getRemoteFileSize = function(remotePath) {
            var req = new XMLHttpRequest();
            req.open('head', remotePath, false);
            req.send(null);
            return req.getResponseHeader("Content-length");
        };
	}

	blackberry.Loader.javascriptLoaded("blackberry.io.fileTransfer", FileTransfer);
})();
