package widgetpackage;

import java.util.Hashtable;

import net.rim.device.api.script.ScriptableFunction;

public class FunctionRegistry {
    
    private static FunctionRegistry registry;

    // record type is key
    private Hashtable ndef_functions = new Hashtable();
    private Hashtable error_functions = new Hashtable();
    
    private FunctionRegistry() {
        
    }
    
    public static synchronized FunctionRegistry getInstance() {
        if (registry == null) {
            registry = new FunctionRegistry();
        }
        return registry;
    }
    
    public void register(String record_type, ScriptableFunction ndef_function, ScriptableFunction error_function) throws UnsupportedNdefTypeException {
        if (isSupportedType(record_type)) {
            ndef_functions.put(record_type, ndef_function);
            error_functions.put(record_type, error_function);
        } else {
            throw new UnsupportedNdefTypeException(record_type);
        }
    }
    
    public void unregister(String record_type) {
        ndef_functions.remove(record_type);
        error_functions.remove(record_type);
    }
    
    public boolean isSupportedType(String record_type) {
        if (record_type.equals(Constants.NDEF_SMART_POSTER_TYPE)) {
            return true;
        } else {
            return false;
        }
    }
    
    public ScriptableFunction getNdefFunction(String record_type) {
        return (ScriptableFunction) ndef_functions.get(record_type);
    }

    public ScriptableFunction getErrorFunction(String record_type) {
        return (ScriptableFunction) error_functions.get(record_type);
    }

    
}
