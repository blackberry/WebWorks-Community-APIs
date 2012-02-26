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

    function Template(disp) {

		//Define get/set properties:
        this.constructor.prototype.__defineGetter__("bool", disp.getBoolean);
        this.constructor.prototype.__defineSetter__("bool", disp.setBoolean);
        this.constructor.prototype.__defineGetter__("string", disp.getString);
        this.constructor.prototype.__defineSetter__("string", disp.setString);
        this.constructor.prototype.__defineGetter__("integer", disp.getInteger);
        this.constructor.prototype.__defineSetter__("integer", disp.setInteger);
		
		//Define functions
        this.constructor.prototype.add = function(first, second) {
            return disp.add(first, second);
        };
        this.constructor.prototype.log = function() {
            return disp.log();
        };

    }

//must start with 'blackberry'
    blackberry.Loader.javascriptLoaded("blackberry.template", Template);
	
})();