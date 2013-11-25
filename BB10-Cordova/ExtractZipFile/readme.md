ExtractZIPFile 
==============
Provides extraction (unzipping) of zip archives for html5 under Blackbery 10.


API Examples
--------------
### Example of API usage
	function extractFile(fileName) {
		community.extractZipFile.extract(
			{
				zip: 'folder1/' + zipFilename,
				destination: 'folder2/destination',
				overwriteFiles: false,
				tarBombProtection: false,
				callbackToken: '',
			},
			onExtractCompletion);
	}

	function onExtractSuccess(status) {	
		if (status.result < 0) {
			alert("Extraction Failed");
			console.log(status.resultMessage);

		} else {
			alert("Extracted " + status.zip + " to " + status.destination);
			console.log(status.resultMessage);
		}
		console.log("Extraction returned with token: " + callbackToken);
  	}	 
    									
API Details
===============
The API takes an option object and a callback function.
The following options are supported.

### zip
Required: As you can guess an unknown zip cannot be extracted.
The path and file name to the zip file which is to be extracted.

Default: none! This argument is required!


### destination
The folder into which the zip will be extracted.

Default: '.', Current working directory.


### overwriteFiles
If already existing files should be overwritten by those within the zip.

Default: false, No files will be overwritten.


### tarBombProtection
If true any zips which do not contain a folder as the root will extracted into a
new folder named after the zip archives name.

Default: false. 


### callbackToken
String which will be passed back without alteration upon callback. If not given
callbackToken will contain a copy of the zip argument. Use this argument for
concurrent zip extraction to differentiate between callbacks.

Default: duplicate of zip argument.

