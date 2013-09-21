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

import org.json.me.JSONObject;

import net.rim.device.api.io.nfc.ndef.NDEFRecord;
import net.rim.device.api.io.nfc.readerwriter.ReaderWriterManager;
import net.rim.device.api.script.ScriptableFunction;

public final class FunctionUnregisterNdefListener extends ScriptableFunction {

    private FunctionRegistry _registry = FunctionRegistry.getInstance();
    
    public Object invoke(Object obj, Object[] args) throws Exception {
        boolean result = false;
        String record_type = Constants.NDEF_SMART_POSTER_URI_TYPE;
        int typeNameFormat = NDEFRecord.TNF_WELL_KNOWN;
        
        if(args.length > 0) {
            if (args[0] instanceof String) {
                if (((String)args[0]).indexOf("{")==-1) {
                    record_type = (String) args[0];
                } else {
                    JSONObject params = new JSONObject((String)args[0]);
                    if (params.has(Constants.NDEF_TYPE)) {
                        record_type = params.getString(Constants.NDEF_TYPE);
                    } else {
                        log("Defaulting to Sp (no type in JSON Object)");
                    }
                    if (params.has(Constants.NDEF_TYPE_NAME_FORMAT)) {
                        typeNameFormat = params.getInt(Constants.NDEF_TYPE_NAME_FORMAT);
                    }
                }
            } else {
                log("args[0] is an invalid type of Object");
                return Boolean.FALSE;
            }
        }
        try {
            ReaderWriterManager manager = ReaderWriterManager.getInstance();
            log("Unregister called for: " + typeNameFormat + " " + record_type);
            manager.removeNDEFMessageListener(typeNameFormat, record_type);
            _registry.unregister(typeNameFormat,record_type);
            result = true;
            log("NDEFMessageListener unregistered");
        } catch(Exception e) {
            log("Error trying to unregister",e);
            return Boolean.FALSE;
        }
        if(result)
            return Boolean.TRUE;
        else
            return Boolean.FALSE;
    }
    
    private void log(String message) {
        log(message,null);
    }

    private void log(String message, Exception e) {
        Utilities.log(FunctionUnregisterNdefListener.class.getName() + ": " + message,e);
    }

}
