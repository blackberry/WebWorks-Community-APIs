# Persistent Storage
This extension will allow the user to make use of the persistent storage on the decive. Store and Retrieve data using a GUID. Share data between apps.

**Author:**
[John Mutter](https://github.com/jmutter)

## Tested On

* BlackBerry Bold 9790 v7.1.0.221
* BlackBerry Bold 9900 v7.1.0.251
* BlackBerry Bold 9900 v7.0.0.540

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/jmutter)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.persistent.storage**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Storage**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Storage**_ directory to your new _**ext\webworks.persistent.storage**_ directory

6. Copy the downloaded _**Smartphone\storage\webworks**_ directory to your new _**ext\webworks.persistent.storage\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.persistent.storage**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.persistent.storage" />

## Summary

	//Write to storage
	webworks.persistent.storage.write("mykey", "Some text to be saved");

	//Read from storage
	var myResult = webworks.persistent.storage.read("mykey");
	
	//Delete from storage
	webworks.persistent.storage.delete("mykey");

## Code Example

    <html>
		<head>
		<meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
		<script type="text/javascript" >
		function doWrite(){
			webworks.persistent.storage.write("mykey", "Some text to be saved");
		}


		function doRead(){
			var myResult = webworks.persistent.storage.read("mykey");
			alert(myResult);
		}
		
		function doDelete(){
			webworks.persistent.storage.delete("mykey");
			alert('Persistent Data Destroyed');
		}
		</script>
		</head>
		<body>
		<h1>Hello World</h1>
		<button onclick="doWrite();">Test Write</button>
		<button onclick="doRead();">Test Read</button>
		<button onclick="doDelete();">Test Delete</button>
		</body>
	</html>

## Usage Information
Pass the key and the string you want to store to the write method. A True/False value is returned indicating a successful/unsuccessful write.
Pass the key to the read function to get the string saved in persistent storage as the return value.


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


