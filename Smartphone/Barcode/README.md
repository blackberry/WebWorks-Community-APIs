# Barcode Information
The Barcode API will allow scan a 1D or 2D barcode and read the data contained in the code.  It will also allow you to generate 1D and 2D barcodes.

**Authors:** [Jeff Heifetz](https://github.com/jeffheifetz), [Brent Thornton](https://github.com/bthornton32), [Gord Tanner](https://github.com/gtanner), [Sergey Golod](https://github.com/tohman)

## Tested On

* BlackBerry Torch 6.0 b3242

**Requires BlackBerry WebWorks SDK v2.1.1 or higher and BlackBerry handheld code 6.0+**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.media.barcode**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Barcode**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Barcode**_ directory to your new _**ext\webworks.media.barcode**_ directory

6. Copy the downloaded _**Smartphone\Barcode\src\blackberry**_ directory to your new _**ext\webworks.media.barcode\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.media.barcode**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.media.barcode" />

## Summary

    static Undefined scan(onCaptured : function, onError : function, options : object);
    static Undefined generate(content : String, onGenerated : function, onError : function, options : Object);
	
**Scan Options**

    optional readwrite Boolean tryHarder; // Default true
	optional readwrite String[] format; //['CODE_128', 'CODE_39', 'EAN_13', 'EAN_8', 'ITF', 'UPC_A', 'UPC_E', 'QR_CODE', 'DATAMATRIX', 'PDF417']
	optional read Boolean focusOptions: 'autoFocus' : true || 'macroFocus' : true
	optional read String zoomOptions: 'digitalZoom' : 'int val' || 'opticalZoom' : 'int val'
	
**Generate Options**

    optional readwrite String characterSet; // Type of character set to use 
	optional readwrite Number errorCorrection; // Percentage of error correction
	optional readwrite String format; // barcode format Default ['QR_CODE']
	readwrite Number width; 
	readwrite Number height;
	optional readwrite String filePath; // file path to place the generated image Default ['file:///SDCard/<current time in milliseconds>.png']

	
## Usage Information

When generating a barcode it will always generate a PNG file.  Please ensure your file path provided to generate uses a ".png" file extension

## Scan Code Example

<html>
	<head>
		<meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
		<script type="text/javascript" >
			function onCaptured(value) {
				alert(value);
			}

			function onError(error) {
				alert('Exception: ' + error.message + ' : ' + error.code);
			}

			// Scans QR codes and Barcodes 5,6,7
			function doScan() {
				// default format = all
				/*
				 * using the following formats will result in the default format. Choose the formats you are most interested in to speed up
				 * the detection time of the barcode you are trying to decode
				 * 'formats' : ['CODE_128', 'CODE_39', 'EAN_13', 'EAN_8', 'ITF', 'UPC_A', 'UPC_E', 'QR_CODE', 'DATAMATRIX', 'PDF417']
				 */ 
				var options = {
					'tryHarder' : true,
					'formats' : ['DATAMATRIX', 'QR_CODE']
				};
				var focusOptions = {
					'autoFocus' : true,
					'macroFocus' : true					
				};
				/*
				 * You can use the following zoom options
				 * 'digitalZoom' : '100'
				 * 'opticalZoom' : '100'
				 * where 100 is the default zoom. 1x = 100, 2x = 200. You must provide a value within the zoom range otherwise this will fail
				 * opticalZoom is not widely available on most devices so it is not provided in this sample.
				 */ 
				var zoomOptions = {
					'digitalZoom' : '100'
				};
				try {
					webworks.media.barcode.scan(onCaptured, onError, options, focusOptions, zoomOptions);
				} catch(e) {
					alert("Error: " + e);
				}
			}
		</script>
	</head>
	<body >
		<button onclick="doScan()">
			Scan Code
		</button>
	</body>
</html>

## Generate Code Example

    <html>
      <head>
        <meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
        <script type="text/javascript" >
		
	      // Generates an image based on the file extension for the filePath
		    function generateBarcode() {
		        // default codeType = qr
		        // default errorCorrectio = 50
		        var options = { 'format' : 'QR_CODE', /*required*/
		                    'width' : 50, /*required*/
		                    'height' : 50, /*required*/
		                    'filePath' : 'file:///SDCard/lkajsl.png' /*required*/
		                     }
		        try{
		            webworks.media.barcode.generate("1234567890123", onGenerated, onError, options);
		        }catch(e){
		            alert("Error: " + e);
		        }
    }
		 
		 function onError(error) {
			alert('Exception: ' + error.message + ' : ' + error.code);
		 }
		 
		 function onGenerated(filePath) {
		    alert('code generated here:' + filePath);
		
		    // do upload, display, delete.. whatever..
	     }
		 
        </script>
      </head>
      <body >
    	<button onclick="generateBarCode()">Generate Code</button>
      </body>
    </html>

