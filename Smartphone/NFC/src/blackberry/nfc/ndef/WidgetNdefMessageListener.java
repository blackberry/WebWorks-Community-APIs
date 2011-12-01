package widgetpackage;

import net.rim.device.api.io.nfc.ndef.NDEFMessage;
import net.rim.device.api.io.nfc.ndef.NDEFMessageListener;

public class WidgetNdefMessageListener implements NDEFMessageListener {
    
    private static WidgetNdefMessageListener listener;
    
    private WidgetNdefMessageListener() {
        
    }
    
    public static synchronized WidgetNdefMessageListener getInstance() {
        if (listener == null) {
            listener = new WidgetNdefMessageListener();
        }
        return listener;
    }

    public void onNDEFMessageDetected(NDEFMessage message) {
        Utilities.log("XXXX received NDEF message");
        NdefMessageParser.getInstance(message).parseAndDeliver(message);
        Utilities.log("XXXX sent NDEF message to parser for processing....");
    }

}
