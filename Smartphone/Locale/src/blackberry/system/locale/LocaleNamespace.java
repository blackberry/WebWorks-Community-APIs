/*
* Copyright 2010 Research In Motion Limited.
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
package blackberry.system.locale;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.ui.ScrollChangeListener;
import net.rim.device.api.ui.Manager;
import net.rim.device.api.ui.VirtualKeyboard;
import net.rim.device.api.i18n.Locale;



public class LocaleNamespace extends Scriptable {

	private static final String FIELD_DEVICELOCALE = "deviceLocale";
	private static final String FIELD_APPLOCALE = "appLocale";
	public static final String NAME = "blackberry.system.locale"; 

	
/*	public LocaleNamespace() {
		
			
	}*/

    public Object getField(String name) throws Exception {
        if (name.equals(FIELD_DEVICELOCALE)) {
            return new String(Locale.getDefaultForSystem().toString());	  
        }
		else if (name.equals(FIELD_APPLOCALE)) {
            return new String(Locale.getDefault().toString());	  
        }
		
		
        return super.getField(name);
    }

    public boolean putField(String field, Object value) throws Exception {
		return super.putField(field, value);
    }
	
	
	
}

