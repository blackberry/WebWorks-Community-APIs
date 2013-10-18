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
package blackberry.ui.spinner;


//import common.util.ArgumentValidationUtil;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.TransitionContext;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.UiEngineInstance;

/**
 * Opens the spinner control
 * 
 * @author tneil
 * 
 */
public class FunctionOpenSpinner extends ScriptableFunction {

    public static final String NAME = "open";
	private ScriptableFunction _callback = null;

   
    public Object invoke(Object thiz, Object[] args) throws Exception {
		if (args.length == 2) {
			// Get our configuration object
	        Scriptable config = (Scriptable)args[0];
			
			// Grab the selected Index to display
			int selectedIndex = ((Integer)config.getField("selectedIndex")).intValue();     
			int rowHeight  = ((Integer)config.getField("rowHeight")).intValue();
			int visibleRows  = ((Integer)config.getField("visibleRows")).intValue();
			String title = (String)config.getField("title");
			
			// Grab the items to display from the config object
			Scriptable stringArray  = (Scriptable)config.getField("items");
	        int count = stringArray.getElementCount();
	        String[] choices = new String[count];
	        for (int i=0; i<count; i++) {
	            choices[i] = stringArray.getElement(i).toString();
	        }
			
			// Now get the callback method to fire
			_callback = (ScriptableFunction)args[1];
			
			final UiApplication uiApp = UiApplication.getUiApplication();
			uiApp.invokeLater (new SpinnerRunnable(_callback, selectedIndex, rowHeight, visibleRows, title, choices));	
		}
		return UNDEFINED;	  
    }

	/**
	 * Provides a Runnable so that the Modal dialog can be opened asynchronously and the 
	 * JavaScript thread will continue to run without being blocked.
	 * 
	 * @author tneil
	 * 
	 */
	private class SpinnerRunnable implements Runnable {
		private ScriptableFunction _callback = null;
		private int _selectedIndex; 
		private int _rowHeight; 
		private int _visibleRows; 
		private String _title;
		private String[] _items;
		
		public SpinnerRunnable(ScriptableFunction callback, int selectedIndex, int rowHeight, int visibleRows, String title, String[] items) {
			_callback = callback;
			_selectedIndex = selectedIndex;
			_rowHeight = rowHeight;
			_visibleRows = visibleRows;
			_title = title;
			_items = items;
		}
		
		public void run() {
			try
			{
				// Create an instance of the popup screen
				final SpinnerScreen customSpinnerPopup = new SpinnerScreen(_title, 
						_items,
						_visibleRows,
						_rowHeight,
						_selectedIndex);
				
				final UiApplication uiApp = UiApplication.getUiApplication();
				
				// Now create the zooming transition effect to make the spinner appear
				TransitionContext transition = new TransitionContext(TransitionContext.TRANSITION_ZOOM);
				transition.setIntAttribute(TransitionContext.ATTR_DURATION, 250);
				transition.setIntAttribute(TransitionContext.ATTR_KIND, TransitionContext.KIND_IN);
				
				UiEngineInstance engine = uiApp.getUiEngineInstance();
				engine.setTransition(null, customSpinnerPopup, UiEngineInstance.TRIGGER_PUSH, transition);

				transition = new TransitionContext(TransitionContext.TRANSITION_ZOOM);
				transition.setIntAttribute(TransitionContext.ATTR_DURATION, 250);
				transition.setIntAttribute(TransitionContext.ATTR_KIND, TransitionContext.KIND_OUT);
				engine.setTransition(customSpinnerPopup, null, UiEngineInstance.TRIGGER_POP, transition); 
			
				// Push the screen to the foreground
				uiApp.pushModalScreen(customSpinnerPopup);
				Object[] result = new Object[1];
		
				// Record the result that the user chose. If they didn't choose an
				// option return undefined
				if(customSpinnerPopup.isSet()) {    
					result[0] = new Integer(customSpinnerPopup.getChoice());
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
			catch (Exception e)
			{
				throw new RuntimeException(e.getMessage());
			}
		}
	}
}


