# Locale Information
The Locale API will allow you to retrieve the locale string based on the device system settings, as well as the locale string configured for the current application.

**Author:** [Tim Neil](https://github.com/tneil)

## Tested On

* BlackBerry Storm 9550 v5.0.0.469
* BlackBerry Bold 9700 v5.0.0.469
* BlackBerry Torch 9800 v6.0.0.227

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks/issues) or contact the [Author](https://github.com/tneil)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**blackberry.system.locale**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Locale**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Locale**_ directory to your new _**ext\blackberry.system.locale**_ directory

6. Copy the downloaded _**Smartphone\Locale\src\blackberry**_ directory to your new _**ext\blackberry.system.locale\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\blackberry.system.locale**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.system.locale" />

## Summary

    static String deviceLocale // Returns locale setting of the device
    static String appLocale // Returns locale of current application

## Code Example

    <html>
      <head>
        <meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
        <script type="text/javascript" >
          function deviceLocale(){
    	     alert(blackberry.system.locale.deviceLocale);
    		}
    		
    	  function appLocale() {
    		alert(blackberry.system.locale.appLocale);
    	  }
        </script>
      </head>
      <body >
    	<button onclick="deviceLocale()">Device Locale</button>
    	<button onclick="appLocale();">App Locale</button>
      </body>
    </html>

