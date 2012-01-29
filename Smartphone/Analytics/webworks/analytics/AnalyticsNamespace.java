package webworks.analytics;

import java.util.Hashtable;
import java.util.TimeZone;

import com.webtrends.mobile.analytics.rim.WebtrendsConfig;
import com.webtrends.mobile.analytics.rim.WebtrendsConfigurator;
import com.webtrends.mobile.analytics.rim.WebtrendsDataCollector;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.ApplicationDescriptor;
import net.rim.device.api.system.CodeModuleManager;

public class AnalyticsNamespace extends Scriptable {

	public static final String FIELD_REGISTER = "register";
	public static final String FIELD_ADCLICK = "adclick";
	public static final String FIELD_ADIMPRESSION = "adimpression";
	public static final String FIELD_BUTTON = "button";
	public static final String FIELD_CONVERSION = "conversion";
	public static final String FIELD_CUSTOM = "custom";
	public static final String FIELD_MEDIA = "media";
	public static final String FIELD_PRODUCT = "product";
	public static final String FIELD_SCREEN = "screen";
	public static final String FIELD_SEARCH = "search";
	public static final String FIELD_TERMINATE = "terminate";
	public static final String FIELD_ERROR = "error";
	public static final String FIELD_FOREGROUND = "foreground";
	public static final String FIELD_BACKGROUND = "background";
	public static final String FIELD_START = "start";

	private AnalyticsConfig config;

	public Object getField(String name) throws Exception {
		if (name.equals(FIELD_REGISTER)) {
			return new RegisterAnalytics();
		} else if (name.equals(FIELD_ADCLICK)) {
			return new AdClick();
		} else if (name.equals(FIELD_ADIMPRESSION)) {
			return new AdImpression();
		} else if (name.equals(FIELD_BUTTON)) {
			return new ButtonClick();
		} else if (name.equals(FIELD_CONVERSION)) {
			return new Conversion();
		} else if (name.equals(FIELD_CUSTOM)) {
			return new CustomEvent();
		} else if (name.equals(FIELD_MEDIA)) {
			return new MediaEvent();
		} else if (name.equals(FIELD_PRODUCT)) {
			return new ProductEvent();
		} else if (name.equals(FIELD_SCREEN)) {
			return new ScreenView();
		} else if (name.equals(FIELD_SEARCH)) {
			return new Search();
		} else if (name.equals(FIELD_TERMINATE)) {
			return new Terminate();
		} else if (name.equals(FIELD_ERROR)) {
			return new Error();
		}
		return super.getField(name);
	}

	/**
	 * Configuration of the BlackBerry Analytics settings. Takes:
	 * 
	 * <pre>
	 * String dcsid
	 * String timezone offset
	 * String app category
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	private class AnalyticsConfig extends WebtrendsConfig {

		private String dcsid;
		private String category;

		public AnalyticsConfig(String dcsid, String category) {
			this.dcsid = dcsid;
			this.category = category;
		}

		public String wt_dc_app_name() {
			return ApplicationDescriptor.currentApplicationDescriptor().getName();
		}

		public String wt_dc_app_version() {
			return ApplicationDescriptor.currentApplicationDescriptor().getVersion();
		}

		public String wt_dc_dcsid() {
			return dcsid;
		}

		public String wt_dc_timezone() {
			return Integer.toString(TimeZone.getDefault().getRawOffset());
		}

		public String wt_dc_app_category() {
			return category;
		}

		public String wt_dc_app_publisher() {
			return CodeModuleManager.getModuleVendor(ApplicationDescriptor.currentApplicationDescriptor().getModuleHandle());
		}
	}

	/**
	 * Setup the WebtrendsConfig object and initialize BlackBerry Analytics.
	 * Takes in order: <br/>
	 * 
	 * <pre>
	 * String dcsid
	 * String app category
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class RegisterAnalytics extends ScriptableFunction {
		private static final String NAME = FIELD_REGISTER;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 2) {
				try {
					final String dcsid = (args[0].toString());
					final String appCategory = args[1].toString();
					config = new AnalyticsConfig(dcsid, appCategory);
					WebtrendsConfigurator.LoadConfigFile(config);
					WebtrendsDataCollector wtDC = WebtrendsDataCollector.getInstance();
					wtDC.Initialize();
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record an Ad Click Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * String Ad Name
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class AdClick extends ScriptableFunction {
		private static final String NAME = FIELD_ADCLICK;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 4) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					final String name = args[3].toString();
					WebtrendsDataCollector.getInstance().onAdClickEvent(page, desc, type, null, name);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record an Ad Impression Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * String Ad Name1
	 * [String Ad Name2] Optional continuation
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class AdImpression extends ScriptableFunction {
		private static final String NAME = FIELD_ADIMPRESSION;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length >= 4) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					final String[] names = getStringList(3, args);
					WebtrendsDataCollector.getInstance().onAdImpressionEvent(page, desc, type, null, names);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}

		/**
		 * Return an array of Strings from the argument list starting with the
		 * item at startIndex, through to the end.
		 * 
		 * @param startIndex
		 * @param args
		 * @return
		 * @throws Exception
		 */
		private String[] getStringList(int startIndex, Object[] args) throws Exception {
			int argsLength = args.length - startIndex;
			String[] results = new String[argsLength];
			for (int i = 0; i < argsLength; i++) {
				results[i] = args[startIndex + i].toString();
			}
			return results;

		}
	}

	/**
	 * Record a Button Click Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class ButtonClick extends ScriptableFunction {
		private static final String NAME = FIELD_BUTTON;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 3) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					WebtrendsDataCollector.getInstance().onButtonClick(page, desc, type, null);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record a Conversion Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * String Content Category - used to group different events together along a similar theme.
	 * String Conversion Name - name to identify this conversion type, such as 'full version'.
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class Conversion extends ScriptableFunction {
		private static final String NAME = FIELD_CONVERSION;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 5) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					final String category = args[3].toString();
					final String name = args[4].toString();
					WebtrendsDataCollector.getInstance().onConversionEvent(page, desc, type, null, category, name);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record a Custom Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String Event Type - Custom identifier for the event.
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class CustomEvent extends ScriptableFunction {
		private static final String NAME = FIELD_CUSTOM;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 3) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = (args[2].toString());
					Hashtable customParams = new Hashtable();
					customParams.put("WT.ev", type);
					WebtrendsDataCollector.getInstance().onCustomEvent(page, desc, customParams);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record a Media Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * String Content Category - used to group different events together along a similar theme
	 * String Media Name
	 * String Media Type
	 * String Media Event Type - The status of the media event. Valid values include 'p' (play), '25' (25% complete), '50' (50% complete), '75' (75% complete), and 'f' (finished).
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class MediaEvent extends ScriptableFunction {
		private static final String NAME = FIELD_MEDIA;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 7) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					final String category = args[3].toString();
					final String name = args[4].toString();
					final String mediaType = args[5].toString();
					final String mediaEvent = args[6].toString();
					WebtrendsDataCollector.getInstance().onMediaEvent(page, desc, type, null, category, name, mediaType, mediaEvent);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record a Product View Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * String Content Category - used to group different events together along a similar theme
	 * String Product ID
	 * String Product SKU
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class ProductEvent extends ScriptableFunction {
		private static final String NAME = FIELD_PRODUCT;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 6) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					final String category = args[3].toString();
					final String id = args[4].toString();
					final String sku = args[5].toString();
					Hashtable customParams = new Hashtable();
					customParams.put("WT.tx_e", "v");
					customParams.put("WT.tx_u", "1");
					WebtrendsDataCollector.getInstance().onProductView(page, desc, type, customParams, category, id, sku);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record a Screen View Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * String Content Category - used to group different events together along a similar theme.
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class ScreenView extends ScriptableFunction {
		private static final String NAME = FIELD_SCREEN;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 4) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					final String category = args[3].toString();
					WebtrendsDataCollector.getInstance().onScreenView(page, desc, type, null, category);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record a Search Event. Takes in order: <br/>
	 * 
	 * <pre>
	 * String page - The hierarchical location of where an object or content is located in an application.
	 * String description - The name of the screen where the event occurs.
	 * String type - a custom parameter you can use to identify the event.
	 * String Search Query
	 * String Number of Results
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class Search extends ScriptableFunction {
		private static final String NAME = FIELD_SEARCH;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 5) {
				try {
					final String page = (args[0].toString());
					final String desc = args[1].toString();
					final String type = args[2].toString();
					final String query = args[3].toString();
					final String results = args[4].toString();
					WebtrendsDataCollector.getInstance().onSearchEvent(page, desc, type, null, query, results);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record an Application Start Event.
	 * 
	 * @author twindsor
	 * 
	 */
	public class Start extends ScriptableFunction {
		private static final String NAME = FIELD_START;

		public Object invoke(Object obj, Object[] args) throws Exception {
			try {
				WebtrendsDataCollector.getInstance().onApplicationStart(config.wt_dc_app_name(), null);
			} catch (final Exception e) {
				System.err.println(e.getMessage());
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record an Application Termination Event.
	 * 
	 * @author twindsor
	 * 
	 */
	public class Terminate extends ScriptableFunction {
		private static final String NAME = FIELD_TERMINATE;

		public Object invoke(Object obj, Object[] args) throws Exception {
			try {
				WebtrendsDataCollector.getInstance().onApplicationTerminate(config.wt_dc_app_name(), null);
			} catch (final Exception e) {
				System.err.println(e.getMessage());
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record an Application Error Event. Takes: <br/>
	 * 
	 * <pre>
	 * String Message
	 * </pre>
	 * 
	 * @author twindsor
	 * 
	 */
	public class Error extends ScriptableFunction {
		private static final String NAME = FIELD_ERROR;

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args != null && args.length == 1) {
				try {
					final String message = (args[0].toString());
					Hashtable customParams = new Hashtable();
					customParams.put("WT.er", message);
					WebtrendsDataCollector.getInstance().onApplicationError(config.wt_dc_app_name(), customParams);
				} catch (final Exception e) {
					System.err.println(e.getMessage());
				}
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record an Application Foreground Event.
	 * 
	 * @author twindsor
	 * 
	 */
	public class Foreground extends ScriptableFunction {
		private static final String NAME = FIELD_FOREGROUND;

		public Object invoke(Object obj, Object[] args) throws Exception {
			try {
				WebtrendsDataCollector.getInstance().onApplicationForeground(config.wt_dc_app_name(), null);
			} catch (final Exception e) {
				System.err.println(e.getMessage());
			}
			return UNDEFINED;
		}
	}

	/**
	 * Record an Application Background Event.
	 * 
	 * @author twindsor
	 * 
	 */
	public class Background extends ScriptableFunction {
		private static final String NAME = FIELD_BACKGROUND;

		public Object invoke(Object obj, Object[] args) throws Exception {
			try {
				WebtrendsDataCollector.getInstance().onApplicationBackground(config.wt_dc_app_name(), null);
			} catch (final Exception e) {
				System.err.println(e.getMessage());
			}
			return UNDEFINED;
		}
	}

}
