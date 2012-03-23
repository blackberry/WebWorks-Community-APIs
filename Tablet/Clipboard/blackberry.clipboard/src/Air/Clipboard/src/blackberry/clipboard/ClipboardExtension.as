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


//------------------------------------------------------------------------------------
//
// CREATE YOUR OWN BLACKBERRY WEBWORKS EXTENSION WITH THIS TEMPLATE.
// GET STARTED BY COMPLETING ALL OF THE 'STEP' INSTRUCTIONS LISTED BELOW.
//
// 1. Package name
// 2. Class and constructor name
// 3. Feature names
// 4. Properties and functions
//
//
//  Consider contributing your extension to the BlackBerry WebWorks Community APIs:
//	   https://github.com/blackberry/WebWorks-Community-APIs
//
//------------------------------------------------------------------------------------


/**
*  An extension is a bridge between JavaScript, running in a WebWorks application, and 
*  developer APIs found in the underlying OS (J2ME for BlackBerry Smartphones and Adobe 
*  AIR for BlackBerry Tablet OS).
*
*  Using this technique, WebWorks developers can expose any available devices features  
*	in their HTML5 application content.
*
* Example (config.xml): 
*		<feature id="webworks.template" required="true" version="1.0.0.0"/>
*
* Example (JavaScript): 
*		webworks.template.add(5,6);   //11
*/


//
//STEP 1: Choose a unique package name that describes your extension.
//        Use the same package name in all classes that define this extension.
//
//		  e.g. "my.extension" or "companyname.description"
//
package blackberry.clipboard
{
	import flash.events.Event;
	import flash.desktop.Clipboard;
	import flash.desktop.ClipboardFormats;
	import webworks.extension.DefaultExtension;

	import qnx.events.QNXApplicationEvent;
	import qnx.system.QNXApplication;

//
//STEP 2: Rename this class and constructor to describe the primary purpose of this extension.
//		  e.g. HelloWorldExtension or LEDextension
//
	public class ClipboardExtension extends DefaultExtension
	{

//
//STEP 3: For each feature name, add it to an array. 
//

        private const FEATURE_ID:Array = new Array("blackberry.clipboard");

//
//STEP 2 (Continued): Rename this constructor to match the class name.
//

		public function ClipboardExtension()
		{
			super();
		}
/*
		public override function unloadFeature():void {
			webView.removeEventListener(Event.ACTIVATE,activate);
		}
*/		
		override public function getFeatureList():Array
		{
			return FEATURE_ID;
		}

//
//STEP 4: Replace the following section with any properties or functions that 
//        will be supported in your extension.
//

		//
		// Functions:
		//
		public function setText(value:String):void
		{
            Clipboard.generalClipboard.clear();
            Clipboard.generalClipboard.setData(ClipboardFormats.TEXT_FORMAT, value);
		}
		public function getText():String
		{
			return String(Clipboard.generalClipboard.getData(ClipboardFormats.TEXT_FORMAT));
		}

	}
}
