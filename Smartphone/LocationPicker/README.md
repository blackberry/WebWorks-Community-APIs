# Location Picker
This extension will allow the user to select a point on the map and get the latitude and longitude result. This extension uses an embedded BlackBerry MapField that automatically gets the 
users current location. The point of the extension is to allow users to select a more accurate location if the current location isn't accurate enough.

**Author:** [Brent Thornton](https://github.com/bthornton32) – Special thanks to Rob Watt and Mark Trolley for their help with the MapField.

## Tested On

* BlackBerry Bold 9790 v7.1.0.221
* BlackBerry Bold 9900 v7.1.0.251
* BlackBerry Bold 9900 v7.0.0.540

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/bthornton32)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**blackberry.ui.locationPicker**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\LocationPicker**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\LocationPicker**_ directory to your new _**ext\blackberry.ui.locationPicker**_ directory

6. Copy the downloaded _**Smartphone\LocationPicker\blackberry**_ directory to your new _**ext\blackberry.ui.locationPicker\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\blackberry.ui.locationPicker**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.ui.locationPicker" />

## Summary

	//if a location is found these options are ignored, these values are fall backs.
    var options = {	
        lat: '27.718',
        lon: '-114.829',
        zoom: '4'
    };
    //call open with a default location in case geolocate doesn't work
    blackberry.ui.locationPicker.open(myCallBack, options);

   
## Code Example

    <html>
		<head>
		<meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
		<script type="text/javascript" >
		function doClick(){
			//inLocationPicker = true;
			alert('Select a point on the map and click the back button to return to this screen.');
			var options = {	//if a location is found this is ignored, these values are fall backs. 
		        lat: '27.718',
		        lon: '-114.829',
		        zoom: '4'
		    };
		    //call open with a default location in case geolocate doesn't work
		    blackberry.ui.locationPicker.open(myCallBack, options);
		    
		    //call open with no default location
			//blackberry.ui.locationPicker.open(myCallBack);
		}


		function myCallBack(result){
			document.getElementById('debug').innerHTML = 'result: ' + result;
		}
		</script>
		</head>
		<body >
		<h1>Hello World</h1>
		<button onclick="doClick();">Pick Location</button> 
		<div id="debug"></div>
		</body>
	</html>

## Usage Information
Pass the callback as the first parameter to the open method. You can pass the default location as the second parameter to the open method or null. If you set to null the default location if no GPS location is found is Ottawa.
Once the map is shown the user can click the back key and their current location will be selected, if they pick another location on the map those coordinates are returned.
Result will be a JSON formatted string sent to your callback function. Ex. {"latitude":"43.5069","longitude":"-80.54768"}


_**NOTE:**_ The callback is handled asynchronously, so code that is placed directly after
the "open" function call will be executed immediately while waiting for the user's 
response input.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


