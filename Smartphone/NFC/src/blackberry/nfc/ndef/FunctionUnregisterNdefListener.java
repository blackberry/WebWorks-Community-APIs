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