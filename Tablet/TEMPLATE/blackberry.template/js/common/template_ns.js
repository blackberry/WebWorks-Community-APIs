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
(function () {

	//
	//An interface and partial implementation for this extension:
	//
	function Template(dispatch) {

		//
		// Properties (GET/SET):
		//
		this.constructor.prototype.__defineGetter__("bool", dispatch.getBoolean);
		this.constructor.prototype.__defineSetter__("bool", dispatch.setBoolean);
		this.constructor.prototype.__defineGetter__("string", dispatch.getString);
		this.constructor.prototype.__defineSetter__("string", dispatch.setString);
		this.constructor.prototype.__defineGetter__("integer", dispatch.getInteger);
		this.constructor.prototype.__defineSetter__("integer", dispatch.setInteger);
		
		//
		// Functions:
		//
		this.constructor.prototype.add = function(first, second) {
			return dispatch.add(first, second);
		};
		this.constructor.prototype.log = function(msg) {
			return dispatch.log(msg);
		};

	}

	//
	// Unique namespace for this extension (name must start with 'blackberry'):
	//
	blackberry.Loader.javascriptLoaded("blackberry.template", Template);
	
})();