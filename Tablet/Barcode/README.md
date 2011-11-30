# Barcode Information
The Barcode API will allow scan a 1D or 2D barcode and read the data contained in the code.  It will also allow you to generate 1D and 2D barcodes.

**Author:** [Sergey Golod](https://github.com/tohman), [Gord Tanner](https://github.com/gtanner)

## Tested On

* Blackberry PlayBook

**Requires BlackBerry WebWorks SDK for Tablet OS v2.1 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Tablet OS extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks SDK for TabletOS\bbwp\ext**_

2. Create a new _**webworks.media.barcode**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Tablet\Barcode**_

5. Copy the _**library.xml**_ file from the downloaded _**Tablet\Barcode**_ directory to your new _**ext\webworks.media.barcode**_ directory

6. Copy the downloaded _**Tablet\Barcode\src**_ directory to your new _**ext\webworks.media.barcode\src**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.media.barcode**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.media.barcode" />
    <feature id="blackberry.io.dir" /> - Required when using 'generate' method.
    
## Required Permissions
Any references to files/directories under "shared" folder (e.g. music) requires this permission to be set in your application's config.xml file

    <rim:permit>access_shared</rim:permit>
    <rim:permit>use_camera</rim:permit> - Required when using 'scan' method.

## Summary

    static Undefined scan(onCaptured : function, [onError : function], [options : object]); // Scans a barcode
    static Undefined generate(content, onGenerated : function, [onError : function], [options : object]); // Generates a PNG image barcode
    
**Scan Options**

    optional readwrite Boolean tryHarder; // Default true
    optional readwrite String[] formats; // Default value ['1d', '2d']
    
**Generate Options**

    optional readwrite String characterSet; // Type of character set to use
    optional readwrite Number errorCorrection; // Percentage of error correction
    readwrite String format; // barcode format, default value 'QR'
    readwrite Number width; 
    readwrite Number height;
    readwrite String filePath; // file path to place the generated image

    
## Usage Information

* When generating a barcode it will always generate a PNG file. 
* Please ensure your file path provided to generate uses a ".png" file extension.
* File name would be timestamp with .png extension (i.e. 1312478402621.png) when file path is not provided or file extension is not valid.
* 'UPC_E', 'UPC_A', 'EAN_8', 'EAN_13', 'CODE_128', 'CODE_39', 'ITF', 'PDF417', 'QR' and 'DATAMATRIX' are supported barcode formates.
* Parameter passed to callbacks is of object type with following default values: 
   {'code': 0,   // 0 for Success, -1 for error.
    'msg': null, // Contains an error message.
    'data': {}}. // The actual data: value of 'content' for successful scan call, value of 'filePath' for successful generate call.


## Known Issues

For encoding currently 'EAN_8', 'EAN_13' and 'QR' are supported barcode formats.
Encoding hint 'errorCorrection' is not taken into consideration.

## Scan Code Example

    <html>
      <head>
        <meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
        <script type="text/javascript" >
          function doScan() {
            try {
              blackberry.media.barcode.scan(
                function successCB(resultObj) {
                  alert("Barcode scanning succeed: " + resultObj.data.content);
                }, 
                function errorCB(errorObj) {
                  alert("Error occured: " + errorObj.code + ", " + errorObj.msg);
                },
                {
                  'tryHarder' : false,
                  'format' : ['EAN_13', 'QR']
                }
              );
            } catch(e) {
              alert("Error occured when scanning barcode: " + e);
            }
          }
        </script>
      </head>
      <body >
        <button onclick="doScan()">Scan Code</button>
      </body>
    </html>

## Generate Code Example

    <html>
      <head>
        <meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
        <script type="text/javascript" >
        
          function generateBarCode() {
            var cameraDirFilePath = blackberry.io.dir.appDirs.shared.camera.path + "/qrImageEncode.png";

            try {
              blackberry.media.barcode.generate("Encodng Checked!", 
                function successCB(resultObj) {
                  alert("Barcode generation succeed: " + resultObj.data.filePath);
                },
                function errorCB(errorObj) {
                  alert("Error occured: " + errorObj.code + ", " + errorObj.msg);
                },
                {
                  'format' : 'qr',
                  'width' : 250,
                  'height' : 250,
                  'characterSet' : 'UTF-8',
                  'filePath' : cameraDirFilePath,
                  'errorCorrection' : 50
                }
              );
            } catch(e) {
                alert("Error occured when generating barcode: " + e);
            }
          }
        </script>
      </head>
      <body >
        <button onclick="generateBarCode()">Generate Code</button>
      </body>
    </html>

