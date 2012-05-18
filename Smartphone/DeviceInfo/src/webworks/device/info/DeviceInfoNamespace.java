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

package webworks.device.info;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.DeviceInfo;
import net.rim.device.api.system.USBPort;
import net.rim.device.api.system.SystemListener2;

public class DeviceInfoNamespace extends Scriptable
{
    
    public static final String CAN_RESET_IDLE_TIME = "canResetIdleTime";
	public static final String GET_BATTERY_LEVEL = "getBatteryLevel";
	public static final String GET_BATTERY_STATUS = "getBatteryStatus";
	public static final String GET_BATTERY_TEMPERATURE = "getBatteryTemp";
	public static final String GET_DEVICE_ID = "getDeviceId";
	public static final String GET_DEVICE_INFO = "getDeviceInfo";
	public static final String GET_DEVICE_NAME = "getDeviceName";
	public static final String GET_IDLE_TIME = "getIdleTime";
	public static final String GET_LOCKOUT_TIMEOUT = "getLockoutTimeout";
	public static final String GET_MANUFACTURER_NAME = "getManufacturerName";
	public static final String GET_PLATFORM_VERSION = "getPlatformVersion";
	public static final String GET_SOFTWARE_VERSION = "getSoftwareVersion";
	public static final String GET_TOTAL_FLASH_SIZE = "getTotalFlashSize";
	public static final String GET_USB_CONNECTION_STATE = "getUSBConnectionState";
	public static final String HAS_CAMERA = "hasCamera";
	public static final String IS_BACKLIGHT_ON = "isBacklightOn";
	public static final String IS_BATTERY_REMOVABLE = "isBatteryRemovable";
	public static final String IS_PASSWORD_ENABLED = "isPasswordEnabled";
	public static final String IS_SIMULATOR = "isSimulator";

	
	
    public Object getField(String name) throws Exception
    {
        if (name.equals(GET_DEVICE_INFO))
        {
            return new DeviceInfoProperties();
        } 
        return super.getField(name);
    }

    public class DeviceInfoProperties extends ScriptableFunction
    {
       
        public Object invoke(Object obj, Object[] args) throws Exception
        {
        	/* Read in all of the device property arguments. Default behavior is to return all values
        	*  unless specific properties are included as arguments of the getDeviceInfo() Javascript function
        	*  e.g. allProperties =  getDeviceInfo() returns a JSON String of all of the DeviceInfo properties
        	*  You may also want to get only specific values from this function call so execute the following
        	*  getDeviceInfo("getBatteryLevel", "hasCamera", "getSoftwareVersion");
        	*/
        	String jsonString = "";
        	String functionParameter = "";
        	DeviceProperties deviceProperties = new DeviceProperties();
        	if(args.length == 0) {
        		//getting all device properties because none were specified in the javascript function call
        		jsonString = deviceProperties.objToJsonString();
        	} else {
        		jsonString = "{ ";
            	for(int i=0; i<args.length; i++) {
            		functionParameter = args[i].toString();
            		if(functionParameter.equals(DeviceInfoNamespace.CAN_RESET_IDLE_TIME)) {
            			jsonString += "\"canResetIdleTime\" : \"" + deviceProperties.canResetIdleTime + "\", ";
            			continue;
            		} else if (functionParameter.equals(DeviceInfoNamespace.GET_BATTERY_LEVEL)) {
            			jsonString += "\"getBatteryLevel\" : " + deviceProperties.getBatteryLevel + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_BATTERY_STATUS)) {
            			jsonString += "\"getBatteryStatus\" : \"" + deviceProperties.getBatteryStatus + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_BATTERY_TEMPERATURE)) {
            			jsonString += "\"getBatteryTemperature\" : \"" + deviceProperties.getBatteryTemperature + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_DEVICE_ID)) {
            			jsonString += "\"getBatteryVoltage\" : \"" + deviceProperties.getBatteryVoltage + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_DEVICE_INFO)) {
            			jsonString += "\"getDeviceId\" : \"" + deviceProperties.getDeviceId + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_DEVICE_NAME)) {
            			jsonString += "\"getDeviceName\" : \"" + deviceProperties.getDeviceName + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_IDLE_TIME)) {
            			jsonString += "\"getIdleTime\" : \"" + deviceProperties.getIdleTime + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_LOCKOUT_TIMEOUT)) {
            			jsonString += "\"getLockTimeout\" : \"" + deviceProperties.getLockTimeout + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_MANUFACTURER_NAME)) {
            			jsonString += "\"getManufacturerName\" : \"" + deviceProperties.getManufacturerName + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_PLATFORM_VERSION)) {
            			jsonString += "\"getSoftwarePlatformVersion\" : \"" + deviceProperties.getSoftwarePlatformVersion + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_SOFTWARE_VERSION)) {
            			jsonString += "\"getSoftwareVersion\" : \"" + deviceProperties.getSoftwareVersion + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_TOTAL_FLASH_SIZE)) {
            			jsonString += "\"getTotalFlashSizeEx\" : \"" + deviceProperties.getTotalFlashSizeEx + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.GET_USB_CONNECTION_STATE)) {
            			String usbConnection = "STATE_UNKNOWN";
            			String state = "";
            			
            			state = deviceProperties.getUSBConnectionState;
            			int usbState = -1;
            			usbState = Integer.parseInt(state);
            			//need to check for both the SystemListener2 usb constants returned from 
            			//USBPort.getConnection state does not return the constants below. 
            			//Simulator returns 12 for disconnected and 3 for connected.
            			//Device returns 12 for disconnected and 19 for connected.
            			if (usbState == SystemListener2.USB_STATE_CABLE_CONNECTED || usbState == 3 || usbState == 19) {
            				usbConnection = "CONNECTED";
            			} else if (usbState == SystemListener2.USB_STATE_CABLE_DISCONNECTED || usbState == 12) {
            				usbConnection = "DISCONNECTED";
            			}
            			jsonString += "\"getUSBConnectionState\" : \"" + usbConnection + "\":" + usbState + ", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.HAS_CAMERA)) {
            			jsonString += "\"hasCamera\" : \"" + deviceProperties.hasCamera + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.IS_BATTERY_REMOVABLE)) {
            			jsonString += "\"isBatteryRemovable\" : \"" + deviceProperties.isBatteryRemovable + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.IS_BACKLIGHT_ON)) {
            			jsonString += "\"isBacklightOn\" : \"" + deviceProperties.isBacklightOn + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.IS_PASSWORD_ENABLED)) {
            			jsonString += "\"isPasswordEnabled\" : \"" + deviceProperties.isPasswordEnabled + "\", ";
            			continue;
            		}else if (functionParameter.equals(DeviceInfoNamespace.IS_SIMULATOR)) {
            			jsonString += "\"isSimulator\" : \"" + deviceProperties.isSimulator + "\", ";
            			continue;
            		}else {
            		
            			//did not receive correct function parameter to build json string. send back the
            			//Json string with any errors.
            			jsonString += "\"Unknown device parameter received\" : \"" + functionParameter + "\", ";
            			
            		}
            	}
            	int lastCommaIndex = jsonString.length();
            	jsonString = jsonString.substring(0, lastCommaIndex - 2);
            	jsonString += " }";
        	}

        	return jsonString;
        }
    }

}