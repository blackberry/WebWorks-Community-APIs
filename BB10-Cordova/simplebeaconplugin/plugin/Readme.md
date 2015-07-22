# Bluetooth iBeacon Cordova Plugin #

This Bluetooth Smart iBeacon WebWorks Cordova Plugin (**SimpleBeaconPlugin**) is intended to allow  WebWorks applications to monitor and detect devices using the Apple de-factor iBeacon standard.

There is a document in the **/documents** folder describing the plugin's API from JavaScript.

It comes with a sample WebWorks application in the form of a simple **iBeacon Monitor** application that uses the Bluetooth iBeacon Plugin to monitor for iBeacons.

### Applies To

* [Apache Cordova for BlackBerry 10](https://github.com/blackberry/cordova-blackberry/tree/master/blackberry10)

### Author

* [John Murray](https://github.com/jcmurray)

### Release History

* **V1.0.0** - Initial release

### Known Issues

* None

### Dependencies

1. BlackBerry Device Software **10.2.1** and greater for the native component.
2. The native component of the plugin in GitHub has been built against BlackBerry Device Software **10.3.1** as this was the most recent version of BlackBerry 10 at the time of posting to GitHub.


# API Reference #

The following describes the APIs:

- Functions and associated callbacks
	- Common functions
		- **initialiseBluetooth**()
		- **terminateBluetooth**()
		- **pluginVersion**()
	- Bluetooth Smart iBeacon specific
		- **startMonitoring**(beaconCallback)
		- **stopMonitoring**()

that are used to interact with this Bluetooth Plugin. The plugin will operate with any Bluetooth Smart Beacon device that implements the iBeacon de-facto standard from Apple.

## initialiseBluetooth ##

### Name: ###

com.blackberry.community.simplebeaconplugin.initialiseBluetooth

### Description: ###

Used to instruct the plugin to connect to the Bluetooth Services on the BlackBerry 10 handset.

You must do this before using any of the other APIs for this plugin.

Once you are finished using Bluetooth in your application you should call the corresponding `com.blackberry.community.simplebeaconplugin.terminateBluetooth` function before exiting your application.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebeaconplugin.initialiseBluetooth()
```

### Arguments: ###

None

### Returns: ###

On success:

```json
{"status" : "OK", "desc" : "Bluetooth Initialised"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplebeaconplugin.initialiseBluetooth();
```

## terminateBluetooth ##

### Name: ###

com.blackberry.community.simplebeaconplugin.terminateBluetooth

### Description: ###

Used to instruct the plugin to disconnect from the Bluetooth Services on the BlackBerry 10 handset. 

After you do this you cannot use any of the other APIs for this plugin other than 

`com.blackberry.community.simplebeaconplugin.initialiseBluetooth()`.

You should always call this function when you are finished using the Bluetooth services of the plugin in your application.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebeaconplugin.terminateBluetooth()
```

### Arguments: ###

None

### Returns: ###

On success:

```json
{"status" : "OK", "desc" : "Bluetooth Terminated"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplebeaconplugin.terminateBluetooth();
```

## pluginVersion ##

### Name: ###

com.blackberry.community.simplebeaconplugin.pluginVersion

### Description: ###

Retrieves the version of the plugin being used as a string of the form: "major.minor.build".

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebeaconplugin.pluginVersion()
```

### Arguments: ###

None

### Returns: ###

"major.minor.build".

### Example Usage: ###
 
Call the function:

```javascript
var version = com.blackberry.community.simplebeaconplugin.pluginVersion();
```

## startMonitoring ##

### Name: ###

com.blackberry.community.simplebeaconplugin.startMonitoring

### Description: ###

Used to start monitoring for iBeacon advertisement packes and to return any such events via a callback.

Success or failure to initiate monitoring is indicated by a status returned by the function.

Once you are finished monitoring and wish to stop receiving events, you should use the `com.blackberry.community.simplebeaconplugin.stopMonitoring()` function.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebeaconplugin.startMonitoring(beaconCallback)
```

### Arguments: ###

- ***beaconCallback*** -- A function that will be called when an iBeacon advertisment is detected.

### Returns: ###

An immediate status is returned when the function is called.  

#### Immediate status returned by function: ####

On success:

```json
{"status" : "OK", "desc" : "Monititoring started OK"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

#### Results returned by callback: ####


The iBeacon data from the device will be presented as in this example:


```json
{
"status":"OK",
"event":"IBEACON",
"data":
	{
		"uuid": "CCBF9400E2C011E48A001681E6B88EC1",
		"major": "1",
		"minor": "1",
		"rssi": "-84",
		"txpower": "-64"
},
"desc":"iBeacon event"
}
```

This represents an iBeacon event with the three identification fields of **uuid**, **major** number and **minor** number. The fields **rssi** represents the received power in dBm whilst the **txpower** field is the transmitted power in dBm as measured at 1 metre from the beacon assuming an isotropic antenna. Together **rssi** and **txpower** can be used (**rssi** - **txpower**) to calculate the channel path power loss in dB. This in turn cal be used to calculate the beacon's distance since we know the power at 1 metre and the power level diminishes as 1/r^2.

Following the convention notifications that are associated with an error condition will be identified by `"status": "ERROR"` in the JSON object.

### Example Usage: ###
 
Call the function:

```javascript
var beaconCallback = function(data) {
	var json = JSON.parse(data);
	if (json.status === 'OK') {
	...
	}
};
var foo = com.blackberry.community.simplebeaconplugin.startMonitoring(beaconCallback);
```

## stopMonitoring ##

### Name: ###

com.blackberry.community.simplebeaconplugin.stopMonitoring

### Description: ###

Used to instruct the plugin to stop minitoring iBeacon advertisements.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebeaconplugin.stopMonitoring()
```

### Arguments: ###

None

### Returns: ###

On success:

```json
{"status" : "OK", "desc" : "Stopped monitoring for iBeacons OK"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplebeaconplugin.stopMonitoring();
```

## Contributing Changes

Please see the [README](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/README.md) of the WebWorks-Community-APIs repository for instructions on how to add new Samples or make modifications to existing Samples.

## Bug Reporting and Feature Requests

If you find a bug in a Sample, or have an enhancement request, simply file an [Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) for the Sample and send a message (via github messages) to the Sample Author(s) to let them know that you have filed an [Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues).


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.