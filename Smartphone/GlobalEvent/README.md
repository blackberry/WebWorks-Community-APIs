# Global Event
This extension will allow the user listen for Global Events (given a GUID). They can also fire Global Events using this extension.

**Author:** [Brent Thornton](https://github.com/bthornton32)

## Tested On

* BlackBerry Bold 9790 v7.1.0.221
* BlackBerry Bold 9900 v7.1.0.251
* BlackBerry Bold 9900 v7.0.0.540

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/bthornton32)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.event.globalEvent**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\GlobalEvent**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\GlobalEvent**_ directory to your new _**ext\webworks.event.globalEvent**_ directory

6. Copy the downloaded _**Smartphone\GlobalEvent\webworks**_ directory to your new _**ext\webworks.event.globalEvent\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.event.globalEvent**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.event.globalEvent" />

## Summary

	//setup a listener - the myKey string gets converted to a GUID by the extension
	webworks.event.globalEvent.listen("myKey", myCallBack);
    
    //callback will fire when GUIDs match. The callback returns the key in case you have multiple listeners setup
    function myCallBack(mykey){
		debug("Global Event Occurred! Key=" + mykey);
	}
	
	//to fire the event - returns true or false depending on if the fire event was successful or not
	var myResult = webworks.event.globalEvent.fire("myKey");
	debug("Fire event: " + myResult);
	
	**NOTE: debug is just a function that outputs the results to a div on the page.

   
## Code Example

    <html>
		<head>
		<meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
		<script type="text/javascript" >
		
		//setup a listener - the myKey string gets converted to a GUID by the extension
		webworks.event.globalEvent.listen("myKey", myCallBack);
		
		function doClick(){
			var myResult = webworks.event.globalEvent.fire("myKey");
			debug("Fire event: " + myResult);
		}
		
		//callback will fire when GUIDs match. The callback returns the key in case you have multiple listeners setup
	    function myCallBack(mykey){
			debug("Global Event Occurred! Key=" + mykey);
		}

		function debug(result){
			document.getElementById('debug').innerHTML += '<br / >' + result;
		}
		</script>
		</head>
		<body >
		<h1>Hello World</h1>
		<button onclick="doClick();">Fire Event</button> 
		<div id="debug"></div>
		</body>
	</html>

## Usage Information
Pass the key as the first parameter to the listen method and the callback function as the second. This will setup a Global Listener for your key. Now any time
a Global Event is fired with your key the callback will fire. Keep in mind that the string you use for the key actually gets converted to a GUID.
To Fire a Global Event pass in the key as the first parameter. Any Global Listeners setup to listen for that key will fire.


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


