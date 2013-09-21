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


import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.TransitionContext;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.UiEngineInstance;

/**
 * Subscribes an event to the start of scrolling on the screen
 * 
 * @author tneil
 * 
 */
public class FunctionOnScrollStart extends ScriptableFunction {

    public static final String NAME = "onScrollStart";
	private ScriptableFunction _callback = null;

   
    public Object invoke(Object thiz, Object[] args) throws Exception {
		if (args.length == 1) {
			// Now get the callback method to fire
			_callback = (ScriptableFunction)args[0];
		}
		return UNDEFINED;	  
    }
	
	public void trigger() {
		new Thread () {
			public void run() {
				try
				{
					// Trigger our event callback
					if (_callback != null) {
						Object[] args = new Object[0];
						_callback.invoke(_callback,args);
					}
				}
				catch (Exception e) {
					//throw new RuntimeException(e.getMessage());
				}
			}
		}.start();
	}
}


