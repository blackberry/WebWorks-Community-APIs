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
