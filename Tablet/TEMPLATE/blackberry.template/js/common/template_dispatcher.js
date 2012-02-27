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

	//
	//  Dispatches JavaScript property/function calls to ActionScript 
	//
    function TemplateDispatcher() {
    }

	//
	// Location of ActionScript code for this extension (found in src/Air/Template/src folder):
	//
    var TEMPLATE_EXTENSION_URL = "blackberry/template";
	
	//
	// Property and function names supported by this extension:
	//    
	var GET_BOOLEAN = "getBoolean";
    var SET_BOOLEAN = "setBoolean";
    var GET_STRING  = "getString";
    var SET_STRING  = "setString";
    var GET_INTEGER = "getInteger";
    var SET_INTEGER = "setInteger";
    var FUNCTION_ADD = "add";
    var FUNCTION_LOG = "log";


	//
	//Helper methods for property GET/SET:
	//
    function makeGetCall(method) {
        var recall = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" + method);
        return recall.makeSyncCall();
    }
    function makeSetCall(method, newValue) {
        var recall = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" + method);
        recall.addParam("value", newValue);
        recall.makeSyncCall();
    }

	//
	// Properties (GET/SET):
	//
	TemplateDispatcher.prototype.getBoolean = function() {
		return makeGetCall(GET_BOOLEAN);
    };
    TemplateDispatcher.prototype.setBoolean = function(newValue) {
		return makeSetCall(SET_BOOLEAN, newValue);
    };
	TemplateDispatcher.prototype.getString = function() {
		return makeGetCall(GET_STRING);
    };
    TemplateDispatcher.prototype.setString = function(newValue) {
		return makeSetCall(SET_STRING, newValue);
    };
	TemplateDispatcher.prototype.getInteger = function() {
		return makeGetCall(GET_INTEGER);
    };
    TemplateDispatcher.prototype.setInteger = function(newValue) {
		return makeSetCall(SET_INTEGER, newValue);
    };

	//
	// Functions:
	//
    TemplateDispatcher.prototype.add = function (first, second) {
        var request = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" +  FUNCTION_ADD);
        request.addParam("first", first);
        request.addParam("second", second);
        return request.makeSyncCall();		//use sync if you need a return value.
    };
    TemplateDispatcher.prototype.log = function () {
        var request = new blackberry.transport.RemoteFunctionCall(TEMPLATE_EXTENSION_URL + "/" +  FUNCTION_LOG);
        return request.makeAsyncCall();		//use async if you don't need a return value.
    };

	//
	// Unique namespace for this dispatcher (name must start with 'blackberry'):
	//
	blackberry.Loader.javascriptLoaded("blackberry.template", TemplateDispatcher);

})();