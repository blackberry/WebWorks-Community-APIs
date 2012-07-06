/*
 * Copyright 2011-2012 Research In Motion Limited.
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
package widgetpackage;

import org.w3c.dom.Document;
import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public final class NfcExtension implements WidgetExtension {

    private static final String FEATURES[] = { "blackberry.nfc" }; 
    private static final Class FEATURE_CLASS[] = { ScriptableNfc.class };
    
    public String[] getFeatureList() {
        return FEATURES;
    }

    public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception {
        for (int i=0; i<FEATURES.length; ++i) {
            if (FEATURES[i].equals(feature)) {
                scriptEngine.addExtension(feature,FEATURE_CLASS[i].newInstance());
            }
        }
    }

    public void register(WidgetConfig config, BrowserField bf) {
    }

    public void unloadFeatures(Document doc) {
    }
}
