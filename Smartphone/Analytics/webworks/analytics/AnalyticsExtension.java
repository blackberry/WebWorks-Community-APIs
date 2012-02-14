/*
* Copyright (c) 2012 Research In Motion Limited.
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

package webworks.analytics;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public class AnalyticsExtension implements WidgetExtension {

	private String FEATURE_SYSTEM_DISPLAY = "webworks.analytics";

	public String[] getFeatureList() {
		String[] result = new String[] { FEATURE_SYSTEM_DISPLAY };
		return result;
	}

	public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception {
		if (feature.equals(FEATURE_SYSTEM_DISPLAY)) {
			scriptEngine.addExtension(FEATURE_SYSTEM_DISPLAY, new AnalyticsNamespace());
		}
	}

	public void register(WidgetConfig config, BrowserField bf) {
		// TODO Auto-generated method stub
	}

	public void unloadFeatures(Document doc) {
		// TODO Auto-generated method stub
	}
}
