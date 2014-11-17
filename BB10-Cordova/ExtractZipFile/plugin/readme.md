ExtractZIPFile 
==============

**Provides:**
* extraction (unzipping) of zip archives for html5 under Blackberry 10.
* compression (zipping) of a single file or multiple files for html5 under Blackberry 10.

## Version History

	1.0.0 Initial Release
	1.1.0 Adds compression of files
	1.1.1 Adds compression of multiple files and directories

**Tested On**

* BlackBerry 10.2.1.1927
* Webworks 2.0.0.71
* Dev Alpha C

**Author(s)** 

* [Daniel Dressler](https://github.com/daniel-dressler)
* [Morgan Parlee](https://github.com/mkparlee) 
* [Tim Tung](https://github.com/t470520) 


## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.extractZipFile

or,
	
	webworks plugin add com.blackberry.community.extractZipFile

Installation from source is the same but instead of the id ("com.blackberry.community.extractZipFile"), use the file system path to the source plugin folder.


Extraction API Examples
--------------
### Example of API usage
	function extractFile(fileName) {
		community.extractZipFile.extract(
			{
				zip: 'folder1/' + zipFilename,
				destination: 'folder2/destination',
				overwriteFiles: true,
				tarBombProtection: false,
				callbackToken: '',
			},
			onExtractCompletion);
	}

	function onExtractCompletion(status) {	
		if (status.result < 0) {
			alert("Extraction Failed");
			console.log(status.resultMessage);

		} else {
			alert("Extracted " + status.zip + " to " + status.destination);
			console.log(status.resultMessage);
		}
		console.log("Extraction returned with token: " + status.callbackToken);
  	}	 
    									
Extraction API Details
===============
The API takes an option object and a callback function.
The following options are supported.

### zip
Required: As you can guess an unknown zip cannot be extracted.
The path and file name to the zip file which is to be extracted.

Default: none! This argument is required!


### destination
The folder into which the zip will be extracted.

Default: './', Current working directory.


### overwriteFiles
If already existing files should be overwritten by those within the zip.

Default: true, Files will be overwritten.


### tarBombProtection
If true any zips which do not contain a folder as the root will extracted into a
new folder named after the zip archives name.

Default: false. 


### callbackToken
String which will be passed back without alteration upon callback. If not given
callbackToken will contain a copy of the zip argument. Use this argument for
concurrent zip extraction to differentiate between callbacks.

Default: duplicate of zip argument.



Compression API Examples
--------------
	### Example of API usage for single file compression
	function compressFile(filePath) {
		community.extractZipFile.compress(
			{
				filePath: filePath, // filePath = "./app/native/res/zip/fileToCompress.txt"
				destination: "./app/native/res/zip/zipFileDestination.zip",
				callbackToken: ''
			},
			onCompressionCompletion);
	}
	
	### Example of API usage for multiple files or directories compression
	function compressFile(filePath) {
		community.extractZipFile.compress(
			{
				filePath: filePath, // filePath = "./app/native/res/zip/aaaa.txt:./app/native/res/zip/3"
				destination: "./tmp/case/folder333.zip",
				callbackToken: ''
			},
			onCompressionCompletion);
	}

	function onCompressionCompletion(status) {	
		if (status.result < 0) {
			alert("Compression Failed");
			console.log(status.resultMessage);

		} else {
			alert("Compression Succeeded to destination: " + status.callbackToken);
			console.log(status.resultMessage);
		}
		console.log("Compression returned with token: " + status.callbackToken);
  	}	 
    									
Compression API Details
===============
The API takes an option object and a callback function.
The following options are supported.

### filePath
Required: As you can guess an unknown file cannot be compressed.
The directory and file name (the path) to the file which is to be compressed. MUST BE A PROPER PATH -- directory + file name
For multiple files and directories compression, use : as a delimeter. All the files and directories inside a directory will be added to compression list by the crawler function, users do not need to input each single file or directory in specific.

Default: none! This argument is required!


### zipDestinationPath
The path destination into which the file will be compressed. Must end with .zip extension.

Default: none! This argument is required!


### callbackToken
String which will be passed back without alteration upon callback. If not given
callbackToken will contain a copy of the zipDestinationPath argument. Use this argument for
concurrent zip compression to differentiate between callbacks.

Default: duplicate of zipDestinationPath argument.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10-Cordova/ExtractZipFile?pixel)](https://github.com/igrigorik/ga-beacon)