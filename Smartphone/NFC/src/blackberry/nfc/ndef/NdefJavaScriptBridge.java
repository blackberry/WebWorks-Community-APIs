/*
 * Copyright 2011-2012 Research In Motion Limited.
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

import net.rim.device.api.io.nfc.ndef.NDEFRecord;
import net.rim.device.api.script.ScriptableFunction;

public class NdefJavaScriptBridge {

    private static NdefJavaScriptBridge _jsBridge = new NdefJavaScriptBridge();
    private FunctionRegistry _registry = FunctionRegistry.getInstance();
    
    private NdefJavaScriptBridge() {
    }
    
    public static NdefJavaScriptBridge getInstance() {
        return _jsBridge;
    }

    public void useNDEFMessage(String record_type, String json_ndef_message) {
        useNDEFMessage(NDEFRecord.TNF_WELL_KNOWN,record_type,json_ndef_message);
    }
    
    public void useNDEFMessage(int typeNameFormat, String record_type, String json_ndef_message) {
        log("Delivering to JavaScript",typeNameFormat,record_type,json_ndef_message);
        // send JSON object to JavaScript function provided as the call back function parameter to the register_ndef function
        ScriptableFunction ndef_callback = _registry.getNdefFunction(typeNameFormat,record_type);
        if (ndef_callback != null) {
            try {
                Object[] args = new Object[] { json_ndef_message };
                ndef_callback.invoke(null, args);
            } catch (Exception e) {
                log("Could not dispatch message",typeNameFormat,record_type,json_ndef_message,e);
            }
        } else {
            log("No listener registered for message", typeNameFormat,
                    record_type, json_ndef_message);
        }
    }
    
    public void reportError(String record_type, String error_message) {
        reportError(NDEFRecord.TNF_WELL_KNOWN, record_type, error_message);
    }
    
    public void reportError(int typeNameFormat, String record_type, String error_message) {
        log("Reporting error",typeNameFormat,record_type,error_message);
        ScriptableFunction on_error_function = _registry.getErrorFunction(record_type);
        if (on_error_function != null) {
            try {
                Object [] args = new Object[] { error_message };
                on_error_function.invoke(null, args);
            } catch(Exception e) {
                log("Could not dispatch error message",typeNameFormat, record_type, error_message,e);
            }
        } else {
            log("No error handler registered",typeNameFormat,record_type,error_message);
        }
    }
    
    private static void log(String message, int typeNameFormat, String recordType, String extraMessage) {
        log(message,typeNameFormat,recordType,extraMessage,null);
    } 
    private static void log(String message, int typeNameFormat, String recordType, String extraMessage, Exception e) {
        Utilities.log(NdefJavaScriptBridge.class.getName() + ": " + message + ": " + typeNameFormat +":" + recordType + " :" + extraMessage,e);
    }
}
