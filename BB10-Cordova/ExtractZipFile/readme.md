ExtractZIPFile 
==============
Provides extraction (unzipping) of zip archives for html5 under Blackbery 10.
The functionality is exposed through three apis: Android, iOS, and Raw.


APIs Overview
--------------
All APIs are available at all times. Call the one you prefer.
### Android & iOS
These two compatibility APIs implement the calling conventions of ther respective
platform's ExtractZipFile plugins. Thus they serve as drop-in replacements.
Both APIs are functional equivilents.
If these APIs meet your needs then you should need no special code to support BB10.

### Raw
The Raw api exposes features not present in the compatibility APIs. It allows
concurrent extraction of multiple zip files. It also provides more detailed
errors included error descriptions. Warning: the Raw API is BB10 only.


API Examples
--------------
### Example of Android API
	function extractFile(fileName) {
		var ZipClient = new ExtractZipFilePlugin();
		ZipClient.extractFile(
			'folder1/' + fileName,
			onExtractSuccess,
			onExtractFailure,
			'folder2/destination');
	}

	function onExtractSuccess(status) {	 
   		console.log('Success:' + status);
  	}	 
    												  
   	function onExtractFailure(error) { 
   		console.log('Failure:' + error);
   	}
	
### Example of iOS API
Notice the different argument ordering.

	function extractFile(fileName) {
		window.plugins.extractZipFile.extractFile(
			'folder1/' + zipFilename,
			'folder2/destination',
			onExtractSuccess,
			onExtractFailure);
	}

	function onExtractSuccess(status) {	 
   		console.log('Success:' + status);
  	}	 
    												  
   	function onExtractFailure(error) { 
   		console.log('Failure:' + error);
   	}
	
	
### Example of Raw API
	function extractFile(fileName) {
		PATH.TO.PLUGIN.extractZipFile(
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
    									
Raw API Details
===============
Raw API takes an option object and a callback function.
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

