# Bluetooth SPP WebWorks Plugin #


This Bluetooth SPP WebWorks Plugin (**SimpleBtSppPlugin**) is intended to allow  WebWorks applications to connect to devices that uses the Bluetooth SPP profile.

There is a document in the **/documents** folder describing the plugin's API from JavaScript.

It comes with a sample WebWorks application in the form of a simple **Chat** application that uses SPP to communicate between two BlackBerry 10 devices.

###Applies To

* [Apache Cordova for BlackBerry 10](https://github.com/blackberry/cordova-blackberry/tree/master/blackberry10)

###Author

* [John Murray](https://github.com/jcmurray)

###Release History

* **V1.2.1** - Bugfix
	* Buffer overrun in inbound messages
* **V1.2.0** - Addition of **sppBufferSize** API
* **V1.1.1** - Bugfix
	* Buffer overrun in message logging
* **V1.1.0** - Support for bt\_spp\_open\_ex() and bt\_spp\_open\_server\_ex() in **10.3.0** and above allowing access to the RFCOMM API.
* **V1.0.1** - Minor bugs fixed in plugin
* **V1.0.0** - Initial release

###Known Issues

* None

###Dependencies

1. BlackBerry Device Software **10.2.1** and greater for the native component.
2. BlackBerry Device Software **10.3.0** and above is required for access to the RFCOMM API. 
1. BlackBerry WebWorks **2.1** and greater.

###Required Plugins

None.

## The Plugin ##

[API Reference](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10-Cordova/SimpleBtSppPlugin/documents/API-Reference.md)

## Bug Reporting and Feature Requests

If you find a bug in a Sample, or have an enhancement request, simply file an [Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) for the Sample and send a message (via github messages) to the Sample Author(s) to let them know that you have filed an [Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues).


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.