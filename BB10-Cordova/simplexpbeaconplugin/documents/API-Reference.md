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

		

