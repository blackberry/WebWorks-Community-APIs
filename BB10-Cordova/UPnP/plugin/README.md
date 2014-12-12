uPnP Extention
==============

This BlackBerry 10 WebWorks extension provides additional APIs supplying information about Universal Plug and Play devices available over WiFi.

** Tested On **

BlackBerry Z10 10.3.1.634

** Author **

	[Peardox] (http://supportforums.blackberry.com/t5/user/viewprofilepage/user-id/325249)
	[Junjie Huang] (https://github.com/umcsdon)

## Version History

	1.0.0 Initial Release

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.upnp

or,
	
	webworks plugin add com.blackberry.community.upnp

Installation from source is the same but instead of the id ("com.blackberry.community.upnp"), use the file system path to the source plugin folder.

Note that it is also required that you give your app the disabled websecurity feature. This is as the extension needs to talk to any and all uPnP devices on your WiFi network. The websecurity feature is a bit black and white in this respect, all or nothing...

```xml
	<preference name="websecurity" value="false" />
```

## The Extension API
The uPnP Extension provides the following API:

```javascript
	var result = community.upnp.discover(timeout [optional], devtype [optional]);
	
	/*
	Result is a javascript object with the following properties

	success,      	// [Bool] 	- Did it work?
	timeout,    	// [Int] 	- Timeout used
	devtype,  		// [String] - Device Type searched for
	devcount,  		// [Int] 	- Number of devices found
	device			// [Array] 	- An array of device objects
		url			// [String]	- Announce URL from attached device
		xml			// [String]	- XML documents returned from attached device
	*/
```

The parameters to discover are as follows...

timeout : How long to wait for a reply in ms (default - 1000)
devtype : Device type to search for (default - upnp:rootdevice). This is the ST field of the M-SEARCH query.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
