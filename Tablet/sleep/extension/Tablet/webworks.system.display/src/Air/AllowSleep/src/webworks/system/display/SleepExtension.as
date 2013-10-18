/*
 * Copyright 2010 Research In Motion Limited.
 *
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
package webworks.system.display
{
    import flash.desktop.NativeApplication;
    import flash.desktop.SystemIdleMode;

    import webworks.config.ConfigConstants;
    import webworks.extension.DefaultExtension;

    public class SleepExtension extends DefaultExtension
    {
        private const FEATUREID : Array = new Array ("webworks.system.display");
        public static var debug : String = "";

        public function SleepExtension() {
        }

        override public function getFeatureList():Array
        {
            return FEATUREID;
        }

        private function isSleepAllowed():Object {
            //figure out the current state

            //return NativeApplication.nativeApplication.systemIdleMode;

            if (NativeApplication.nativeApplication.systemIdleMode == SystemIdleMode.KEEP_AWAKE) {
                return false;
            }
            else {
                return true;
            }
        }

        public function normal():Object
        {
            try {
                NativeApplication.nativeApplication.systemIdleMode = SystemIdleMode.NORMAL;
		return true;
            }
            catch (ex:*) {
                debug = ex.description;
            }
	    return "error";
        }

        public function keepAwake():Object
        {
            try {
                NativeApplication.nativeApplication.systemIdleMode = SystemIdleMode.KEEP_AWAKE;
		return false;
            }
            catch (ex:*) {
                debug = ex.description;
            }
	    return "error";
        }

	public function allowSleep(sleep:String = "get"):Object {
	
		var dataObject:Object = new Object();
		

		switch (sleep) {
			case ("get"):
				dataObject["allowSleep"] = isSleepAllowed();
				break;
			case ("true"):
				dataObject["allowSleep"] = normal();
				//dataObject["allowSleep"] = NativeApplication.nativeApplication.systemIdleMode;
				break;
			case ("false"):
				dataObject["allowSleep"] = keepAwake();
				//dataObject["allowSleep"] = NativeApplication.nativeApplication.systemIdleMode;
				break;
			default:
				//invalid case. should do something
		}
		
		var returnObject:Object = {
			"code" : 0,
			"msg" : null,
			"data" : dataObject
		};

	    return returnObject;
	
	
	}
    }
}
