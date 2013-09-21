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

package webworks.io;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

/**
 * 
 * @author Jerome Carty
 *
 */
public class FileDownloaderExtension implements WidgetExtension {
	private WidgetConfig _widgetConfig;
	
	public FileDownloaderExtension()
	{
		Logger.enableLogging(0x9bfb4ae66b367958L, FileDownloaderNamespace.NAME);
	}
	
	public String[] getFeatureList() {
		return new String[] { FileDownloaderNamespace.NAME };
	}

	public void loadFeature(String feature, String version, Document doc,
			ScriptEngine scriptEngine) throws Exception {
		

		if (feature.equals(FileDownloaderNamespace.NAME)) {
			scriptEngine.addExtension(feature, new FileDownloaderNamespace(_widgetConfig));
		}		
	}

	public void register(WidgetConfig config, BrowserField bf) {
		_widgetConfig = config;
	}

	public void unloadFeatures(Document arg0) {
	}

}
