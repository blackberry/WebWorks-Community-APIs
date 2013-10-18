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
package webworks.bbalert.vibrate;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Alert;

public class VibrateNamespace extends Scriptable {
    public static final String FUNCTION_MANAGE = "manage";
    
    public Object getField(String name) throws Exception
    {
        if (name.equals(FUNCTION_MANAGE))
        {
            return new DeviceVibrate();
        }
        return super.getField(name);
    }

    public class DeviceVibrate extends ScriptableFunction
    {

        public Object invoke(Object obj, Object[] args) throws Exception
        {
            if(args.length == 0) {
            	return UNDEFINED;
            } else {
            	/* arguments greater than one. When the Javascript method is called with
            	 * a value that can be parsed as a value in milliseconds, start vibrating
            	 * the device. Else, look for the keyword stop to stop the vibration 
            	 */
            	String vibrateParameter = args[0].toString();
            	int duration;
            	boolean timeValProvided = true;
            	try {
            		//durations over 25500 throw a illegal argument exception if a value from
            		//the javascript is greater than 25500. If a value is greater than 25500
            		//set the vibration time to the maximum.
            		duration = Integer.parseInt(vibrateParameter);
            		if(duration <= 25500) {
            			Alert.startVibrate(duration);
            		} else {
            			
            			duration = 25500;
            			Alert.startVibrate(duration);
            		}

            	} catch (NumberFormatException ex) {
            		//trap the number format exception and check for the stop keyword 
            		timeValProvided = false;
            	}
            	if (!timeValProvided) {
            		if(vibrateParameter.equalsIgnoreCase("stop")) {
            			Alert.stopVibrate();

            		}
            	}
            	
            }

            return "Vibration started or stopped";
        }
    }
	
	
}
