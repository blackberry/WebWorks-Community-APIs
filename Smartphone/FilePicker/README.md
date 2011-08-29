# File Picker
The spinner control will allow you to have a more usable interface when interacting
with &lt;select&gt; elements on a BlackBerry 5.0 browser.  It will also allow you to keep 
the same consistent feel with your UI on a BlackBerry 6.0 browser.

**Author:** [Tim Neil](https://github.com/tneil)

## Tested On

* BlackBerry Bold 9700 v5.0.0.469
* BlackBerry Storm 9550 v5.0.0.469
* BlackBerry Torch 9800 v6.0.0.227, v6.0.0.278

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/tneil)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.ui.filePicker**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\FilePicker**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\FilePicker**_ directory to your new _**ext\webworks.ui.filePicker**_ directory

6. Copy the downloaded _**Smartphone\FilePicker\src**_ directory to your new _**ext\webworks.ui.filePicker\src*_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.ui.filePicker**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.ui.filePicker" />

## Summary

    static void open(callback : function (filePath : Number) ) // If nothing is selected undefined is returned

   
## Code Example

    <html>
		<head>
		<meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
		<script type="text/javascript" >
		  function doClick() {
				webworks.ui.filePicker.open(doCallback);
		  }
		  
		  function doCallback(file) {
			alert('You selected file: ' + file);
		  }
		</script>
		</head>
		<body >
		<h1>Hello World</h1>
		<button onclick="doClick()">Click Me</button>
		</body>
	</html>

## Usage Information
When creating a spinner control you will pass it a callback function that will 
be invoked once the user selects a file.  If no file is selected the callback will
have undefined returned to it


_**NOTE:**_ The callback is handled asynchronously, so code that is placed directly after
the "open" function call will be executed immediately while waiting for the user's 
response input.

