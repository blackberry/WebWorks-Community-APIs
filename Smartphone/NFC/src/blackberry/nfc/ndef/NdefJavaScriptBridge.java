package widgetpackage;

import net.rim.device.api.script.ScriptableFunction;

public class NdefJavaScriptBridge {

    private static NdefJavaScriptBridge _jsBridge;
    private FunctionRegistry _registry = FunctionRegistry.getInstance();
    
    private NdefJavaScriptBridge() {
        
    }
    
    public static synchronized NdefJavaScriptBridge getInstance() {
        if(_jsBridge == null) {
            _jsBridge = new NdefJavaScriptBridge();
        }
        return _jsBridge;
    }

    public void useNDEFMessage(String record_type, String json_ndef_message) {
        Utilities.log("XXXX delivering to JavaScript:" + json_ndef_message);
        // send JSON object to JavaScript function provided as the call back function parameter to the register_ndef function
        Object [] args = new Object[1];
        args[0] = json_ndef_message;
        try {
            ScriptableFunction ndef_callback = _registry.getNdefFunction(record_type);
            ndef_callback.invoke(null, args);
        } catch(Exception e) {
            Utilities.log("XXXX " + e.getClass().getName() + ":" + e.getMessage());
        }
    }
    
    public void reportError(String record_type, String error_message) {
        ScriptableFunction on_error_function = _registry.getErrorFunction(record_type);
        if (on_error_function != null) {
            Utilities.log("XXXX reporting error for record type" + record_type+":"+error_message);
            Object [] args = new Object[1];
            args[0] = error_message;
            try {
                on_error_function.invoke(null, args);
            } catch(Exception e) {
                Utilities.log("XXXX " + e.getClass().getName() + ":" + e.getMessage());
            }
        } else {
            Utilities.log("XXXX could not report error for record type" + record_type+":"+error_message);
        }
    }
}
