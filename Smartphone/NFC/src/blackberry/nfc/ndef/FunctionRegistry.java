/*
* Copyright 2011 Research In Motion Limited.
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
