/*
* Copyright 2011 Research In Motion Limited.
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

package webworks.message.list;

import org.w3c.dom.Document;
import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public final class MessageListExtension  implements WidgetExtension
{
	
	private static final String FEATURE_ID = "webworks.message.list";
	
	public String[] getFeatureList() {
		String[] result = new String[1];
		result[0] = FEATURE_ID;
		return result;
	}

	public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception {
		if (feature == FEATURE_ID)
		{
			scriptEngine.addExtension(FEATURE_ID, new MessageListNamespace());
		}
	}

	public void register(WidgetConfig widgetConfig, BrowserField browserField) {
		// TODO Auto-generated method stub
		
	}

	public void unloadFeatures(Document doc) {
		// TODO Auto-generated method stub
		
	}

}
