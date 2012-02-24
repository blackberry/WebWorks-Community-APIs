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
    var TEMPLATE_EXTENSION_URL = "webworks/template";
	
	//define methods
    var FUNCTION_ADD = "add";
    var FUNCTION_LOG = "log";
	
	//define setter/getter for properties:
    var GET_BOOLEAN = "getBoolean";
    var SET_BOOLEAN = "setBoolean";
    var GET_STRING  = "getString";
    var SET_STRING  = "setString";
    var GET_INTEGER = "getInteger";
    var SET_INTEGER = "setInteger";

    function TemplateExtensionDispatcher() {
    }

	//Helper methods for Get/Set calls to properties:
    function makeGetCall(method) {
        var recall = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" + method);
        return recall.makeSyncCall();
    }
    function makeSetCall(method, newValue) {
        var recall = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" + method);
        recall.addParam("value", newValue);
        recall.makeSyncCall();
    }
	
	//Get/Set methods for properties:
	TemplateExtensionDispatcher.prototype.getBoolean = function() {
		makeGetCall(GET_BOOLEAN);
    };
    TemplateExtensionDispatcher.prototype.setBoolean = function(newValue) {
		makeSetCall(SET_BOOLEAN, newValue);
    };
	TemplateExtensionDispatcher.prototype.getString = function() {
		makeGetCall(GET_STRING);
    };
    TemplateExtensionDispatcher.prototype.setString = function(newValue) {
		makeSetCall(SET_STRING, newValue);
    };
	TemplateExtensionDispatcher.prototype.getInteger = function() {
		makeGetCall(GET_INTEGER);
    };
    TemplateExtensionDispatcher.prototype.setInteger = function(newValue) {
		makeSetCall(SET_INTEGER, newValue);
    };

	
	//Functions:
    TemplateExtensionDispatcher.prototype.add = function (first, second) {
        var request = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" +  FUNCTION_ADD);
        request.addParam("first", first);
        request.addParam("second", second);
        request.makeAsyncCall();
    };
    TemplateExtensionDispatcher.prototype.log = function () {
        var request = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" +  FUNCTION_LOG);
        request.makeAsyncCall();
    };
    
	
	
	blackberry.Loader.javascriptLoaded("webworks.template", TemplateExtensionDispatcher);

})();