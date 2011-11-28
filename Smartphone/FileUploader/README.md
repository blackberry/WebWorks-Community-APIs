# File Uploader
The File Uploader extension provides a means to upload files to file server. 

**Author:** [Jerome Carty](https://github.com/jcarty)

## Tested On

* BlackBerry Torch 9810 v7.0.0.439
* BlackBerry Bold 9900 v7.0.0.353
* BlackBerry Bold 9650 v6.0.0.546
* BlackBerry Curve 9300 v5.0.0.716

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/jcarty)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.io.FileUploader**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\FileUploader**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\FileUploader**_ directory to your new _**ext\webworks.io.FileUploader**_ directory

6. Copy the downloaded _**Smartphone\FileUploader\src\webworks**_ directory to your new _**ext\webworks.io.FileUploader\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.io.FileUploader**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.io.FileUploader" />

## Summary


## Code Example

    function foo()
    {
        var options = {
			url: 'http://www.youruploadurl.com/path',
			file: 'file:///' + file,
			fileKey: 'media[]',
			mimeType: 'image/jpg',
			timeout: 60000, // in milliseconds default is 30000
			params: {
				custom1: 'test'
			},
			headers: {
				'Authorization', 'AUTH_HERE'
			},
			success: function(data) {
				alert('Success!');
			},
			error: function(e, code) {
				alert(e + " " + code);
			}
		};
		
		webworks.io.FileUploader.upload(options);
    }

## Usage Information

_**NOTE:**_ The _**error**_ and _**success**_ callbacks are handled asynchronously, so code that is placed directly after
the "upload" function call will be executed immediately while waiting for the user's 
response input.

## Advanced
This extension reads the transport configuration in your config.xml (only if it exists).

If it does not, the following transport order is assumed:

- WIFI
- BIS-B
- MDS
- TCP_CELLUAR
- WAP2
- WAP

For more information, visit: http://supportforums.blackberry.com/rim/attachments/rim/java_dev@tkb/397/1/Network_Tranports_tutorial.pdf

## Properties
**url:**
URL to upload file to

**file:**
File to upload to remote server

**fileKey:**
Field name of the uploaded file

**mimeType:**
Uploaded file mimeType (if not specified, default is JPG)

**params:**
Extra parameters to send along with your file

**headers:**
Set HTTP request headers

**success:**
Success callback. This method is called when the POST has completed successfully.

**error:**
Error callback. This method is called when there is an error (invalid file paths, file not found, etc)

**timeout:**
The amount of milliseconds this extension will attempt a connection.

## Change Log
-11/15/2011
	
- Closes gh-1

11/9/2011

- Extension now reads <rim:connection> tag from config.xml
- Timeout option added (defaults to 30000 milliseconds)