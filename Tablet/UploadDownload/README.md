# File Transfer Information

## NOTE: THIS IS A WORK IN PROGRESS

The File Transfer API will allow you to download and upload files from/to a server

**Author:**
[Maurice White](https://github.com/mdwhite76)
[Rosa Tse](https://github.com/rwmtse)
[Tracy Li](https://github.com/tracyli)
[Dan Silivestru](https://github.com/dansilivestru)
[Ken Wallis](https://github.com/kwallis)

## Tested On

Nothing yet...

**Requires BlackBerry WebWorks SDK for Tablet OS v2.1 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors above

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Tablet OS extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.io.filetransfer**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Tablet\UploadDownload**_

5. Copy the _**library.xml**_ file from the downloaded _**Tablet\UploadDownload**_ directory to your new _**ext\webworks.io.filetransfer**_ directory

6. Copy the downloaded _**Tablet\UploadDownload\src\webworks**_ directory to your new _**ext\webworks.io.filetransfer\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.io.filetransfer**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.io.fileTransfer" />

## Summary

    static void getRemoteFileSize(remotePath); // Returns the "Content-length" header value for the content at the remotePath.
    static void download(remotePath, localPath, onProgress, onError, options); // Downloads the remote content to your localPath.
    statid void upload(remotePath, localPath, onProgress, onError, options); // Uploads the local content to the remotePath - remotePath points to a server side script (e.g. a PHP file) that knows how to handle the upload, the script will have full control of the file's destination
    
**Options**
                    
    Integer progressInterval; // Default 0.  The frequency, in milliseconds, that the onProgress callback should be called with updates on the upload/download operation.  0 means onProgress will only be called upon completion ofthe operation.
	optional String username; // the username required for an operation requiring authentication (not supported yet)
    optional String password; // the password required for an operation requiring authentication (not supported yet)
    optional connectionTimeout; // Default 30000.  The timeout for the operation, in milliseconds.
    optional BESMaxSingleChunk; // ???

**Status fields for onProgress callback**
    Integer percent; // current completion percentage of the operation
    Integer totalFileSize; // in Bytes
    Integer numBytes; // Number of bytes retrieved/posted so far
    String localPath;
    String remotePath; //original request remote path
    String redirectPath; //actual path if content was on a redirect (for download only)
    String startTime; // dateTime
    String mimeType; // (for download only)

## Code Example

    <html>
      <head>
        <meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
        <script type="text/javascript" >
            function doUpload() {
                var u = blackberry.io.fileTransfer;
                var options = {'progressInterval': 1000, 
                            'username':'foo', 
                            'password' : 'bar',
                            'connectionTimeout': 30000,
                            'BESMaxSingeChunk' : 3279}
                            
                // Path could be one of http/https
                u.uploadFile(remotePath, localPath, onProgress, onError, options);
            }
      
            function doDownload() {
                var d = blackberry.io.filetransfer;
                
                // Check the file size
                var sizeInBytes = d.getRemoteFileSize('http://foo/bar.jpg');
                if (sizeInBytes > 27498274) {
                    alert('hell no you are not downloading that thing!!');
                    return;
                }
                
                // progressInterval = 0 means, let me know when it is done.  I.e. onProgress will act like a onSuccess... ;)
                var options = { 'progressInterval': 0, 
                            'username':'foo', 
                            'password' : 'bar',
                            'connectionTimeout': 30000,
                            'BESMaxSingeChunk' : 3279}
                // Path could be one of http/https
                d.downloadFile(remotePath, localPath, onProgress, onError, options);
            }
          
            // Error handler is something goes wrong during download/upload
            function onError(error) {
                alert("error code: " + error.code + "error.description: " + error.description);
            }
          
            // Event when data is being downloaded/uploaded
            function onProgress(status ) {
                if (console && console.log) {
                    console.log("Percent complete: " + status.percent +
                        "\nTotal file size: " + status.totalFileSize +
                        "\nBytes downloaded: " + status.numBytes +
                        "\nLocal path: " + status.localPath +
                        "\nRemote path: " + status.remotePath +
                        "\nRedirect path: " + status.redirectPath + 
                        "\nStart time: " + (new Date(status.startTime)).toDateString() +
                        "\nMIME type: " + status.mimeType
                    );
                }
            }
        </script>
      </head>
      <body >
    	<button onclick="doDownload()">Download</button>
    	<button onclick="doUpload();">Upload</button>
      </body>
    </html>

