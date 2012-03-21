/*
* Copyright 2010-2012 Research In Motion Limited.
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
package blackberry.ui.locationPicker;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public final class LocationPickerExtension implements WidgetExtension {

	private final static String[] _features = { LocationPickerNamespace.NAME };
	
	public String[] getFeatureList() {
		return _features;
	}

	public void loadFeature(String feature, String version, Document doc,
			ScriptEngine scriptEngine) throws Exception {
		if (feature.equals(LocationPickerNamespace.NAME)) {
            scriptEngine.addExtension(feature, new LocationPickerNamespace());
        }

	}

	public void register(WidgetConfig widgetConfig, BrowserField browserField) {

	}

	public void unloadFeatures(Document doc) {

	}

}
