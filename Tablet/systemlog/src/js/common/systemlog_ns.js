/*
* Copyright 2010-2011 Research In Motion Limited.
* Copyright 2011-2012 Matthew Haag Verivo Software
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
	
	function SystemLog(disp) {
			
		this.constructor.prototype.setup = function(namespace,appName,mode) {
			if(mode === undefined)
				mode = this.ALL;
			return disp.setup(namespace,appName,mode); 
			
		}
		this.constructor.prototype.write = function(message) { return disp.write(message); }
		
		/*
		 * Getters for public static properties
		 */
		this.constructor.prototype.SYSTEMLOG = 1;
		this.constructor.prototype.STANDARDOUT = 2;
		this.constructor.prototype.CONSOLE = 4;
		this.constructor.prototype.ALL = 7;
	};
	
	blackberry.Loader.javascriptLoaded("blackberry.ext.system.log", SystemLog);
})();
