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

package webworks.message.email;

import org.w3c.dom.Document;

import webworks.message.email.impl.EmailListenerManager;
import webworks.message.email.namespace.EmailListenerNamespace;


import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;



public class EmailListenerExtension implements WidgetExtension {


	public String[] getFeatureList() {
		return new String[] { EmailListenerNamespace.NAME };
	}

	public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception {
		if(scriptEngine == null) {
			throw new IllegalArgumentException("scriptEngine should not be null");
		}
		
		if(EmailListenerNamespace.NAME.equals(feature)) {
			scriptEngine.addExtension(EmailListenerNamespace.NAME, new EmailListenerNamespace(new EmailListenerManager()));
		}

	}

	public void register(WidgetConfig widgetConfig, BrowserField browserField) {
		//Nothing to do here...
	}

	public void unloadFeatures(Document doc) {
		//Nothing to do here either...
	}

}
