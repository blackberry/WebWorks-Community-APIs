/*
* Copyright 2010-2011 Research In Motion Limited.
* Copyright 2011 Matthew Haag
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
	var SYSTEM_API_URL = "blackberry/ext/system/log";
	var OK = 0;

	function makeCall(toFunction, functionArgs) {
		var request = new blackberry.transport.RemoteFunctionCall(SYSTEM_API_URL + "/" + toFunction);

		if(functionArgs) {
			for(var name in functionArgs) {
				request.addParam(name, functionArgs[name]);
			}
		}

		return request.makeSyncCall(); //don't care about the return value
	}

	function SystemLogDispatcher() {
	};
	var jsLog;
	SystemLogDispatcher.prototype.setup = function(theNamespace,theAppName,theMode) {
		var result = makeCall("setup", {namespace : theNamespace,appName : theAppName, mode : theMode});
		jsLog = theMode | 4;
		if (result.code >= OK) {
			return;
		} else {
			throw new Error(result.msg);
		}
	
	};
	SystemLogDispatcher.prototype.write = function(theMessage) {
		var result = makeCall("write", {message : theMessage});
		try{
			if(console && console.log){
				console.log(theMessage);
			}
		} catch(e){}
		return ;
	
	};

	blackberry.Loader.javascriptLoaded("blackberry.ext.system.log", SystemLogDispatcher);
})();
