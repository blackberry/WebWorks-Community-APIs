# Vibrate Notification API Information

## NOTE: THIS IS A WORK IN PROGRESS

The Vibrate Notification API will allow you to control the device vibration. This will alow you to set the vibration duration up to 25000 milliseconds. 
If a value greather than 25500 milliseconds is provided the extension will set the maximum time to 25500 milliseconds.
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

2. Create a new _**webworks.bbalert.vibrate**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Vibrate**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Vibrate**_ directory to your new _**ext\webworks.bbalert.vibrate**_ directory

6. Copy the downloaded _**Smartphone\LED\src\webworks**_ directory to your new _**ext\webworks.bbalert.vibrate\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.bbalert.vibrate**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    	<feature id="webworks.bbalert.vibrate" required="true" version="1.0.0.0"/>

## Summary

webworks.bbalert.vibrate.manage([String:time in milliseconds|stop])



Example:

    
    <script type="text/javascript" >
   
    
    var app = {

	startVibrate : function() {
    		//starts the vibration for a maximum of 25,500 milliseconds
    		var vibrateStartResult = webworks.bbalert.vibrate.manage("25500");
    	},
    	stopVibrate : function () {
    		var vibrateStopResult = webworks.bbalert.vibrate.manage("stop");
    	}
    }
    //start the vibration using the default maximum value
    app.startVibrate();
    </script>

