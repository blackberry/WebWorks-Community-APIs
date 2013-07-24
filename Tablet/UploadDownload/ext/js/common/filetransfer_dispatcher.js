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
	var FILETRANSFER_API_URL = "webworks/io/fileTransfer",
        REMOTE_PATH = "remotePath",
        LOCAL_PATH = "localPath",
        ON_PROGRESS_ID = "onProgressID",
        ON_ERROR_ID = "onErrorID",
        OPTIONS = "options",
        EVENT_PROGRESS = "onFileTransferProgress",
        EVENT_ERROR = "onFileTransferError";

    function _isDefined(item) {
        return (!!item);
    }

    function _doAction(remotePath, localPath, onProgress, onError, options, action) {
		var onProgressId = blackberry.events.registerEventHandler(EVENT_PROGRESS, onProgress),
            onErrorId;

        if (onError && typeof onError == "function") {
            onErrorId = blackberry.events.registerEventHandler(EVENT_ERROR, onError);
        }
            
		makeCall(action, {
			REMOTE_PATH : remotePath, 
			LOCAL_PATH : localPath, 
			ON_PROGRESS_ID : onProgressId, 
			ON_ERROR_ID : onErrorId, 
			OPTIONS : JSON.stringify(options) 
		});
    }
 
	function FileTransferDispatcher() {
	}
    
    function makeCall(toFunction, functionArgs) {
		var request = new blackberry.transport.RemoteFunctionCall(FILETRANSFER_API_URL + "/" + toFunction);

		if(functionArgs) {
			for(var name in functionArgs) {
                if (functionArgs[name] !== undefined) {
                    console.log(name + " || " + functionArgs[name])
                    request.addParam(name, functionArgs[name]);
                }
			}
		}

		request.makeAsyncCall();  //Don't care about the return value
	}	
     
	FileTransferDispatcher.prototype.download = function(remotePath, localPath, onProgress, onError, options) {
        _doAction(remotePath, localPath, onProgress, onError, options, "download");
	};
    
    FileTransferDispatcher.prototype.upload = function(remotePath, localPath, onProgress, onError, options) {
        _doAction(remotePath, localPath, onProgress, onError, options, "upload");
	};

	blackberry.Loader.javascriptLoaded("blackberry.io.fileTransfer", FileTransferDispatcher);
})();
