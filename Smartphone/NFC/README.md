# NFC Information

The NFC API enables the ability to leverage the near field communication (NFC) capabilities of the Blackberry platform. NFC support was introduced in the BlackBerry development platform starting with BlackBerry 7. This extension allows WebWorks applications to read supported NDEF record types. 

**Authors:** 

* Martin Woolley (http://supportforums.blackberry.com/t5/user/viewprofilepage/user-id/191236)
* John Murray
* Rob Williams Jnr

## Tested On

* BlackBerry Bold 9900 7.1 b1698


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

**Required Feature ID**

Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.nfc" />

**Methods**

* blackberry.nfc

		static Object isNFCAvailable();
		static Object isNFCSupported();


* blackberry.nfc.ndef

		static Object init_logging();
		static Object register_ndef(onScan : function, onError : function, record_type : Object);
		static Object unregister_ndef(record_type : Object);
	
	
**Supported Record Types**

Any type allowed by the underlying OS.

	
## Usage Information

The source for a sample application is contained in sampleSrc.
