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
package blackberry.ui.file;


import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.TransitionContext;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.UiEngineInstance;
import net.rim.device.api.ui.*;
import net.rim.device.api.ui.picker.*;
import net.rim.device.api.ui.component.*;
import net.rim.device.api.ui.container.*;
import java.util.*;

/**
 * Opens the spinner control
 * 
 * @author tneil
 * 
 */
public class FunctionOpenPicker extends ScriptableFunction {

    public static final String NAME = "open";
	private ScriptableFunction _callback = null;

   
    public Object invoke(Object thiz, Object[] args) throws Exception {
		if (args.length == 1) {	
			// Now get the callback method to fire
			_callback = (ScriptableFunction)args[0];
			
			final UiApplication uiApp = UiApplication.getUiApplication();
			//uiApp.invokeLater (new SpinnerRunnable(_callback, selectedIndex, rowHeight, visibleRows, title, choices));	
			UiApplication.getUiApplication().invokeLater(new Runnable()
			{
				public void run()
				{
					FilePicker fp = FilePicker.getInstance();
					FilePickListener fileListener = new FilePickListener(_callback);
					fp.setListener(fileListener);           
					fp.show();  
				}
			});
			
		}
		return UNDEFINED;	  
    }
	
	class FilePickListener implements FilePicker.Listener 
	{   
		private ScriptableFunction _callback = null;
	
		public FilePickListener(ScriptableFunction callback) {
			_callback = callback;
		}
	
		public void selectionDone(String str)
		{
			Object[] result = new Object[1];
		
			// Record the result that the user chose. If they didn't choose an
			// option return undefined
			if(str != null) {    
				result[0] = new String(str);
			}  
			else {
				result[0] = UNDEFINED;
			}

			// Create a final thread safe result to pass into the thread object
			final Object[] threadedResult = result;
			
			// Create a new thread to make sure that the invoke of the JavaScript callback
			// does not initiate from the UI thread.  This can otherwise cause a deadlock scenario
			new Thread () {
				public void run() {
					try
					{
						// Pass the result of the spinner back to the handle of the JavaScript callback
						_callback.invoke(_callback, threadedResult);
					}
					catch (Exception e) {
						throw new RuntimeException(e.getMessage());
					}
				}
			}.start();
		}
	}
	
}


