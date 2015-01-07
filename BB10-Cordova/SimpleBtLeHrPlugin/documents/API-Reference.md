# API Reference #

The following describes the APIs:

- Functions and associated callbacks
	- Common functions
		- **initialiseBluetooth**()
		- **terminateBluetooth**()
		- **startDeviceScan**(scanCallback)
		- **pluginVersion**()
	- Bluetooth Smart GATT Heart Rate Profile specific
		- **startMonitoringHr**(notifcationCallback)
		- **stopMonitoringHr**()
- Properties
	- Common properties
		- **bluetoothAddress**

that are used to interact with this Bluetooth Plugin. The plugin will operate with any Bluetooth Smart device that implements the [Bluetooth Smart Heart Rate Service](https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.heart_rate.xml) and interprets the [Bluetooth Smart Heart Rate Measurement Characteristic](https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml) presented by the device on notifications.

## bluetoothAddress ##

### Name: ###

com.blackberry.community.simplebtlehrplugin.bluetoothAddress

### Description: ###

Used to set or retrieve the Bluetooth address that identifies the device to which to connect.

### Type: ###

Property

### Values: ###

- A Bluetooth Address as a string such as: '34:BB:1F:3E:77:BC'


### Example Usage: ###

Set the Bluetooth address to '34:BB:1F:3E:77:BC':

```javascript
com.blackberry.community.simplebtlehrplugin.bluetoothAddress = '34:BB:1F:3E:77:BC';
```

Get the Bluetooth Address:

```javascript
var btAddress;
btAddress = com.blackberry.community.simplebtlehrplugin.bluetoothAddress;
```

## initialiseBluetooth ##

### Name: ###

com.blackberry.community.simplebtlehrplugin.initialiseBluetooth

### Description: ###

Used to instruct the plugin to connect to the Bluetooth Services on the BlackBerry 10 handset.

You must do this before using any of the other APIs for this plugin.

Once you are finished using Bluetooth in your application you should call the corresponding `com.blackberry.community.simplebtlehrplugin.terminateBluetooth` function before exiting your application.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtlehrplugin.initialiseBluetooth()
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
var foo = com.blackberry.community.simplebtlehrplugin.initialiseBluetooth();
```

## terminateBluetooth ##

### Name: ###

com.blackberry.community.simplebtlehrplugin.terminateBluetooth

### Description: ###

Used to instruct the plugin to disconnect from the Bluetooth Services on the BlackBerry 10 handset. 

After you do this you cannot use any of the other APIs for this plugin other than 

`com.blackberry.community.simplebtlehrplugin.initialiseBluetooth()`.

You should always call this function when you are finished using the Bluetooth services of the plugin in your application.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtlehrplugin.terminateBluetooth()
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
var foo = com.blackberry.community.simplebtlehrplugin.initialiseBluetooth();
```

## startDeviceScan ##

### Name: ###

com.blackberry.community.simplebtlehrplugin.startDeviceScan

### Description: ###

Used to search for candidate Bluetooth devices in the neighbourhood of the BlackBerry 10 handset. Candidate devices are returned asynchronously though a callback. Success or failure to start the scan is indicated by a status returned by the function. The scan will report only those devices that have been found and which support the Bluetooth Smart Heart Rate Monitor Service.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtlehrplugin.startDeviceScan(scanCallback)
```

### Arguments: ###

- ***scanCallback*** -- A function that will be called when the scan completes.

### Returns: ###

And immediate status is returned when the function is called and candidate devices are returned via the callback.  

#### Immediate status returned by function: ####

On success:

```json
{"status" : "OK", "desc" : "Bluetooth Device Scan Started"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

#### Results returned by callback: ####

The list of devices will be presented as in this example:

```json
{
"desc":"List of candidate devices",
"devices":[
	{"address":"34:BB:1F:3E:77:BC","name":"BLACKBERRY-612D"},
	{"address":"94:EB:CD:64:A5:31","name":"BLACKBERRY-B345"},
	{"address":"00:18:31:F0:F4:2D","name":"Zephyr HXM00002245"}
],
"status":"OK"
} 
```

If no devices are found then this will returned:

```json
{
"desc":"List of candidate devices",
"devices":[],
"status":"OK"
}
```

### Example Usage: ###
 
Call the function:

```javascript
var scanCallback = function(data) {
	var json = JSON.parse(data);
	if (json.status === 'OK') {
	...
	}
};

var foo = com.blackberry.community.simplebtlehrplugin.startDeviceScan(scanCallback);
```

## pluginVersion ##

### Name: ###

com.blackberry.community.simplebtlehrplugin.pluginVersion

### Description: ###

Retrieves the version of the plugin being used as a string of the form: "major.minor.build".

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtlehrplugin.pluginVersion()
```

### Arguments: ###

None

### Returns: ###

"major.minor.build".

### Example Usage: ###
 
Call the function:

```javascript
var version = com.blackberry.community.simplebtlehrplugin.pluginVersion();
```

## startMonitoringHr ##

### Name: ###

com.blackberry.community.simplebtlehrplugin.startMonitoringHr

### Description: ###

Used to connect to a **Bluetooth Low Energy** (**Smart**) device supporting the **GATT Heart Rate Service** and request that the device start sending Notifications of changes in the associated **GATT Heart Rate Characteristic** via a callback.

Success or failure to initiate monitoring via the appropriate GATT service on the device is indicated by a status returned by the function.

Once you are finished using a device and wish to stop receiving notifications, you should use the `com.blackberry.community.simplebtlehrplugin.stopMonitoringHr()` function.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtlehrplugin.startMonitoringHr(gattCallback)
```

### Arguments: ###

- ***gattCallback*** -- A function that will be called when data is available from the device.

### Returns: ###

And immediate status is returned when the function is called.  

#### Immediate status returned by function: ####

On success:

```json
{"status" : "OK", "desc" : "Requested connection to Heart Rate service OK"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

#### Results returned by callback: ####

Prior to receiving Heart Rate Data notifications you will see a connection event as shown by this example confirming that the registreation for notifications has been successful.

```json
{
"status":"OK"
"event":"CONNECTION",
"desc":"Registered for notifications",
}
```

The Heart Rate data from the device will be presented as in this example:


```json
{
"status":"OK",
"event":"HRDATA",
"data":
	{
		"energy_expended_feature_supported":"false",
		"flags":22,
		"heart_rate":65,
		"hr_data_format_8":"true",
		"rr_interval_data_present":"true",
		"rr_intervals":[6224,18951,0],
		"sensor_contact_detected":true,
		"sensor_contact_feature_supported":"true"
},
"desc":"Heart Rate Device data"
}
```

This represents a GATT Notify event from the GATT Heart Rate Service (UUID **"0x180D"**) for the Heart Rate Measurement Characteristic (UUID **"0x2A37"**). These fields are described here: [Heart Rate Measurement Characteristic](https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml)

If the remote device unilaterally terminates the Bluetooth connection the following message will be delivered via the callback:

```json
{
	"status":"OK",
	"event":"DISCONNECTION",
	"desc":"Heart Rate Service Disconnected"
}
```

Following the convention notifications that are associated with an error condition will be identified by `"status": "ERROR"` in the JSON object.

### Example Usage: ###
 
Call the function:

```javascript
var gattCallback = function(data) {
	var json = JSON.parse(data);
	if (json.status === 'OK') {
	...
	}
};

var foo = com.blackberry.community.simplebtlehrplugin.startMonitoringHr(gattCallback);
```

## stopMonitoringHr ##

### Name: ###

com.blackberry.community.simplebtlehrplugin.stopMonitoringHr

### Description: ###

Used to instruct the plugin to tell a **Bluetooth Low Energy** (**Smart**) device supporting the **GATT Heart Rate Service** to stop sending Notifications of changes in the associated **GATT Heart Rate Characteristic** via a callback and to disconnect from that service on the device.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtlehrplugin.stopMonitoringHr()
```

### Arguments: ###

None

### Returns: ###

On success:

```json
{"status" : "OK", "desc" : "GATT disconnect from Heart Rate service request OK"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplebtlehrplugin.stopMonitoringHr();
```

		

