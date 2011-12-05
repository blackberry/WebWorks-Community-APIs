/*
* Copyright 2010 Research In Motion Limited.
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
import net.rim.device.api.io.nfc.readerwriter.ReaderWriterManager;
import net.rim.device.api.script.ScriptableFunction;

public final class FunctionUnregisterNdefListener extends ScriptableFunction {

    private FunctionRegistry _registry = FunctionRegistry.getInstance();
    
    public Object invoke(Object obj, Object[] args) throws Exception {
        boolean result = false;
        String record_type = Constants.NDEF_SMART_POSTER_TYPE;
        if(args.length > 0) {
            record_type = (String) args[0];
        }
        try {
            ReaderWriterManager manager = ReaderWriterManager.getInstance();
            manager.removeNDEFMessageListener(NDEFRecord.TNF_WELL_KNOWN, record_type);
            _registry.unregister(record_type);
            result = true;
            Utilities.log("XXXX NDEFMessageListener unregistered");
        } catch(Exception e) {
            Utilities.log("XXXX " + Thread.currentThread().getName() + " FunctionUnregisterNdefListener: "
                    + e.getClass().getName() + ":" + e.getMessage());
        }
        if(result)
            return Boolean.TRUE;
        else
            return Boolean.FALSE;
    }

}