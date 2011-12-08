/*
 * Copyright 2010 Research In Motion Limited.
 * Copyright 2011 Matthew Haag
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * Steps to writing a custom ActionScript extension:
 * Updates necessary to this *.as file:
 *  1) Rename package and contents of FEATUREID Array to that of feature name defined in library.xml (e.g. "blackberry.customextension")
 *  2) Rename this *.as file, class name and constructor name to be the same value (e.g. "CustomExtension")
 *  3) Define necessary ActionScript import statements and methods to provide the functionality you want to provide through this extension.
 *
 *
 */
package blackberry.ext.system.log
{
    
    import webworks.config.ConfigConstants;
    import webworks.extension.DefaultExtension;

    public class SystemLogExtension extends DefaultExtension
    {
        private const FEATUREID : Array = new Array ("blackberry.ext.system.log");
        public static var debug : String = "";

        public function SystemLogExtension() {
        }

        override public function getFeatureList():Array
        {
            return FEATUREID;
        }

		private var namespace;
		private var appName;
		private var mode;
		public function setup(namespace:String,appName:String,mode:String):int {
			this.namespace = namespace;
			this.appName = appName;
			this.mode = parseInt(mode);
			return 0;
		}
		
		public function write(message:String):void{
			
			if(this.mode & 2 != 0)
				trace(message);
		}
	}
}
