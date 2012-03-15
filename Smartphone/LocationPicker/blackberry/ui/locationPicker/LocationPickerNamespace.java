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

import net.rim.device.api.script.Scriptable;

public final class LocationPickerNamespace extends Scriptable {
	
	public static final String FIELD_INVOKE_LP = LocationPickerFunction.NAME;
	public static final String NAME = "blackberry.ui.locationPicker";
	
	private LocationPickerFunction locationPickerFunc;

	public LocationPickerNamespace() {
		this.locationPickerFunc = new LocationPickerFunction();
	}

	// The getField() function is called when the 
	//  dot '.' extender is used on your JavaScript object.
	public Object getField(String name) throws Exception {
		if (name.equals(FIELD_INVOKE_LP)) {
			return new LocationPickerFunction();
		} 
		return super.getField(name);
	}
	
	public boolean putField(String field, Object value) throws Exception {
        return super.putField(field, value);
    }
}
