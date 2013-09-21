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
package webworks.device.backlight;


import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Backlight;



public class BacklightNamespace extends Scriptable {

    public static final String FUNCTION_BACKLIGHT = "manageBacklight";
    
    public static final String BACKLIGHT_ENABLE = "backlightEnable";
    public static final String GET_BRIGHTNESS = "getBrightness";
    public static final String GET_BRIGHTNESS_DEFAULT = "getBrightnessDefault";
    public static final String GET_BRIGHTNESS_INCREMENT = "getBrightnessIncrement";
    public static final String GET_TIMEOUT_DEFAULT = "getTimeoutDefault";
    public static final String IS_BRIGHTNESS_CONFIGURABLE = "isBrightnessConfigurable";
    public static final String IS_ENABLED = "isEnabled";
    public static final String SET_BRIGHTNESS = "setBrightness";
    public static final String SET_TIMEOUT = "setTimeout";
    
    public Object getField(String name) throws Exception
    {
        if (name.equals(FUNCTION_BACKLIGHT))
        {
            return new BacklightInfoProperties();
        }
        return super.getField(name);
    }

    public class BacklightInfoProperties extends ScriptableFunction
    {

        public Object invoke(Object obj, Object[] args) throws Exception
        {
        	String jsonString = "";
        	String functionParameter = "";
            if(args.length == 0) {
            	return UNDEFINED;
            } else {
            	try {
            		
            		if(args[0].toString().equalsIgnoreCase("all") || args[0].toString().equals("")) {
            			jsonString = getBacklightProperties("all");
            			return jsonString;
            		} else if(args[0].toString().equalsIgnoreCase(BacklightNamespace.BACKLIGHT_ENABLE)) {
            			if(args[1] != null || args[1].equals("")) {
            			if(args[1].toString().equalsIgnoreCase("on")) {
            				Backlight.enable(true);
            				return "Turned on the backlight";
            			} else if(args[1].toString().equalsIgnoreCase("off")) {
            				Backlight.enable(false);
            				return "Turned off the backlight";
            			}
            			return "Unable to turn on or off the backlight";
            			}
            		} else if(args[0].toString().equalsIgnoreCase(BacklightNamespace.SET_BRIGHTNESS)) {
            			if(args[1] != null || args[1].equals("")) {
            			int brightness = Integer.parseInt(args[1].toString());
            			Backlight.setBrightness(brightness);
            			return "Set the brightness level to " + args[0].toString();
            			}
            			return "Unable to set the brightness level. No brightness level provided.";
            		} else if(args[0].toString().equalsIgnoreCase(BacklightNamespace.SET_TIMEOUT)) {
            			if(args[1] != null || args[1].equals("")) {
            			int timeout = Integer.parseInt(args[1].toString());
            			Backlight.setTimeout(timeout);
            			return "Set the timeout value to " + args[0].toString();
            			}
            			return "Unable to set the timeout value. No timeout value provided.";
            		} else {
            			jsonString = "{ ";
                	for(int i=0; i<args.length; i++) {
                		functionParameter = args[i].toString();
                		if(functionParameter.equalsIgnoreCase(BacklightNamespace.GET_BRIGHTNESS)) {
                			jsonString += getBacklightProperties(BacklightNamespace.GET_BRIGHTNESS);
                			continue;
                		} else if (functionParameter.equals(BacklightNamespace.GET_BRIGHTNESS_DEFAULT)) {
                			jsonString += getBacklightProperties(BacklightNamespace.GET_BRIGHTNESS_DEFAULT);
                			continue;
                		}else if (functionParameter.equals(BacklightNamespace.GET_BRIGHTNESS_INCREMENT)) {
                			jsonString += getBacklightProperties(BacklightNamespace.GET_BRIGHTNESS_INCREMENT);
                			continue;
                		}else if (functionParameter.equals(BacklightNamespace.GET_TIMEOUT_DEFAULT)) {
                			jsonString += getBacklightProperties(BacklightNamespace.GET_TIMEOUT_DEFAULT);
                			continue;
                		}else if (functionParameter.equals(BacklightNamespace.IS_BRIGHTNESS_CONFIGURABLE)) {
                			jsonString += getBacklightProperties(BacklightNamespace.IS_BRIGHTNESS_CONFIGURABLE);
                			continue;
                		}else if (functionParameter.equals(BacklightNamespace.IS_ENABLED)) {
                			jsonString += getBacklightProperties(BacklightNamespace.IS_ENABLED);
                			continue;
                		}else {
                		
                			//did not receive correct function parameter to build json string. send back the
                			//Json string with any errors.
                			jsonString += "\"Unknown backlight parameter received\" : \"" + functionParameter + "\", ";
                			
                		}
                	}
                	int lastCommaIndex = jsonString.length();
                	jsonString = jsonString.substring(0, lastCommaIndex - 2);
                	jsonString += " }";
            		}
            	} catch (Exception ex) {
            		return "{\"Exception\":\"" + ex.getMessage() + "\"}";
            	}
            	
            }
            
            return "Backlight properties returned or modified.";
        }
        public String getBacklightProperties(String props) {
        	BacklightProperties backlightProperties = new BacklightProperties();
        	String jsonString = "";
        	if(props.equalsIgnoreCase(GET_BRIGHTNESS)) {
        		jsonString = "\"getBrightness\" : \"" + backlightProperties.brightness + "\", ";
        		return jsonString;
        	} else if(props.equalsIgnoreCase(GET_BRIGHTNESS_DEFAULT)) {
        		jsonString = "\"getBrightnessDefault\" : \"" + backlightProperties.brightnessDefault + "\", ";
        		return jsonString;
        	} else if(props.equalsIgnoreCase(GET_BRIGHTNESS_INCREMENT)) {
        		jsonString = "\"getBrightnessIncrement\" : \"" + backlightProperties.brightnessIncrement + "\", ";
        		return jsonString;
        	} else if(props.equalsIgnoreCase(GET_TIMEOUT_DEFAULT)) {
        		jsonString = "\"getTimeoutDefault\" : \"" + backlightProperties.timeoutDefault + "\", ";
        		return jsonString;
        	} else if(props.equalsIgnoreCase(IS_BRIGHTNESS_CONFIGURABLE)) {
        		jsonString = "\"isBrightnessConfigurable\" : \"" + backlightProperties.isBrightnessConfigurable + "\", ";
        		return jsonString;
        	} else if(props.equalsIgnoreCase(IS_ENABLED)) {
        		jsonString = "\"isEnabled\" : \"" + backlightProperties.isEnabled + "\", ";
        		return jsonString;
        	} else if(props.equals("all")) {
        		return backlightProperties.objToJsonString();
        	}
        	return "\"PropertyUndefined\":\"" + props + "\"";
        }


    } 
	
}
