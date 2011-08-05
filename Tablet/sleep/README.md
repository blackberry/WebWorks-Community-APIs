# Sleep Object

## NOTE: THIS IS A WORK IN PROGRESS

The sleep object will allow you to suspend and enable the BlackBerry Tablet OS backlight by changing the system IdleMode property.

**Authors:** [Jim Ing](https://github.com/myjing), [Chris Del Col](https://github.com/cdelcol), [Adam Stanley](https://github.com/astanley)

## Tested On

* BlackBerry Tablet OS v 1.0.7.2670

**Requires BlackBerry WebWorks SDK for Tablet OS v2.0 or higher**

## Known Issues
1. PR 104184 defect logged: "Unable to change value of systemIdleMode to KEEP_AWAKE".


Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks/issues) or contact the [Author](https://github.com/myjing)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Tablet OS extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks SDK for TabletOS\bbwp\ext**_

2. Create a new _**blackberry.system.display**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Tablet\sleep\extension**_

5. Copy the _**library.xml**_ file from the downloaded _**Tablet\sleep\extension**_ directory to your new _**ext\blackberry.system.display**_ directory

6. Copy the downloaded _**Tablet\sleep\extension\src**_ directory to your new _**ext\blackberry.system.display\src**_ directory

**NOTE:** Be sure to back-up this _**ext\blackberry.system.display**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.


## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.system.display" />

## Summary

	static boolean allowSleep
	
	uri:
	webworks://webworks/system/display/allowSleep/get
	webworks://webworks/system/display/allowSleep/set

	return:
	{
		"code" : "0",
		"msg" : null,
		"data" : {
			"allowSleep" : "true"
		}
	}
	
## Code Example

	function showStatus()
	{
		var status = webworks.system.display.allowSleep;
		alert("Allow Sleep = " + status);
	}
	function preventSleep()
	{
		blackberry.system.display.allowSleep = false;
		showStatus();
	}
	function allowSleep()
	{
		blackberry.system.display.allowSleep = true;
		showStatus();
	}
