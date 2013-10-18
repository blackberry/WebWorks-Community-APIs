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
package blackberry.ui.screen;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.Screen;
import net.rim.device.api.ui.VirtualKeyboard;

/**
 * Set's the title of this screen
 * 
 * @author tneil
 * 
 */
public class FunctionHideKeyboard extends ScriptableFunction {

    public static final String NAME = "hideKeyboard";
	private BrowserField _browserField = null;

	public FunctionHideKeyboard(BrowserField browserField) {
		_browserField = browserField;
	}
   
    public Object invoke(Object thiz, Object[] args) throws Exception {	
		VirtualKeyboard vk = _browserField.getScreen().getVirtualKeyboard();
		if (vk != null && vk.isSupported()) {
			vk.setVisibility(VirtualKeyboard.HIDE);
		}
		return UNDEFINED;	  
    }

}


