package webworks.io;

import org.w3c.dom.Document;

import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

/**
 * 
 * @author Jerome Carty
 *
 */
public class FileUploaderExtension implements WidgetExtension {

	public String[] getFeatureList() {
		return new String[] { FileUploaderNamespace.NAME };
	}

	public void loadFeature(String feature, String version, Document doc,
			ScriptEngine scriptEngine) throws Exception {
		
		if (feature.equals(FileUploaderNamespace.NAME)) {
			Logger.enableLogging(0xe995515474f898fbL, "webworks.io.FileUploader");
			scriptEngine.addExtension(feature, new FileUploaderNamespace());
		}		
	}

	public void register(WidgetConfig arg0, BrowserField arg1) {
	}

	public void unloadFeatures(Document arg0) {
	}

}
