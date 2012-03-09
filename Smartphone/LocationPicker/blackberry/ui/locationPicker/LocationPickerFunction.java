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
		/*if (!mapsExist()) {
			return UNDEFINED;
		}*/
		if (args.length == 1) {
			// Now get the callback method to fire
			final ScriptableFunction _callback = (ScriptableFunction)args[0];

			//final UiApplication uiApp = UiApplication.getUiApplication();	
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
			
			//final UiApplication uiApp = UiApplication.getUiApplication();	
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

