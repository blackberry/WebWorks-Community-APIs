package webworks.analytics;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public class AnalyticsExtension implements WidgetExtension {

	private String FEATURE_SYSTEM_DISPLAY = "webworks.analytics";

	public String[] getFeatureList() {
		String[] result = new String[] { FEATURE_SYSTEM_DISPLAY };
		return result;
	}

	public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception {
		if (feature.equals(FEATURE_SYSTEM_DISPLAY)) {
			scriptEngine.addExtension(FEATURE_SYSTEM_DISPLAY, new AnalyticsNamespace());
		}
	}

	public void register(WidgetConfig config, BrowserField bf) {
		// TODO Auto-generated method stub
	}

	public void unloadFeatures(Document doc) {
		// TODO Auto-generated method stub
	}
}
