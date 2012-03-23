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

package webworks.media.barcode;


import net.rim.device.api.script.Scriptable;

public class BarcodeNamespace extends Scriptable {

    public static final String FEATURE_NAME = "webworks.media.barcode";

    private static class BarcodeNamespaceHolder{
        private static final BarcodeNamespace INSTANCE = new BarcodeNamespace();
    }

    public static final BarcodeNamespace getInstance() {
        return BarcodeNamespaceHolder.INSTANCE;
    }

    private BarcodeNamespace(){

    }


    public Object getField(final String name) throws Exception {
        //Functions
        if (name.equals(ScanBarcodeAction.NAME)) {
            return new ScanBarcodeAction();
        } else if (name.equals(GenerateBarcodeAction.NAME)){
        	return new GenerateBarcodeAction();
        }

        return super.getField(name);
    }


}
