package webworks.mail.attachment;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public final class MailAttachmentExtension implements WidgetExtension {

	private final static String[] _features = { MailAttachmentNamespace.NAME };
	
	public String[] getFeatureList() {
		return _features;
	}

	public void loadFeature(String feature, String version, Document doc,
			ScriptEngine scriptEngine) throws Exception {
		if (feature.equals(MailAttachmentNamespace.NAME)) {
            scriptEngine.addExtension(feature, new MailAttachmentNamespace());
        }

	}

	public void register(WidgetConfig widgetConfig, BrowserField browserField) {
		// TODO Auto-generated method stub

	}

	public void unloadFeatures(Document doc) {
		// TODO Auto-generated method stub

	}

}
