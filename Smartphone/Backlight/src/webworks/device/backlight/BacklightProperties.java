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

import net.rim.device.api.system.Backlight;

public class BacklightProperties {
	public String brightness;
	public String brightnessDefault;
	public String brightnessIncrement;
	public String timeoutDefault;
	public String isBrightnessConfigurable;
	public String isEnabled;
	public BacklightProperties() {
		brightness = (new Integer(Backlight.getBrightness())).toString();
		brightnessDefault = (new Integer(Backlight.getBrightnessDefault())).toString();
		brightnessIncrement = (new Integer(Backlight.getBrightnessIncrement())).toString();
		timeoutDefault = (new Integer(Backlight.getTimeoutDefault())).toString();
		isBrightnessConfigurable = (new Boolean(Backlight.isBrightnessConfigurable())).toString();
		isEnabled = (new Boolean(Backlight.isEnabled())).toString();
	}
	public String getBrightness() {
		return brightness;
	}
	public void setBrightness(String brightness) {
		this.brightness = brightness;
	}
	public String getBrightnessDefault() {
		return brightnessDefault;
	}
	public void setBrightnessDefault(String brightnessDefault) {
		this.brightnessDefault = brightnessDefault;
	}
	public String getBrightnessIncrement() {
		return brightnessIncrement;
	}
	public void setBrightnessIncrement(String brightnessIncrement) {
		this.brightnessIncrement = brightnessIncrement;
	}
	public String getTimeoutDefault() {
		return timeoutDefault;
	}
	public void setTimeoutDefault(String timeoutDefault) {
		this.timeoutDefault = timeoutDefault;
	}
	public String getIsBrightnessConfigurable() {
		return isBrightnessConfigurable;
	}
	public void setIsBrightnessConfigurable(String isBrightnessConfigurable) {
		this.isBrightnessConfigurable = isBrightnessConfigurable;
	}
	public String getIsEnabled() {
		return isEnabled;
	}
	public void setIsEnabled(String isEnabled) {
		this.isEnabled = isEnabled;
	}
	public String objToJsonString() {
		BacklightProperties backlightProperties = new BacklightProperties();
		String jsonString = "";
		jsonString += "{ ";
		jsonString += "\"getBrightness\" : \"" + backlightProperties.brightness + "\", ";
		jsonString += "\"getBrightnessDefault\" : \"" + backlightProperties.brightnessDefault + "\", ";
		jsonString += "\"getBrightnessIncrement\" : \"" + backlightProperties.brightnessIncrement + "\", ";
		jsonString += "\"getTimeoutDefault\" : \"" + backlightProperties.timeoutDefault + "\", ";
		jsonString += "\"isBrightnessConfigurable\" : \"" + backlightProperties.isBrightnessConfigurable + "\", ";
		jsonString += "\"isEnabled\" : \"" + backlightProperties.isEnabled + "\" ";
		jsonString += " }";
		return jsonString;
		
	}

}
