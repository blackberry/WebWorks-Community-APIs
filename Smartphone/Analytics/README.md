# Analytics SDK extension Information

The BlackBerry Analytics SDK allows you to quickly capture analytics data from your applications and review this 
aggregate data about your user base in the Analytics Portal.

Register for the SDK here: http://www.blackberry.com/developers/platform/analyticsservice/

View documentation on the Portal here: http://docs.blackberry.com/en/developers/subcategories/?userType=21&category=Analytics+Service

**Authors:** [Tim Windsor](https://github.com/timothywindsor)

## Known Issues

* None

## Tested On

* BlackBerry Torch 9810 v7.1.0.213

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.analytics**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Analytics**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Analytics**_ directory to your new _**ext\webworks.analytics**_ directory

6. Copy the downloaded _**Smartphone\Analytics\src\webworks**_ directory to your new _**ext\webworks.analytics\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.analytics**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.analytics" />

## How to add the Extension to your app

REQUIRED: Visit www.blackberry.com/developers/platform/analyticsservice/ to register and for more details of the SDK.
DCSID is provided by the Analytics SDK, when you register a space.

Put a Script like this in your <head> element:

<script type="text/javascript">
	function onBackgroundCallback() {
		webworks.analytics.background();
	}
	function onForegroundCallback() {
		webworks.analytics.foreground();
	}
	function onExitCallback() {
		webworks.analytics.terminate();
	}
	
	// The dcs id is a value you will get from the Analytics Portal when you create a Space for your app. This is a test id.
	// second parameter is app category
	webworks.analytics.register('dcsgkyqzl6bv0h08zoc7zofva_4h6y', 'Utilities');
	webworks.analytics.start();
	blackberry.app.event.onBackground(onBackgroundCallback);
	blackberry.app.event.onForeground(onForegroundCallback);
	blackberry.app.event.onExit(onExitCallback);
</script>

After that, call methods like the following to track events in your app:

Track when a user clicks on an ad:
	webworks.analytics.adclick(/mainscreen', 'Main Screen', 'adclick', 'Demo Ad')
Track when users have been shown ads:
	webworks.analytics.adimpression('/mainscreen', 'Main Screen', 'adimpression', 'Demo Ad1', 'Demo Ad2')
Track when a button has been clicked in the app (does a feature get used? do users find this button?):
	webworks.analytics.button('/mainscreen', 'Main Screen', 'upgradebutton')
Track conversions/purchases in your app:
	webworks.analytics.conversion('/mainscreen/upgrade', 'Main Screen', 'upgrade', 'main app content', 'upgrade to full version')
Track anything not covered in another method:
	webworks.analytics.custom('/mainscreen', 'Custom', 'custom event')
Track media playback:
	webworks.analytics.media('/mainscreen/videoplay', 'Main Screen', 'media', 'Sports', 'tennis.mp4', 'video', 'p')
Track views of a product:
	webworks.analytics.product('/mainscreen/product', 'Product Screen', 'productview', 'Sports', '12345', 'SKU0001')
Track loading of pages/screens:
	webworks.analytics.screen('/mainscreen', 'Main Screen', 'mainscreen view', 'main app content')
Track search queries in the app:
	webworks.analytics.search('/mainscreen/search', 'Searc Screen', 'QueryString', '10')
Track errors:
	webworks.analytics.error('Error Message')
	
* The first parameter for most methods is the hierarchical location within the app, such as the screen or page where the event occurs. 
* The second parameter for most methods is the name of the screen where the event occurs.
* See the JavaDocs for the extension or the Analytics SDK JavaDoc for further details of the parameters.

## FAQ

# What happens out of coverage?
The library will queue events and send when in coverage. The queue is quite large, but if the queue overflows, events will be dropped.

# What is the longest string I can send?
The strings are essentially unlimited, but it is recommended to keep them as short as possible. Longer strings will require more data and battery to transmit, 
and they will be of limited use in the portal. The point of Analytics is really to count events over time, not send a lot of detailed data.