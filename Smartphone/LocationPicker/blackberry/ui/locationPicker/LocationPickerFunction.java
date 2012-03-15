
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
package blackberry.ui.locationPicker;

import javax.microedition.location.Landmark;
import javax.microedition.location.QualifiedCoordinates;

import net.rim.device.api.lbs.picker.LocationPicker;
import net.rim.device.api.lbs.picker.MapsLocationPicker;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.UiApplication;

public final class LocationPickerFunction extends ScriptableFunction {
	
	public static final String NAME = "open";
	//private 
	
	public Object invoke(Object obj, Object[] args) throws Exception
	{
		if (args.length == 1) {
			// Now get the callback method to fire
			final ScriptableFunction _callback = (ScriptableFunction)args[0];

			UiApplication.getUiApplication().invokeLater(new Runnable()
			{
				public void run()
				{
					UiApplication.getUiApplication().pushScreen(new MapFieldSelectionScreen(_callback, null));
				
				}
			});
		}else if (args.length == 2){
			final ScriptableFunction _callback = (ScriptableFunction)args[0];
			
			//if they pass the optional parameter. The default location
			final Scriptable ops = (Scriptable) args[1];
			
			UiApplication.getUiApplication().invokeLater(new Runnable()
			{
				public void run()
				{
					UiApplication.getUiApplication().pushScreen(new MapFieldSelectionScreen(_callback, ops));
				
				}
			});
			
		}
		return UNDEFINED;
	}

	

}

