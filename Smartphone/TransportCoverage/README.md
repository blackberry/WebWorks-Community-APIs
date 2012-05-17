# Transport Coverage
This extension is used to check the availability and coverage of a specific transport on the BlackBerry. 

In order to successfully establish a connection, the desired transport has to be both available (i.e. the device must be capable of supporting that transport), 
and there must be sufficient coverage for the transport in question. Remember to still check for a proper response code from server as these methods only test 
client side transport availability/connectivity. Ex you could have MDS available with coverage, but the BES could have MDS turned off at that time.

**Author:** [Brent Thornton](https://github.com/bthornton32)

## Tested On

* BlackBerry Bold 9790 v7.1.0.221
* BlackBerry Bold 9900 v7.1.0.377
* BlackBerry Bold 9900 v7.0.0.540

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/bthornton32)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**blackberry.transportCoverage**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\TransportCoverage**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\TransportCoverage**_ directory to your new _**ext\blackberry.transportCoverage**_ directory

6. Copy the downloaded _**Smartphone\TransportCoverage\blackberry**_ directory to your new _**ext\blackberry.transportCoverage\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\blackberry.transportCoverage**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.transportCoverage" />

## Summary

	//Possible Transport Strings to Pass as Input:
	//TRANSPORT_MDS
	//TRANSPORT_BIS_B
	//TRANSPORT_TCP_CELLULAR
	//TRANSPORT_TCP_WIFI
	//TRANSPORT_WAP
	//TRANSPORT_WAP2

	var rval = blackberry.transportCoverage.getTransportDetails("TRANSPORT_MDS");
    alert("Transport: " + rval.transport + " Available: " + rval.available+ ", Coverage: " +  rval.coverage);
   
## Code Example

    <html>
		<head>
		<meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
		<script type="text/javascript" >
		function doClick(){
			var rval = blackberry.transportCoverage.getTransportDetails("TRANSPORT_MDS");
		    debug(rval.transport + " Available: " + rval.available+ ", Coverage: " +  rval.coverage);
		
		    var rval2 = blackberry.transportCoverage.getTransportDetails("TRANSPORT_BIS_B");
		    debug(rval2.transport + " Available: " + rval2.available+ ", Coverage: " +  rval2.coverage);
		
		    var rval3 = blackberry.transportCoverage.getTransportDetails("TRANSPORT_TCP_CELLULAR");
		    debug(rval3.transport + " Available: " + rval3.available+ ", Coverage: " +  rval3.coverage);
		
		    var rval4 = blackberry.transportCoverage.getTransportDetails("TRANSPORT_TCP_WIFI");
		    debug(rval4.transport + " Available: " + rval4.available+ ", Coverage: " +  rval4.coverage);
		
		    var rval5 = blackberry.transportCoverage.getTransportDetails("TRANSPORT_WAP");
		    debug(rval5.transport + " Available: " + rval5.available+ ", Coverage: " +  rval5.coverage);
		
		    var rval6 = blackberry.transportCoverage.getTransportDetails("TRANSPORT_WAP2");
		    debug(rval6.transport + " Available: " + rval6.available+ ", Coverage: " +  rval6.coverage);
		}


		function debug(result){
			document.getElementById('debug').innerHTML += '<br / >' + result;
		}

		</script>
		</head>
		<body >
		<h1>Hello World</h1>
		<button onclick="doClick();">Test Transports</button> 
		<div id="debug"></div>
		</body>
	</html>

## Usage Information
Pass the transport string as the first parameter to the getTransportDetails method. An object with the attributes transport, available, and coverage will be returned.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


