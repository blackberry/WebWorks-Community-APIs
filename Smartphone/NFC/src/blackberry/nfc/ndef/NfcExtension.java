package widgetpackage;

import org.w3c.dom.Document;
import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public final class NfcExtension implements WidgetExtension {

    private static final String FEATURES[] = { "blackberry.nfc.ndef" };
    private WidgetNdefMessageListener listener = WidgetNdefMessageListener.getInstance();

    public String[] getFeatureList() {
        return FEATURES;
    }

    public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception {
        if(feature.equals(FEATURES[0])) {
            scriptEngine.addExtension(FEATURES[0], new ScriptableNdef());
        }
    }

    public void register(WidgetConfig config, BrowserField bf) {
    }

    public void unloadFeatures(Document doc) {
    }
}