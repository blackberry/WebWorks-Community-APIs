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
package webworks.bbalert.led;

import java.util.Enumeration;

import org.json.me.JSONArray;
import org.json.me.JSONException;
import org.json.me.JSONObject;
import java.lang.Integer;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Alert;
import net.rim.device.api.system.LED;

public class LedAlertNamespace extends Scriptable {
    public static final String FUNCTION_FLASH_LED = "flashLED";
    
    public Object getField(String name) throws Exception
    {
        if (name.equals(FUNCTION_FLASH_LED))
        {
            return new FlashLED();
        }
        return super.getField(name);
    }

    public class FlashLED extends ScriptableFunction
    {
		private int[] PATTERN = new int[] { 0x00B22234, 1000, 100,
				0x00FFFFFF, 1000, 100, 0x003C3B6E, 1000, 100 };
		private static final String USAGE = "Usage: start|stop, pattern JSON string, true|false. e.g. webworks.bbalert.led.flashLED('start', '[{" +
		"\"hex-color\": \"0x00B22234\",\"on-duration-in-milliseconds\": \"1000\", \"transition-time-in-milliseconds\": \"100\"}'" +
		", 'true'";
		private static final String HEX_COLOR = "hex-color";
		private static final String ON_DURATION = "on-duration-in-milliseconds";
		private static final String TRANSITION_TIME = "transition-time-in-milliseconds";
		private int [] customPattern = new int[10]; 
		private boolean loop = false;
        public Object invoke(Object obj, Object[] args) throws Exception
        {
			try {
				if (args.length == 0) {
					return FlashLED.USAGE;
				} else {
				
					// expecting two parameters for starting the flash and a single
					//parameter if it is called to stop
					int [] colorPattern;
					if (args[0].toString().equalsIgnoreCase("start")) {
						if (args[1] != null && args[1].toString().equals("")) {
							LED.setColorPattern(this.PATTERN, true);
						} else if (args[1] != null
								&& !args[1].toString().equals("")) {
							// set the color pattern int array here
							parseJson((String) args[1]);

							if (args[2].toString().equalsIgnoreCase("true")) {
								// loop true
								loop = true;
							} else {
								// loop false
								loop = false;
							}
							LED.setColorPattern(customPattern, loop);
						}
					} else if (args[0].toString().equalsIgnoreCase("stop")) {
						LED.setState(LED.STATE_OFF);
					} else {
						return FlashLED.USAGE;
					}

				}
			} catch (JSONException ex) {
				return "Exception encountered when starting or stopping the LED. " + ex.getMessage() + "\n " + FlashLED.USAGE;
			} catch(NumberFormatException ex) {
				return "Exception encountered when starting or stopping the LED. " + ex.getMessage() + "\n " + FlashLED.USAGE;
			}catch (Exception ex) {

				return "Exception encountered when starting or stopping the LED. " + ex.getMessage() + "\n " + FlashLED.USAGE;
			}
            

            return "START OR STOP LED FLASHING BASED ON PATTERN AND DURATION";
        }
    	private void parseJson(String jsonPattern) throws JSONException, NumberFormatException {
			try {
				JSONArray jsonArray = new JSONArray(jsonPattern);
				JSONObject jsonObj = null;
				Enumeration jsonKeys = null;
				String jsonObjKey = "";
				String hexColor = "";
				String onTime = "";
				String transitionTime = "";
				String patternArray [] = new String[10];
				String tmp = "";
				if (jsonArray != null && jsonArray.length() > 0) {
					// determine the total pattern array size
					int jsonItems = 0;
					for (int i = 0; i < jsonArray.length(); i++) {
						jsonObj = jsonArray.getJSONObject(i);
						jsonKeys = jsonObj.keys();
						while (jsonKeys.hasMoreElements()) {
							tmp = (String)jsonKeys.nextElement();
							jsonItems++;
						}
					}
					patternArray = new String[jsonItems];
				}
				int itemCount = 0;
				for (int i = 0; i < jsonArray.length(); i++) {
					jsonObj = jsonArray.getJSONObject(i);
					jsonKeys = jsonObj.keys();

					while (jsonKeys.hasMoreElements()) {
						jsonObjKey = (String) jsonKeys.nextElement();
						if (jsonObjKey.equalsIgnoreCase(HEX_COLOR)) {
							hexColor = jsonObj.getString(jsonObjKey);
							patternArray[itemCount] = hexColor;
							itemCount++;
							continue;
						} else if (jsonObjKey.equalsIgnoreCase(ON_DURATION)) {
							onTime = jsonObj.getString(jsonObjKey);
							patternArray[itemCount] = onTime;
							itemCount++;
							continue;
						} else if (jsonObjKey.equalsIgnoreCase(TRANSITION_TIME)) {
							transitionTime = jsonObj.getString(jsonObjKey);
							patternArray[itemCount] = transitionTime;
							itemCount++;
							continue;
						}

					}
				}
				//convert String array to an int array so that the pattern can be set.
				int tempInt;
				if(patternArray.length > 0) {
					customPattern = new int[patternArray.length];
					for(int i=0; i<patternArray.length; i++) {
						if(patternArray[i].startsWith("0x00")) {
							tempInt = Integer.parseInt(patternArray[i].substring(2, patternArray[i].length()), 16);
						} else {
							tempInt = Integer.parseInt(patternArray[i]);
						}
						customPattern[i] = tempInt;
					}
				}
			} catch (JSONException ex) {
				throw ex;
			} catch (NumberFormatException ex) {
				throw ex;
			}
		}
	}
	
	
}
