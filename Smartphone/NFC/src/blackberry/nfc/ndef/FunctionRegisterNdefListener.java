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

import org.json.me.JSONException;
import org.json.me.JSONObject;

import net.rim.device.api.io.nfc.ndef.NDEFRecord;
import net.rim.device.api.io.nfc.readerwriter.ReaderWriterManager;
import net.rim.device.api.script.ScriptableFunction;

public final class FunctionRegisterNdefListener extends ScriptableFunction {

    private FunctionRegistry _registry = FunctionRegistry.getInstance();

    private WidgetNdefMessageListener _listener = WidgetNdefMessageListener
            .getInstance();

    public FunctionRegisterNdefListener() {
    }

    /**
     * <p>
     * arg[0] is a function to call when we read an NDEF message : mandatory
     * </p>
     * <p>
     * arg[1] is a function to call if an error occurs : mandatory
     * </p>
     * <p>
     * arg[2] is a String, which is either an NDEF type name or a JSON object
     * which contains the following values:
     * <ul>
     * <li><code>type</code> - the NDEF type. Defaults to <code>Sp</code>.</li>
     * <li><code>typeNameFormat</code> - the NDEF type name format. Defaults to
     * {@link NDEFRecord#TNF_WELL_KNOWN} (1).</li>
     * </ul>
     * </p>
     */
    public Object invoke(Object obj, Object[] args) {
        boolean result = false;
        int typeNameFormat = NDEFRecord.TNF_WELL_KNOWN;
        String record_type = Constants.NDEF_SMART_POSTER_URI_TYPE;
        ScriptableFunction ndef_message_function = null;
        ScriptableFunction on_error_function = null;
        if (args.length < 2) {
            log("insufficient arguments received");
            return Boolean.FALSE;
        }
        if (args[0] instanceof ScriptableFunction) {
            ndef_message_function = (ScriptableFunction) args[0];
        } else {
            log("arg[0] is invalid type of Object: " + cl(args[0]));
            return Boolean.FALSE;
        }
        if (args[1] instanceof ScriptableFunction) {
            on_error_function = (ScriptableFunction) args[1];
        } else {
            log("arg[1] is invalid type of Object: " + cl(args[1]));
            return Boolean.FALSE;
        }
        if (args.length > 2) {
            if (args[2] instanceof String) {
                if (((String) args[2]).indexOf("{") == -1) {
                    record_type = (String) args[2];
                } else {
                    try {
                        JSONObject params = new JSONObject((String) args[2]);
                        if (params.has(Constants.NDEF_TYPE)) {
                            record_type = params.getString(Constants.NDEF_TYPE);
                        } else {
                            log("Defaulting to Sp (no type in JSONObject)");
                        }
                        if (params.has(Constants.NDEF_TYPE_NAME_FORMAT)) {
                            typeNameFormat = params
                                    .getInt(Constants.NDEF_TYPE_NAME_FORMAT);
                        }
                    } catch (JSONException je) {
                        log("Could not decode param", je);
                        return Boolean.FALSE;
                    }
                }
            } else {
                log("args[2] is invalid type of Object: " + cl(args[2]));
                return Boolean.FALSE;
            }
        }
        try {
            _registry.register(typeNameFormat, record_type,
                    ndef_message_function, on_error_function);
        } catch (UnsupportedNdefTypeException e) {
            log("value of arg[2] (" + record_type
                    + ") is of unsupported NDEF record type");
            return Boolean.FALSE;
        }
        try {
            log("last try");
            ReaderWriterManager manager = null;
            try {
                manager = ReaderWriterManager.getInstance();
            } catch (Exception e) {
                log("Failed in call to getInstance()");
                return Boolean.FALSE;
            }
            log("manager: " + manager);
            log("listener: " + _listener);
            log("typeNameFormat: " + typeNameFormat);
            log("record type: " + record_type);
            manager.addNDEFMessageListener(_listener, typeNameFormat,
                    record_type, true);
            result = true;
            log("NDEFMessageListener registered " + typeNameFormat + ":"
                    + record_type);
        } catch (Exception e) {
            log("Failed registration", e);
            return Boolean.FALSE;
        }
        if (result) {
            return Boolean.TRUE;
        } else {
            return Boolean.FALSE;
        }
    }

    private String cl(Object o) {
        log("Getting class of " + o.toString());
        return o == null ? "null" : o.getClass().getName();
    }

    private void log(String string) {
        log(string, null);
    }

    private void log(String string, Exception e) {
        Utilities.log(getClass().getName() + ": " + string, e);
    }

}
