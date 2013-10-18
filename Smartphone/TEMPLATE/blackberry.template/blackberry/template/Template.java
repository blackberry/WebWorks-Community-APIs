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
// 1. Namespace
// 2. Class name
// 3. Define feature names
// 4. Return list of feature names
// 5. Associate namespace class with each feature
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
*  Using this technique, WebWorks developers can expose any native feature available from the 
*	BlackBerry OS in their HTML5 application content.
*
* Example (config.xml): 
*		<feature id="webworks.template" required="true" version="1.0.0.0"/>
*
* Example (JavaScript): 
*		webworks.template.add()
*/


//
//STEP 1: Choose a unique package name that describes your extension.
//        Use the same package name in all classes that define this extension.
//
//		  e.g. "my.extension" or "companyname.description"
//
package blackberry.template;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

import java.lang.ref.WeakReference;

//
//STEP 2: Rename this class to describe the primary purpose of this extension.
//		  e.g. HelloWorldExtension or LEDextension
//
public class Template implements WidgetExtension 
{
	private WeakReference _weakRefBrowserField = null;

//
//STEP 3: Define one or more feature names here. A feature name is used to register 
//		  this extension with the JavaScript engine and is white listed in a WebWorks
//		  application's config.xml file. You can organize the logic of your extension 
//	  	  by grouping them under different feature names.
//
//	Example: "webworks" or "companyname.xyz" or "barcode.scanner"
//
    private String FEATURE_WEBWORKS_TEMPLATE = "blackberry.template";

//
//STEP 4: For each feature name, add it to an array. 
//
        String[] _features = new String[] { FEATURE_WEBWORKS_TEMPLATE };

		
	/**
	 * Provides a list of all features supported by this extension. If this method does not 
     * return a feature name, that feature cannot be used in a WebWorks application.
	 * @return an array of features 
     * @see    net.rim.device.api.web.WidgetExtension#getFeatureList()
	 */
	public String[] getFeatureList()
	{
		System.out.println("Template.getFeatureList");

		return _features;
	}


	/**
	 * Load a feature defined in config.xml into the JavaScript engine.
	 *
     * @param feature      name of the feature  as defined by config.xml
     * @param version      version of the feature as defined by config.xml
     * @param doc          the HTML document for which this extension is being loaded 
	 *					   (e.g. index.html)
     * @param scriptEngine reference to the instance of the JavaScript engine used 
	 *        by the WebWorks app
     * @see   net.rim.device.api.web.WidgetExtension#loadFeature(String, String, Document, ScriptEngine)
	 */
	public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception
	{
		Object obj = null;
		System.out.println("Template.loadFeature(" + feature + ")");
  
//
//STEP 5: For each feature name defined above, add a namespace to the script engine.  The logic 
//        for your extension will exist within this namespace, and it is through the script 
//        engine of your WebWorks app that you can make calls into this extension.
//
		if (feature.equals(FEATURE_WEBWORKS_TEMPLATE))
		{
			obj = new TemplateNamespace();
			//
			//	TIP: You can pass values into the namespace object through its constructor.
			//
			//  e.g. Passing the reference of the active web view into the namespace:
			//		obj = new TemplateNamespace(_weakRefBrowserField);
			//
		}

		if( obj != null ) {
			scriptEngine.addExtension( feature, obj );
		}
	}

	
	/**
	 * Handle the extension registration event. Called when this extension is first loaded.
	 * 
     * @param config  the config.xml document used by the WebWorks app
     * @param bf      the browserfield object of the WebWorks app
     * @see   net.rim.device.api.web.WidgetExtension#register(WidgetConfig, BrowserField)
	 */
	public void register(WidgetConfig config, BrowserField bf)
	{
		System.out.println("Template.register");
		
		//
		//	TIP: Always wrap objects using a weak reference in order to avoid memory leaks.
		//		Not doing so results in GC being unable to reclaim the reference to this memory.
		//
		_weakRefBrowserField = new WeakReference(bf);
	}

	
	/**
	 * Handles the extension unload event. Called when the application is closed or the user 
	 * navigates to different page.
	 * 
     * @param doc  the HTML document for which this extension is being unloaded from (e.g. index.html)
     * @see   net.rim.device.api.web.WidgetExtension#unloadFeatures(Document)
	 */
	public void unloadFeatures(Document doc)
	{
		System.out.println("Template.unloadFeatures");

		//  Can be used to cleanup any unmanaged resources or to audit usage.		
	}


}