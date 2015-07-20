# API Reference #

The following describes the APIs:

- Functions and associated callbacks
	- Common functions
		- **initialiseBluetooth**(initialiseCallback)
		- **terminateBluetooth**(terminateCallback)
		- **pluginVersion**(versionCallback)
	- Bluetooth Smart iBeacon specific
		- **startMonitoring**(beaconCallback)
		- **stopMonitoring**(stopMonitoringCallback)
		- **addBeaconUuidToMonitor**(UUID, addBeaconUuidToMonitorCallback)
		- **removeBeaconUuidToMonitor**(UUID, removeBeaconUuidToMonitorCallback)

that are used to interact with this Bluetooth Plugin. The plugin will operate with any Bluetooth Smart Beacon device that implements the iBeacon de-facto standard from Apple.

## initialiseBluetooth ##

### Name: ###

com.blackberry.community.simplexpbeaconplugin.initialiseBluetooth

### Description: ###

Used to instruct the plugin to connect to the Bluetooth Services on the BlackBerry 10 handset.

You must do this before using any of the other APIs for this plugin.

Once you are finished using Bluetooth in your application you should call the corresponding `com.blackberry.community.simplexpbeaconplugin.terminateBluetooth` function before exiting your application.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplexpbeaconplugin.initialiseBluetooth(initialiseCallback)
```

### Arguments: ###

- ***initialiseCallback*** -- A function that will be called when the status of the Bluetooth initialisation request is available.

### Returns: ###

#### Immediate status returned by function: ####

None.

#### Results returned by callback: ####

On success:

```json
{
	"status" : "OK",
	"desc" : "Bluetooth Initialised"
}
```

On failure:

```json
{
	"status" : "ERROR",
	"desc" : "<description of failure>",
	"error_code" : <number>
}
```

### Example Usage: ###
 
Call the function:

```javascript
com.blackberry.community.simplexpbeaconplugin.initialiseBluetooth(
	function(data) {
		var json = JSON.parse(data);
		if (json.status === 'OK') {
		...
		}
	}
);
```

## terminateBluetooth ##

### Name: ###

com.blackberry.community.simplexpbeaconplugin.terminateBluetooth

### Description: ###

Used to instruct the plugin to disconnect from the Bluetooth Services on the BlackBerry 10 handset. 

After you do this you cannot use any of the other APIs for this plugin other than 

`com.blackberry.community.simplexpbeaconplugin.initialiseBluetooth()`.

You should always call this function when you are finished using the Bluetooth services of the plugin in your application.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplexpbeaconplugin.terminateBluetooth(terminateCallback)
```

### Arguments: ###

- ***terminateCallback*** -- A function that will be called when the status of the Bluetooth terminate request is available.

### Returns: ###

#### Immediate status returned by function: ####

None.

#### Results returned by callback: ####

On success:

```json
{
	"status" : "OK",
	"desc" : "Bluetooth Terminated"
}
```

On failure:

```json
{
	"status" : "ERROR",
	"desc" : "<description of failure>",
	"error_code" : <number>
}
```

### Example Usage: ###
 
Call the function:

```javascript
com.blackberry.community.simplexpbeaconplugin.terminateBluetooth(
	function(data) {
		var json = JSON.parse(data);
		if (json.status === 'OK') {
		...
		}
	}
);
```

## pluginVersion ##

### Name: ###

com.blackberry.community.simplexpbeaconplugin.pluginVersion

### Description: ###

Retrieves the version of the plugin being used as a string of the form: "major.minor.build".

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplexpbeaconplugin.pluginVersion(versionCallback)
```

### Arguments: ###

None

### Returns: ###

#### Immediate status returned by function: ####

None.

#### Results returned by callback: ####

```json
{
	"desc":"Plugin Version",
	"plugin_version":"major.minor.build",
	"status":"OK"
}
```

### Example Usage: ###
 
Call the function:

```javascript
var version = com.blackberry.community.simplexpbeaconplugin.pluginVersion(
	function(data) {
		var json = JSON.parse(data);
		if (json.status === 'OK') {
		...
		}
	}
);
```

## startMonitoring ##

### Name: ###

com.blackberry.community.simplexpbeaconplugin.startMonitoring

### Description: ###

Used to start monitoring for iBeacon advertisement packes and to return any such events via a callback.

Once you are finished monitoring and wish to stop receiving events, you should use the `com.blackberry.community.simplexpbeaconplugin.stopMonitoring()` function.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplexpbeaconplugin.startMonitoring(beaconCallback)
```

### Arguments: ###

- ***beaconCallback*** -- A function that will be called when an iBeacon advertisment is detected.

### Returns: ###

#### Immediate status returned by function: ####

None.

#### Results returned by callback: ####

On success:

```json
{
	"status" : "OK", 
	"event":"STARTED",
	"desc" : "Requested iBeacon Monitoring OK"
}
```

On failure:

```json
{
	"status" : "ERROR",
	"event":"STARTED",
	"desc" : "<description of failure>",
	"error_code" : <number>
}
```

The iBeacon data from the device will be presented either as in this example (for **BlackBerry 10** or **Android**):

```json
{
"status":"OK",
"event":"IBEACON",
"data":
	{
		"uuid": "8AEFB031-6C32-486F-825B-E26FA193487D",
		"major": "1",
		"minor": "1",
		"rssi": "-84",
		"txpower": "-64"
},
"desc":"iBeacon event"
}
```

Or this example (for **iOS**):

```json
{
"status":"OK",
"event":"IBEACON_PROXIMITY",
"data":
	{
		"uuid": "8AEFB031-6C32-486F-825B-E26FA193487D",
		"major": "1",
		"minor": "1",
		"rssi": "-84",
		"proximity": "2",
		"accuracy": "3.14159"
},
"desc":"iBeacon proximity event"
}
```

This represents an iBeacon event with the three identification fields of **uuid**, **major** number and **minor** number. The fields **rssi** represents the received power in dBm whilst the **txpower** field is the transmitted power in dBm as measured at 1 metre from the beacon assuming an isotropic antenna. Together **rssi** and **txpower** can be used (**rssi** - **txpower**) to calculate the channel path power loss in dB. This in turn cal be used to calculate the beacon's distance since we know the power at 1 metre and the power level diminishes as 1/r^2.

The reason there are two formats (**IBEACON** and **IBEACON_PROXIMITY**) is that **iOS** does not report on the **Transmitted Power** of the iBeacon as BlackBerry 10 and Android do. Rather, it reports **accuracy** which is a calculated measure of the distance of the iBeacon in metres, and **proximity** which is an integer {0, 1, ... ,4} which gives a sense of the proximity ( near to far away ).

Following the convention notifications that are associated with an error condition will be identified by `"status": "ERROR"` in the JSON object.

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplexpbeaconplugin.startMonitoring(
	function(data) {
		var json = JSON.parse(data);
		if (json.status === 'OK') {
		...
		}
	}
);
```

## stopMonitoring ##

### Name: ###

com.blackberry.community.simplexpbeaconplugin.stopMonitoring

### Description: ###

Used to instruct the plugin to stop minitoring iBeacon advertisements.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplexpbeaconplugin.stopMonitoring(stopMonitoringCallback)
```

### Arguments: ###

- ***stopMonitoringCallback*** -- A function that will be called when the status of the stop monitoring  request is available.

### Returns: ###

On success:

```json
{
	"status" : "OK", 
	"desc" : "Stopped monitoring for iBeacons OK"
}
```

On failure:

```json
{
	"status" : "ERROR",
	"desc" : "<description of failure>",
	"error_code" : <number>
}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplexpbeaconplugin.stopMonitoring(
	function(data) {
		var json = JSON.parse(data);
		if (json.status === 'OK') {
		...
		}
	}
);
```

## addBeaconUuidToMonitor ##

### Name: ###

com.blackberry.community.simplexpbeaconplugin.addBeaconUuidToMonitor

### Description: ###

Used to instruct the plugin to monitor for specific iBeacon UUIDs.

This APIs is really to help bridge a difference in behaviour between iOS and both BlackBerry 10 and Android. BlackBerry 10 and Android will report on iBeacon events from iBeacons closeby. On the other hand iOS **must** be told to monitor what are termed *Beacon Regions* ( identified by either UUID, Major or Minor numbers ). Without specifying such a pattern to match iOS will not respond with any iBeacon events.

So, using this API we can accomodate an approximation of this behaviour by allowing the caller to specify the UUID os an iBeacon that should be monitored for on each platfom (Android, BlackBerry 10 and iOS).

This API can be called many times with different **UUID**s to allow collections of iBeacons to be monitored.

If this API is not called in your application and the list of UUIDs for monitoring is empty then iOS will report **no** iBeacon events whilst BlackBerry 10 and Android will report **all** iBeacon events.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplexpbeaconplugin.addBeaconUuidToMonitor(uuid, addBeaconUuidToMonitorCallback)
```

### Arguments: ###

- **uuid** -- a string representation of a UUID of an iBeacon that should be filtered upon ( e.g. *"8AEFB031-6C32-486F-825B-E26FA193487D"*).
- ***addBeaconUuidToMonitorCallback*** -- A function that will be called when the status of the add beacon uuid request is available.

### Returns: ###

On success:

```json
{
	"status" : "OK", 
	"desc" : "UUID added"
}
```

On failure:

```json
{
	"status" : "ERROR",
	"desc" : "<description of failure>",
	"error_code" : <number>
}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplexpbeaconplugin.addBeaconUuidToMonitor(
	"8AEFB031-6C32-486F-825B-E26FA193487D",
	function(data) {
		var json = JSON.parse(data);
		if (json.status === 'OK') {
		...
		}
	}
);
```

## removeBeaconUuidToMonitor ##

### Name: ###

com.blackberry.community.simplexpbeaconplugin.removeBeaconUuidToMonitor

### Description: ###

The converse operation to **addBeaconUuidToMonitor** where a UUID is removed from the list of iBeacons to be monitored.

If the last uuid is removed from the list the default iOS, BlackBerry 10 and Android behaviours that occur when no UUID filter has been specified will be re-instated.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplexpbeaconplugin.removeBeaconUuidToMonitor(uuid, removeBeaconUuidToMonitorCallback)
```

### Arguments: ###

- **uuid** -- a string representation of a UUID of an iBeacon that should be removed from the filtering list ( e.g. *"8AEFB031-6C32-486F-825B-E26FA193487D"*).
- ***removeBeaconUuidToMonitorCallback*** -- A function that will be called when the status of the remove beacon uuid request is available.

### Returns: ###

On success:

```json
{
	"status" : "OK", 
	"desc" : "iBeacon UUID removed sucessfully"
}
```

On failure:

```json
{
	"status" : "ERROR",
	"desc" : "<description of failure>",
	"error_code" : <number>
}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplexpbeaconplugin.removeBeaconUuidToMonitor(
	"8AEFB031-6C32-486F-825B-E26FA193487D",
	function(data) {
		var json = JSON.parse(data);
		if (json.status === 'OK') {
		...
		}
	}
);
```

		
		

