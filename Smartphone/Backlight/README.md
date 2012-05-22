# Backlight Management API Information

## NOTE: THIS IS A WORK IN PROGRESS

The Backlight Management API will allow you to control the smartphone backlight. This API provides the ability to check for backlight properties as well as turning on and off the backlight.
Currently the API does not allow for users to set a time and will default to the timeout value set by the user in the profile settings.
 

**Authors:** [HyunJun Jung](https://github.com/hjung)

## Known Issues

* None

## Tested On

* BlackBerry Bold 9900 v7.1.0.xxx
* BlackBerry Torch 9810 v7.0.0.261


**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.device.backlight**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Backlight**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Backlight**_ directory to your new _**ext\webworks.device.backlight**_ directory

6. Copy the downloaded _**Smartphone\Backlight\src\webworks**_ directory to your new _**ext\webworks.device.backlight\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.device.backlight**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    	<feature id="webworks.device.backlight" required="true" version="1.0.0.0"/>

## Summary


To list all backlight properties:
webworks.device.backlight.manageBacklight([String:all])

To list individual properties (Returns a JSON string of key value pairs)
webworks.device.backlight.manageBacklight([String:Property, String:Property, String:Property...etc])

Available properties include:

1.  Get Brightness: Property: getBrightness

2.  Get Brightness Default: Property: getBrightnessDefault

3.  Get Brightness Increment: Property: getBrightnessIncrement

4.  Get Timeout Default: Property: getTimeoutDefault

5.  Is Brightness Configurable: Property: isBrightnessConfigurable

6.  Is Enabled: Property: isEnabled


To turn on or off the backlight:
webworks.device.backlight.manageBacklight([String:backlightEnable, String:on|off)

Set the brightness of the backlight
webworks.device.backlight.manageBacklight([String:setBrightness, String:number of seconds])

Set the timeout of the backlight
webworks.device.backlight.manageBacklight([String:setTimeout, String:number of seconds])
The maximum timeout is 255 seconds

Example:

    
    <script type="text/javascript" >
    var app = {
    	getBacklightProperties : function () {
    		
    		var properties = webworks.device.backlight.manageBacklight("all");
    		var propertyDiv = document.getElementById("properties");
    		propertyDiv.innerHTML = properties;
    	
    	},
    	turnOffBacklight : function () {
    		alert('the backlight will be turned off for 10 seconds');
    		webworks.device.backlight.manageBacklight("backlightEnable", "off");
    		setTimeout("app.turnOnBacklight()", 10000);
    	
    	},
    	turnOnBacklight : function () {
    		webworks.device.backlight.manageBacklight("backlightEnable", "on");
    	}
    }
    </script>

