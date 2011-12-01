package widgetpackage;

import net.rim.device.api.script.ScriptableFunction;

public final class FunctionInitLogging extends ScriptableFunction {

    public Object invoke(Object obj, Object[] args) throws Exception {
        String app_name = "NfcExtension";
        if(args.length > 0) {
            app_name = (String) args[0];
        }
        long log_id = Utilities.generateId(app_name);
        Utilities.initLogging(log_id, app_name);
        return Boolean.TRUE;
    }

}