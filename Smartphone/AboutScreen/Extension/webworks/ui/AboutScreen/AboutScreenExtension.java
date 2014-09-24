package webworks.ui.AboutScreen;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

public final class AboutScreenExtension implements WidgetExtension
{
	private final static String FEATURE_ID = "webworks.ui.AboutScreen"; 

	public String[] getFeatureList()
	{
	    return new String[] {FEATURE_ID};
	}

	public void loadFeature(String feature, String version, Document doc, ScriptEngine scriptEngine) throws Exception
	{
		if (feature.equals(FEATURE_ID)) 
	    {
	        scriptEngine.addExtension(FEATURE_ID, new AboutScreenNamespace());
	    }
	}

	public void register(WidgetConfig widgetConfig, BrowserField browserField) {}

	public void unloadFeatures(Document doc) {}
}