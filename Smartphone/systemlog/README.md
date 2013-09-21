# WebWorks System Event Log Information

## NOTE: THIS IS A WORK IN PROGRESS

The WebWorks System Event Log API will allow you to write to the Blackberry System Event Log from within your WebWorks applications. 

**Authors:** 
[John Mutter](https://github.com/jmutter)
[Matthew Haag](https://github.com/muerl)

## Known Issues

* None

## Tested On

* BlackBerry Bold 9900 v7.0.0.261
* BlackBerry Torch 9800 v6.0.0.227

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.system.log**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Copy extracted contents to the _**webworks.system.log**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.system.log**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.ext.system.log" />

## Summary

blackberry.ext.system.log.setup ( [String:App Guid Name] , [String:App Name] , [Integer:Mode] )

blackberry.ext.system.log.write ( [String:System Log Message Text] )



## Options Summary
	
## Item Summary
[String:App Guid Name] // This should be a unique package like name to identify your application example... "com.demo.myapp"

[String:App Name] // This is the friendly name you would see listed as the origin of the log message.  "My Application Name"

[String:System Log Message Text] // This is the physical text that would make up the actual system event log message.

[Integer:Mode] //This is an ENUM, 1 for System Log, 2 for Standard Out, 3 for both.  There are defined ENUMs but they are broken will be fixed in next version

## Initialization Example

    function setupLog() {
    
      blackberry.ext.system.log.setup ( "com.demo.myappname", "My Application", 3 );
    
    }
	
	function writeLog(){
		blackberry.ext.system.log.write( "Test Message");
	}
