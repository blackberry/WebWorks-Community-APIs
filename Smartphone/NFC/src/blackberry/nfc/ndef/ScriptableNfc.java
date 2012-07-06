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

import net.rim.device.api.script.Scriptable;

public class ScriptableNfc extends Scriptable {
    private FunctionIsNfcAvailable _isNFCAvailableFunction;
    private FunctionIsNfcSupported _isNFCSupportedFunction;
    private ScriptableNdef _ndef;

    public ScriptableNfc() {
        _isNFCSupportedFunction = new FunctionIsNfcSupported();
        _isNFCAvailableFunction = new FunctionIsNfcAvailable();
        _ndef = new ScriptableNdef();
    }

    public Object getField(String name) throws Exception {
        if(Constants.IS_NFC_AVAILABLE.equals(name)) {
            return _isNFCAvailableFunction;
        }
        if(Constants.IS_NFC_SUPPORTED.equals(name)) {
            return _isNFCSupportedFunction;
        }
        if(Constants.NDEF.equals(name)) {
            return _ndef;
        }
        return super.getField(name);
    }

}
