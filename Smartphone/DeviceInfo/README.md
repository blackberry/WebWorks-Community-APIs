# Device Info API Information

## NOTE: THIS IS A WORK IN PROGRESS

The Device Info API will allow you to return information about the device. The information is provided back to the user
in the form of a json string. Individual elements can be returned or a complete list of all the device information that
is exposed to the user through the Java API's.

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

2. Create a new _**webworks.device.info**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\DeviceInfo**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\DeviceInfo**_ directory to your new _**ext\webworks.device.info**_ directory

6. Copy the downloaded _**Smartphone\DeviceInfo\src\webworks**_ directory to your new _**ext\webworks.device.info\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.device.info**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    	<feature id="webworks.device.info" required="true" version="1.0.0.0"/>

## Summary Usage:
To return all device properties:
webworks.device.info.getDeviceInfo()

Returning specific properties:
webworks.device.info.getDeviceInfo([String:property],[String:property], [String:property])

Available properties include:

1.  Reset Idle Time: Property: canResetIdleTime

2.  Battery Level: Property: getBatteryLevel

3.  Battery Status: Property: getBatteryStatus

4.  Battery Temperature: Property: getBatteryTemp

5.  Device ID: Property: getDeviceId

6.  Device Name: Property: getDeviceName

7.  Idle Time: Property: getIdleTime

8.  Lockout Timeout: Property: getLockoutTimeout

9.  Manufacturer Name: Property: getManufacturerName

10. Platform Version: Property: getPlatformVersion

11. Software Version: Property: getSoftwareVersion

12. Total Flash Size: Property: getTotalFlashSize

13. USB Connection State: Property: getUSBConnectionState (returns connected|disconnected)

14. Has Camera: Property: hasCamera

15. IS Backlight On: Property: isBacklightOn

16. Is Battery Removable: Property: isBatteryRemovable

17. Is Pasword Enabled: Property: isPasswordEnabled

18. Is Simulator: Property: isSimulator

Example:

    
    <script type="text/javascript" >
   
    
    var app = {

	getAllDeviceInfo : function() {
    		//starts the vibration for a maximum of 25,500 milliseconds
    		var allDeviceInfo = webworks.device.info.getDeviceinfo();
    	},
    	getBatteryLevelAndStatus : function () {
    		var result = webworks.device.info.getDeviceinfo("getBatteryLevel", "getBatteryStatus");
    	}
    }
    //returns battery level and status as a JSON string
    app.getBatteryLevelAndStatus();
    </script>

