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

import net.rim.device.api.script.ScriptableFunction;

public final class FunctionInitLogging extends ScriptableFunction {

    public Object invoke(Object obj, Object[] args) throws Exception {
        String app_name = "NfcExtension";
        if (args.length > 0) {
            app_name = (String) args[0];
        }
        long log_id = Utilities.generateId(app_name);
        Utilities.initLogging(log_id, app_name);
        return Boolean.TRUE;
    }

}
