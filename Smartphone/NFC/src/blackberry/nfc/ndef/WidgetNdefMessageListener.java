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

import net.rim.device.api.io.nfc.ndef.NDEFMessage;
import net.rim.device.api.io.nfc.ndef.NDEFMessageListener;

public class WidgetNdefMessageListener implements NDEFMessageListener {
    
    private static WidgetNdefMessageListener listener = new WidgetNdefMessageListener();
    
    private WidgetNdefMessageListener() {
    }
    
    public static WidgetNdefMessageListener getInstance() {
        return listener;
    }

    public void onNDEFMessageDetected(NDEFMessage message) {
        Utilities.log(WidgetNdefMessageListener.class.getName() + ": Received NDEF message");
        NdefMessageParser.getInstance(message).parseAndDeliver(message);
        Utilities.log(WidgetNdefMessageListener.class.getName() + ": Sent NDEF message to parser for processing....");
    }

}
