# LED Notification API Information

## NOTE: THIS IS A WORK IN PROGRESS

The LED Notification API will allow you to control the LED. This will alow you to set a color pattern and duration of the LED notification. 

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

2. Create a new _**webworks.bbalert.led**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\LED**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\LED**_ directory to your new _**ext\webworks.bbalert.led**_ directory

6. Copy the downloaded _**Smartphone\LED\src\webworks**_ directory to your new _**ext\webworks.bbalert.led\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.bbalert.led**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    	<feature id="webworks.bbalert.led" required="true" version="1.0.0.0"/>

## Summary

webworks.bbalert.led.flashLED([String:start], [JSON String:pattern], [String:true|false])

webworks.bbalert.led.flashLED([String:stop])


Example:
//The hex color must be in the format 0x00[RRGGBB]
//The color patterns can be anything that the device can handle.

var app = {
		flashLED : function () {
		var pattern = '[{"hex-color": "0x00B22234","on-duration-in-milliseconds": "1000","transition-time-in-milliseconds": "500"},' +
						'{"hex-color": "0x00000000","on-duration-in-milliseconds": "1000","transition-time-in-milliseconds": "500"},'+
						'{"hex-color": "0x00FFFFFF","on-duration-in-milliseconds": "1000","transition-time-in-milliseconds": "500"},' +
						'{"hex-color": "0x00000000","on-duration-in-milliseconds": "1000","transition-time-in-milliseconds": "500"},' +
						'{ "hex-color": "0x00000FB3","on-duration-in-milliseconds": "1000","transition-time-in-milliseconds": "500"}]';
		webworks.bbalert.led.flashLED('start', pattern, 'true');
	},
	stopLED : function () {
		webworks.bbalert.led.flashLED('stop');
	}
}
