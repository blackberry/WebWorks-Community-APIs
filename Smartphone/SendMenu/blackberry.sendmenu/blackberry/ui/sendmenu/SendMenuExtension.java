/*
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

package blackberry.ui.sendmenu;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

import java.lang.ref.WeakReference;

public class SendMenuExtension implements WidgetExtension 
{
	private static WeakReference _browserField = null;
    private String NAME = "blackberry.ui.sendmenu";
		
    /**
     * @see net.rim.device.api.web.WidgetExtension#getFeatureList()
     */
	public String[] getFeatureList()
	{
		return new String[] { NAME };
	}

	/**
     * @see   net.rim.device.api.web.WidgetExtension#loadFeature(String, String, Document, ScriptEngine)
	 */
	public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception
	{
		Object obj = null;
		if (feature.equals(NAME))
		{
			obj = new SendMenuNamespace();
		}

		if( obj != null ) {
			scriptEngine.addExtension( feature, obj );
		}
	}
	
    /**
     * Get the current BrowserFiled. 
     * 
     * @return the current BrowserFiled
     */
    public static BrowserField getBrowserField() {
        return ( BrowserField ) _browserField.get();
    }
	
	/**
     * @see   net.rim.device.api.web.WidgetExtension#register(WidgetConfig, BrowserField)
	 */
	public void register(WidgetConfig config, BrowserField bf)
	{
		_browserField = new WeakReference(bf);
	}

	
	/**
     * @see   net.rim.device.api.web.WidgetExtension#unloadFeatures(Document)
	 */
	public void unloadFeatures(Document doc)
	{
		// no cleanup required
	}


}