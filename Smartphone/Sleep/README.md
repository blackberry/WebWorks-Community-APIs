# Sleep Object

## NOTE: THIS IS A WORK IN PROGRESS

The sleep object will allow you to suspend and enable the BlackBerry Device Software backlight through extending the security timer.
This is accomplished by calling Backlight.enable(true, INTERVAL).  

**Authors:** [Adam Stanley](https://github.com/astanley), [Jim Ing](https://github.com/myjing), [Chris Del Col](https://github.com/cdelcol)

## Tested On

* BlackBerry Torch 9810

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

## Known Issues
1. The Backlight.setTimeout class allows a max interval of only 255 s. In order to support a persistent backlight 'on' experience, a Timer is used to repeat the call made to enable() every INTERVAL seconds.  This behaviour can be affected by an IT Policy or Application Permission.
2. The user-defined backlight timeout (Device software options screen) cannot be read.  As a result, there is no way to restore the user-defined timeout.  Use Backlight.getTimeoutDefault() instead (default value is 30 seconds).


Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks/issues) or contact the [Author](https://github.com/astanley)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.system.display**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Sleep\extension**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Sleep\extension**_ directory to your new _**ext\webworks.system.display**_ directory

6. Copy the downloaded _**Smartphone\Sleep\extension\webworks**_ directory to your new _**ext\webworks.system.display**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.system.display**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.system.display" />

## Summary

	static boolean allowSleep

## Code Example

	function displayStatus()
	{
		var status = webworks.system.display.allowSleep;
		alert("Allow backlight timeout: " + status);
	}

	function allowSleep(val)
	{
		webworks.system.display.allowSleep = val;
		displayStatus();
	}

