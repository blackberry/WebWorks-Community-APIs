package widgetpackage;

import net.rim.device.api.io.nfc.ndef.NDEFRecord;
import net.rim.device.api.io.nfc.readerwriter.ReaderWriterManager;
import net.rim.device.api.script.ScriptableFunction;

public final class FunctionRegisterNdefListener extends ScriptableFunction {

    private FunctionRegistry _registry = FunctionRegistry.getInstance();

    private WidgetNdefMessageListener _listener = WidgetNdefMessageListener.getInstance();

    public FunctionRegisterNdefListener() {
    }

    /*
     * arg[0] is a function to call when we read an NDEF message : mandatory arg[1] is a function to call if an error occurs :
     * mandatory arg[2] is an optional NDEF record type. If absent, we default to "Sp" (Smart Poster)
     */
    public Object invoke(Object obj, Object[] args) throws Exception {
        boolean result = false;
        String record_type = Constants.NDEF_SMART_POSTER_TYPE;
        ScriptableFunction ndef_message_function = null;
        ScriptableFunction on_error_function = null;
        if(args.length < 2) {
            Utilities.log("XXXX " + Thread.currentThread().getName()
                    + " FunctionRegisterNdefListener: insufficient arguments received");
            return Boolean.FALSE;
        }
        if(args[0] instanceof ScriptableFunction) {
            ndef_message_function = (ScriptableFunction) args[0];
        } else {
            Utilities.log("XXXX " + Thread.currentThread().getName()
                    + " FunctionRegisterNdefListener: arg[0] is invalid type of Object");
            return Boolean.FALSE;
        }
        if(args[1] instanceof ScriptableFunction) {
            on_error_function = (ScriptableFunction) args[1];
        } else {
            Utilities.log("XXXX " + Thread.currentThread().getName()
                    + " FunctionRegisterNdefListener: arg[1] is invalid type of Object");
            return Boolean.FALSE;
        }
        if(args.length > 2) {
            if(args[2] instanceof String) {
                record_type = (String) args[2];
            } else {
                Utilities.log("XXXX " + Thread.currentThread().getName()
                        + " FunctionRegisterNdefListener: arg[2] is invalid type of Object");
                return Boolean.FALSE;
            }
        }
        try {
            _registry.register(record_type, ndef_message_function, on_error_function);
        } catch(UnsupportedNdefTypeException e) {
            Utilities.log("XXXX " + Thread.currentThread().getName()
                    + " FunctionRegisterNdefListener: value of arg[2] ("+record_type+") is of unsupported NDEF record type");
            return Boolean.FALSE;
        }
        try {
            ReaderWriterManager manager = ReaderWriterManager.getInstance();
            manager.addNDEFMessageListener(_listener, NDEFRecord.TNF_WELL_KNOWN, record_type, true);
            result = true;
            Utilities.log("XXXX " + Thread.currentThread().getName()
                    + " FunctionRegisterNdefListener: NDEFMessageListener registered");
        } catch(Exception e) {
            Utilities.log("XXXX " + Thread.currentThread().getName() + " FunctionRegisterNdefListener: " + e.getClass().getName()
                    + ":" + e.getMessage());
        }
        if(result)
            return Boolean.TRUE;
        else
            return Boolean.FALSE;
    }

}