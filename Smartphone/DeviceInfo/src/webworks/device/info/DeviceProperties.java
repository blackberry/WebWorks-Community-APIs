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

package webworks.device.info;

import net.rim.device.api.system.Backlight;
import net.rim.device.api.system.DeviceInfo;
import net.rim.device.api.system.SystemListener2;
import net.rim.device.api.system.USBPort;

public class DeviceProperties {
	public String canResetIdleTime = "";
	public String getBatteryLevel = "";
	public String getBatteryStatus = "";
	public String getBatteryTemperature = "";
	public String getBatteryVoltage = "";
	public String getDeviceId = "";
	public String getDeviceName = "";
	public String getIdleTime = "";
	public String getLockTimeout = "";
	public String getManufacturerName = "";
	public String getSoftwarePlatformVersion = "";
	public String getSoftwareVersion = "";
	public String getTotalFlashSizeEx = "";
	public String getUSBConnectionState = "";
	public String hasCamera = "";
	public String isBacklightOn = "";
	public String isBatteryRemovable = "";
	public String isPasswordEnabled = "";
	public String isSimulator = "";
	
	public DeviceProperties () {
		canResetIdleTime = (new Boolean(DeviceInfo.canResetIdleTime())).toString();
		getBatteryLevel = (new Integer(DeviceInfo.getBatteryLevel())).toString();
		getBatteryStatus = (new Integer(DeviceInfo.getBatteryStatus())).toString();
		getBatteryTemperature = (new Integer(DeviceInfo.getBatteryTemperature())).toString();
		getBatteryVoltage = (new Integer(DeviceInfo.getBatteryVoltage())).toString();
		getDeviceId = (new Integer(DeviceInfo.getDeviceId())).toString();
		getDeviceName = DeviceInfo.getDeviceName();
		getIdleTime = (new Long (DeviceInfo.getIdleTime())).toString();
		getLockTimeout = (new Integer(DeviceInfo.getLockTimeout())).toString();
		getManufacturerName = DeviceInfo.getManufacturerName();
		getSoftwarePlatformVersion = DeviceInfo.getPlatformVersion();
		getSoftwareVersion = DeviceInfo.getSoftwareVersion();
		getTotalFlashSizeEx = (new Long (DeviceInfo.getTotalFlashSizeEx())).toString();
		getUSBConnectionState = (new Integer(USBPort.getConnectionState())).toString();
		hasCamera = (new Boolean(DeviceInfo.hasCamera())).toString();
		isBacklightOn = (new Boolean(Backlight.isEnabled())).toString();
		isBatteryRemovable = (new Boolean(DeviceInfo.isBatteryRemovable())).toString();
		isPasswordEnabled = (new Boolean (DeviceInfo.isPasswordEnabled())).toString();
		isSimulator = (new Boolean(DeviceInfo.isSimulator())).toString();
	}
	public String objToJsonString() {
		DeviceProperties deviceProperties = new DeviceProperties();
		String json = "";
		json += "{ ";
		json += "\"canResetIdleTime\" : \"" + deviceProperties.canResetIdleTime + "\", ";
		json += "\"getBatteryLevel\" : " + deviceProperties.getBatteryLevel + "\", ";
		json += "\"getBatteryStatus\" : " + deviceProperties.getBatteryStatus + "\", ";
		json += "\"getBatteryTemperature\" : " + deviceProperties.getBatteryTemperature + "\", ";
		json += "\"getBatteryVoltage\" : " + deviceProperties.getBatteryVoltage + "\", ";
		json += "\"getDeviceId\" : " + deviceProperties.getDeviceId + "\", ";
		json += "\"getDeviceName\" : " + deviceProperties.getDeviceName + "\", ";
		json += "\"getIdleTime\" : " + deviceProperties.getIdleTime + "\", ";
		json += "\"getLockTimeout\" : " + deviceProperties.getLockTimeout + "\", ";
		json += "\"getManufacturerName\" : " + deviceProperties.getManufacturerName + "\", ";
		json += "\"getSoftwarePlatformVersion\" : " + deviceProperties.getSoftwarePlatformVersion + "\", ";
		json += "\"getSoftwareVersion\" : " + deviceProperties.getSoftwareVersion + "\", ";
		json += "\"getTotalFlashSizeEx\" : " + deviceProperties.getTotalFlashSizeEx + "\", ";
		json += "\"getUSBConnectionState\" : " + deviceProperties.usbConnectionStatusToString(deviceProperties.getUSBConnectionState) + "\", ";
		json += "\"hasCamera\" : " + deviceProperties.hasCamera + "\", ";
		json += "\"isBacklightOn\" : " + deviceProperties.isBacklightOn + "\", ";
		json += "\"isBatteryRemovable\" : " + deviceProperties.isBatteryRemovable + "\", ";
		json += "\"isPasswordEnabled\" : " + deviceProperties.isPasswordEnabled + "\", ";
		json += "\"isSimulator\" : " + deviceProperties.isSimulator + "\"";
		json += " }";
		return json;
		
	}
	public String usbConnectionStatusToString(String state) {
		String usbConnection = "STATE_UNKNOWN";
		try {
			
			int usbState = -1;
			usbState = Integer.parseInt(state);
			usbConnection = usbState + " : ";
			//need to check for both the SystemListener2 usb constants returned from 
			//USBPort.getConnection state does not return the constants below. 
			//Simulator returns 12 for disconnected and 3 for connected.
			//Device returns 12 for disconnected and 19 for connected.
			if (usbState == SystemListener2.USB_STATE_CABLE_CONNECTED || usbState == 3 || usbState == 19) {
				usbConnection += "CONNECTED";
			} else if (usbState == SystemListener2.USB_STATE_CABLE_DISCONNECTED || usbState == 12) {
				usbConnection += "DISCONNECTED";
			}
		} catch (NumberFormatException ex) {
			usbConnection = "NumberFormatException";

		}
		
		return usbConnection;
	}


}
