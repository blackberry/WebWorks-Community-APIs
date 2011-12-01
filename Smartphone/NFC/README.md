# NFC Information

The NFC API enables the ability to leverage the near field communication (NFC) capabilities of the Blackberry platform. NFC support was introduced in the BlackBerry development platform starting with BlackBerry 7. This extension allows WebWorks applications to read supported NDEF record types. 

**Note**: there are multiple types of NDEF record types defined as part of the NFC spect.  Currently this NFC API supports only _smart poster_.

**Authors:** 

* Martin Wooley (http://supportforums.blackberry.com/t5/user/viewprofilepage/user-id/191236)
* John Murray

## Tested On

* BlackBerry Bold 9900 7.0 b2380
* BlackBerry Curve 9360 7.0 b2365


**Requires:** BlackBerry WebWorks SDK (for Smartphone) v2.2 or higher and BlackBerry handheld code 7.0+

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues)


## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Example path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks SDK 2.2.0.15\ext**_

2. Create a new _**blackberry.nfc.ndef**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\NFC**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\NFC**_ directory to your new _**ext\blackberry.nfc.ndef**_ directory

6. Copy the downloaded _**Smartphone\NFC\src\blackberry**_ directory to the new _**ext\blackberry.nfc.ndef\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\blackberry.nfc.ndef**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.


## Summary

** Required Feature ID **

Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.nfc.ndef" />

** Methods **

	static Undefined init_logging();
	static Undefined register_ndef(onScan : function, onError : function, record_type : Object);
	static Undefined unregister_ndef(record_type : Object);
	
	
**Supported Record Types**

    Sp - Smart poster Ndef tag

	
## Usage Information

If a second call is made to the register_ndef message after a listener has already been registered, an error will occur.

## Config.xml Example

	<?xml version="1.0" encoding="UTF-8"?>
	<widget xmlns="http://www.w3.org/ns/widgets" 
			xmlns:rim="http://www.blackberry.com/ns/widgets" 
			version="1.0.0.0">
	  <name>NfcDemo</name>
	  <feature id="blackberry.nfc.ndef" required="false" version="1.0.0"/>
	  <content src="index.html"/>
	</widget>

## Sample application 

	<html>
	<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<meta name="viewport" id="viewport"
		content="height=device-height,width=device-width,user-scalable=no" />
	
	<script type="text/JavaScript">
		function initLogging() {
			if (blackberry.nfc.ndef.init_logging()) {
				console.log("init_logging() ready.");
			} else {
				alert("The was a problem initialising logging");
			}
		}
		function registerListener(record_type) {
			if (blackberry.nfc.ndef.register_ndef(onScan, onError, record_type)) {
				alert("NDEFMessageListener registered OK for record type " + record_type);
			} else {
				alert("There was a problem registering a NDEFMessageListener for record type " + record_type)
			}
		}
		function removeListener(record_type) {
			if (blackberry.nfc.ndef.unregister_ndef(record_type)) {
				alert("NDEFMessageListener unregistered OK for record type " + record_type);
			} else {
				alert("The was a problem unregistering the NDEFMessageListener for record type " + record_type)
			}
		}
		// call back functions
		function onScan(ndef_message) {
			alert("NDEF message:" + ndef_message);
		}
		function onError(error_message) {
			alert("NDEF error:" + error_message);
		}
		
	</script>
	</head>
	<body onLoad="initLogging();registerListener('Sp');">
		
		<p>NFC Extension user</p>
		<input type="button" value="Register NDEF listener" onclick="registerListener('Sp')" />
		<input type="submit" value="Register unsupported type" onclick="registerListener('XX')" />
		<input type="submit" value="Unregister NDEF listener" onclick="removeListener('Sp')" />

	</body>
	</html>
