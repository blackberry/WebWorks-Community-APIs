# Message List Information

## NOTE: THIS IS A WORK IN PROGRESS

The Notification API will allow you to register your application with the BlackBerry Custom notification and sound profiles.  Users can select their own preference for alert notification from applications as content is updated. 

**Authors:** [John Mutter](https://github.com/jmutter)

## Known Issues

* None

## Tested On

* BlackBerry Bold 9900 v7.0.0.261
* BlackBerry Torch 9800 v6.0.0.227
* BlackBerry Storm 9550 v5.0.0.469
* BlackBerry Bold 9700 v5.0.0.469

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.notification**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Notification**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Notification**_ directory to your new _**ext\webworks.notification**_ directory

6. Copy the downloaded _**Smartphone\Notification\src\webworks**_ directory to your new _**ext\webworks.notification\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.notification**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.notification" />

## Summary

webworks.notification.register ( [String:App Guid Name] , [String:App Name] )

webworks.notification.fire ( [String:App Guid Name] )

webworks.notification.cancel ( [String:App Guid Name] )

webworks.notification.unregister ( [String:App Guid Name] )



## Options Summary
	
## Item Summary
[String:App Guid Name] // This should be a unique package like name to identify your application example... "com.demo.myapp"

[String:App Name] // This is the friendly name you would see in the sounds for a selected profile list.  "My Application Name"


## Initialization Example

    function startTasks() {
 	    
			webworks.notification.register ( "com.demo.myappname" , "My Application Name" )
			
		}

    function fireEvent() {
    
      webworks.notification.fire ( "com.demo.myappname" )
    
    }
    
    function cancelEvent() {
    
      webworks.notification.cancel ( "com.demo.myappname" )
    
    }
