package widgetpackage;

import net.rim.device.api.script.Scriptable;

public final class ScriptableNdef extends Scriptable {

    private FunctionRegisterNdefListener _registerFunction;
    private FunctionUnregisterNdefListener _unregisterFunction;
    private FunctionInitLogging _initLoggingFunction;

    // Constructor: called when feature is loaded into memory ("blackberry.nfc.ndef")
    public ScriptableNdef() {
        _registerFunction = new FunctionRegisterNdefListener();
        _unregisterFunction = new FunctionUnregisterNdefListener();
        _initLoggingFunction = new FunctionInitLogging();
    }

    public Object getField(String name) throws Exception {
        if(name.equals(Constants.REGISTER_NDEF)) {
            // will call invoke() method from the FunctionRegisterNdefListener class
            return this._registerFunction;
        }
        if(name.equals(Constants.UNREGISTER_NDEF)) {
            // will call invoke() method from the FunctionUnregisterNdefListener class
            return this._unregisterFunction;
        }
        if(name.equals(Constants.INIT_LOGGING)) {
            // will call invoke() method from the FunctionRegisterNdefListener class
            return this._initLoggingFunction;
        }
        return super.getField(name);
    }

}
