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
package blackberry.system.locale
{
	import qnx.locale.*;
	
	import webworks.extension.DefaultExtension;

	
	/**
	 * 
	 * Extension for retrieving the application and/or device's Locale
	 * 
	 */

	public class Locale extends DefaultExtension
	{	
			
		private const FEATUREID:Array = new Array ("blackberry.system.locale");
		
		
		public function Locale() {
			super();
		}
		
		override public function getFeatureList():Array {
			return FEATUREID;
		}
	
		public function app(): String {
			var lm:LocaleManager = LocaleManager.localeManager;
			return lm.getCurrentLocale();
		}
		
		public function device() : String {
			var lm:LocaleManager = LocaleManager.localeManager;
			return lm.getDeviceLocale();
		}
		
	}
}