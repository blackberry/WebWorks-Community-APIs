# API Reference #

The following describes the APIs:

- Functions and associated callbacks
	- Common functions
		- **initialiseBluetooth**()
		- **terminateBluetooth**()
		- **startDeviceScan**(scanCallback)
		- **pluginVersion**()
	- Bluetooth SPP specific
		- SPP Client mode (handset connects to device)
			- **connectToDevice**(dataCallback)
		- SPP Server mode (device connects to handset)
			- **listenForConnection**(inboundConnectionCallback)
			- **stopListening**()
		- SPP Common functions
			- **disconnectFromDevice**()
			- **sendMessage**(message)
- Properties
	- Common properties
		- **bluetoothAddress**
		- **sppServiceUuid**
		- **sppBufferSize**
	- SPP **RFCOMM** API ( available in **10.3.0** and above )
		- **sppRfcommMode**
		- **sppRfcommServicePort**

that are used to interact with this Bluetooth Plugin.

## bluetoothAddress ##

### Name: ###

com.blackberry.community.simplebtsppplugin.bluetoothAddress

### Description: ###

Used to set or retrieve the Bluetooth address that identifies the device to which to connect.

### Type: ###

Property

### Values: ###

- A Bluetooth Address as a string such as: '34:BB:1F:3E:77:BC'


### Example Usage: ###

Set the Bluetooth address to '34:BB:1F:3E:77:BC':

```javascript
com.blackberry.community.simplebtsppplugin.bluetoothAddress = '34:BB:1F:3E:77:BC';
```

Get the Bluetooth Address:

```javascript
var btAddress;
btAddress = com.blackberry.community.simplebtsppplugin.bluetoothAddress;
```

## initialiseBluetooth ##

### Name: ###

com.blackberry.community.simplebtsppplugin.initialiseBluetooth

### Description: ###

Used to instruct the plugin to connect to the Bluetooth Services on the BlackBerry 10 handset.

You must do this before using any of the other APIs for this plugin.

Once you are finished using Bluetooth in your application you should call the corresponding `com.blackberry.community.simplebtsppplugin.terminateBluetooth` function before exiting your application.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.initialiseBluetooth()
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
var foo = com.blackberry.community.simplebtsppplugin.initialiseBluetooth();
```

## terminateBluetooth ##

### Name: ###

com.blackberry.community.simplebtsppplugin.terminateBluetooth

### Description: ###

Used to instruct the plugin to disconnect from the Bluetooth Services on the BlackBerry 10 handset. 

After you do this you cannot use any of the other APIs for this plugin other than `com.blackberry.community.simplebtsppplugin.initialiseBluetooth()`.

You should always call this function when you are finished using the Bluetooth services of the plugin in your application.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.terminateBluetooth()
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
var foo = com.blackberry.community.simplebtsppplugin.initialiseBluetooth();
```

## startDeviceScan ##

### Name: ###

com.blackberry.community.simplebtsppplugin.startDeviceScan

### Description: ###

Used to search for candidate Bluetooth devices in the neighbourhood of the BlackBerry 10 handset. Candidate devices are returned asynchronously though a callback. Success or failure to start the scan is indicated by a status returned by the function.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.startDeviceScan(scanCallback)
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
	{"address":"F8:BC:FC:8C:C6:10","name":"estimote"},
	{"address":"94:EB:CD:64:A5:31","name":"BLACKBERRY-B345"},
	{"address":"CE:5A:65:32:9A:61","name":"estimote"}
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

var foo = com.blackberry.community.simplebtsppplugin.startDeviceScan(scanCallback);
```

## connectToDevice ##

### Name: ###

com.blackberry.community.simplebtsppplugin.connectToDevice

### Description: ###

Used to connect to an SPP Bluetooth device and start data from the device being received via a callback. 

Success or failure to connect to the device is indicated by a status returned by the function.

Once you are finished using a device and wish to disconnect you should use the corresponding `com.blackberry.community.simplebtsppplugin.disconnectFromDevice()` function.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.connectToDevice(dataCallback)
```

### Arguments: ###

- ***dataCallback*** -- A function that will be called when the data is available from the device.

### Returns: ###

And immediate status is returned when the function is called.  

#### Immediate status returned by function: ####

On success:

```json
{"status" : "OK", "desc" : "Bluetooth Connection Thread Started"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

If the error code is **11** then this generally indicates a temporary resource-busy condition and you should be able to retry the connection request a short time later (a second or two). This is usually precipitated when a connection is terminated and a new connection attempt is made almost immediately to the same device and service. It's probably a good idea, in general, to wait for a second or two before reconnecting in this way.

#### Results returned by callback: ####


Messages will be parsed and delivered in **RAW** format something like this example:

```json
{
	"status":"OK",
	"format":"RAW",
	"desc":"Device data",
	"data":	[2, 244, 7, 5, 57, 53, 54, 48, 0, 217, 3]
}
```

If the remote device unilaterally terminates the Bluetooth connection the following message will be delivered via the callback:

```json
{
	"status":"OK",
	"event":"DISCONNECTION",
	"desc":"Connection terminated - <error description>"
}
```

### Example Usage: ###
 
Call the function:

```javascript
var dataCallback = function(data) {
	var json = JSON.parse(data);
	if (json.status === 'OK') {
	...
	}
};

var foo = com.blackberry.community.simplebtsppplugin.connectToDevice(dataCallback);
```

## listenForConnection ##

### Name: ###

com.blackberry.community.simplebtsppplugin.listenForConnection

### Description: ###

Used to listen for an inbound connection from an SPP Bluetooth device and start data from the device being received via a callback once a device connects.

Success or failure to connect to the device is indicated by a status returned by the function.

If you wish to stop listening for inbound connections you should use the corresponding `com.blackberry.community.simplebtsppplugin.stopListening()` function.

Once an inbound connection has been established it's treated just like an ordinary SPP connection and, once you are finished using a device and wish to disconnect, you should use the `com.blackberry.community.simplebtsppplugin.disconnectFromDevice()` function.

### Type: ###

Asynchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.listenForConnection(inboundConnectionCallback)
```

### Arguments: ###

- ***inboundConnectionCallback*** -- A function that will be called when the data is available from the device.

### Returns: ###

And immediate status is returned when the function is called and candidate devices are returned via the callback.  

#### Immediate status returned by function: ####

On success:

```json
{"status" : "OK", "desc" : "Started listener"}
```
On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

#### Results returned by callback: ####

When a device connects a message will be presented as in this example identifying the Bluetooth address of the connecting device:

```json
{
"status":"OK",
"event":"CONNECTION",
"desc":"Inbound Connection Event",
"address":"34:BB:1F:3E:77:BC"
}
```

Subsequently messages will be parsed and delivered in **RAW** format something like this example:

```json
{
	"status":"OK",
	"format":"RAW",
	"desc":"Device data",
	"data":	[2, 244, 7, 5, 57, 53, 54, 48, 0, 217, 3]
}
```

If the remote device unilaterally terminates the Bluetooth connection the following message will be delivered via the callback:

```json
{
	"status":"OK",
	"event":"DISCONNECTION",
	"desc":"Connection terminated - <error description>"
}
```

### Example Usage: ###
 
Call the function:

```javascript
	var inboundConnectionCallback = function(data) {
	var json = JSON.parse(data);
	if (json.status === 'OK') {
	...
	}
};

var foo = com.blackberry.community.simplebtsppplugin.listenForConnection(inboundConnectionCallback);
```

## disconnectFromDevice ##

### Name: ###

com.blackberry.community.simplebtsppplugin.disconnectFromDevice

### Description: ###

Used to instruct the plugin to disconnect from an SPP Bluetooth device. After this is called then no more data from the device will be presented via a callback.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.disconnectFromDevice()
```

### Arguments: ###

None

### Returns: ###

On success:

```json
{"status" : "OK", "desc" : "Bluetooth Connection Thread Stopped"}
```
On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

### Example Usage: ###
 
Call the function:

```javascript
var foo = com.blackberry.community.simplebtsppplugin.disconnectFromDevice();
```

## stopListening ##

### Name: ###

com.blackberry.community.simplebtsppplugin.stopListening

### Description: ###

Used to instruct the plugin to stop listening for inbound connections from an SPP Bluetooth device. After this is called then no more data from the device will be presented via a callback.

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.stopListening()
```

### Arguments: ###

None

### Returns: ###

On success:

```json
{"status" : "OK", "desc" : "Stopped listener", }
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

### Example Usage: ###
 
Call the function:

	var foo = com.blackberry.community.simplebtsppplugin.stopListening();

## sendMessage ##

### Name: ###

com.blackberry.community.simplebtsppplugin.sendMessage

### Description: ###

Used to send a message to an SPP Bluetooth connected device.  

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.sendMessage(message)
```

### Arguments: ###

- ***message*** -- Message to send to device

### Returns: ###

On success:

```json
{"status" : "OK", "desc" : "Data written to socket"}
```

On failure:

```json
{"status" : "ERROR", "desc" : "<description of failure>", "error_code" : <number>}
```

### Example Usage: ###

A message as an array of integers i where 0 <= i <= 255:

```javascript
var bar = {command: [0x02, 0x74, 0x02, 0x05, 0x05, 0x03]};

var foo = com.blackberry.community.simplebtsppplugin.sendMessage(JSON.stringify(bar));
```

## pluginVersion ##

### Name: ###

com.blackberry.community.simplebtsppplugin.pluginVersion

### Description: ###

Retrieves the version of the plugin being used as a string of the form: "major.minor.build".

### Type: ###

Synchronous Function

### Usage: ###

```javascript
com.blackberry.community.simplebtsppplugin.pluginVersion()
```

### Arguments: ###

None

### Returns: ###

"major.minor.build".

### Example Usage: ###
 
Call the function:

```javascript
var version = com.blackberry.community.simplebtsppplugin.pluginVersion();
```

## sppServiceUuid ##

### Name: ###

com.blackberry.community.simplebtsppplugin.sppServiceUuid

### Description: ###

Used to set or retrieve the SPP Service UUID that identifies the SPP device to which to connect.

### Type: ###

Property

### Values: ###

- An SPP Service UUID as a string such as: '00001101-0000-1000-8000-00805F9B34FB'

### Example Usage: ###

Set the SPP Service UUID to '00001101-0000-1000-8000-00805F9B34FB':

```javascript
com.blackberry.community.simplebtsppplugin.sppServiceUuid = '00001101-0000-1000-8000-00805F9B34FB';
```

Get the SPP Service UUID:

```javascript
var serviceUuid;
serviceUuid = com.blackberry.community.simplebtsppplugin.sppServiceUuid;
```

## sppRfcommMode ##

### Name: ###

com.blackberry.community.simplebtsppplugin.sppRfcommMode

### Description: ###

Used to set or retrieve the SPP RFCOMM Mode. Setting the RFCOMM mode means that the underlying extended APIs: **bt\_spp\_open\_server\_ex()** and **bt\_spp\_open\_ex()** will be used in preference to **bt\_spp\_open\_server()** and **bt\_spp\_open()**.

The extended RFCOMM APIs are available in **10.3.0** and above and require an extra parameter: the **RFCOMM Service Port**, which is an integer and which can be set using the **com.blackberry.community.simplebtsppplugin.sppRfcommServicePort** property.

### Type: ###

Property

### Values: ###

- A boolean
	- A value of **true** indicates that the **RFCOMM** API is to be used
	- A value of **false** indicates that the **SPP** API is to be used (**default**)

### Example Usage: ###

Set the SPP RFCOMM Mode to **true**:

```javascript
com.blackberry.community.simplebtsppplugin.sppRfcommMode = true;
```

Get the SPP RFCOMM Mode setting:

```javascript
var sppRfcommMode;
sppRfcommMode = com.blackberry.community.simplebtsppplugin.sppRfcommMode;
```

## sppRfcommServicePort ##

### Name: ###

com.blackberry.community.simplebtsppplugin.sppRfcommServicePort

### Description: ###

Used to set or retrieve the current value of the SPP RFCOMM Service Port.

The SPP RFCOMM Service Port is used only by the underlying extended APIs: **bt\_spp\_open\_server\_ex()** and **bt\_spp\_open\_ex()**.

In order to use the extended APIs rather than **bt\_spp\_open\_server()** and **bt\_spp\_open()** you need to set the value of the property **sppRfcommMode** to **true**. 

The extended RFCOMM APIs are available in **10.3.0** and above.

### Type: ###

Property

### Values: ###

- An SPP RCOMM Service Port value is an **Integer**. If you don't set this value explicitly then it will default to a value of 0 (Zero).

### Example Usage: ###

Set the SPP RFCOMM Service Port to a value to 1000:

```javascript
com.blackberry.community.simplebtsppplugin.sppRfcommServicePort = 1000;
```

Get the current SPP RFCOMM Service Port value:

```javascript
var sppRfcommServicePort;
sppRfcommServicePort = com.blackberry.community.simplebtsppplugin.sppRfcommServicePort;
```

## sppBufferSize ##

### Name: ###

com.blackberry.community.simplebtsppplugin.sppBufferSize

### Description: ###

Used to set or retrieve the current value of the SPP Buffer Size Property.

This is simply the size of the read/write buffer used internally to the plugin when it reads and write data to and from the underlying Bluetooth SPP APIs. This property should be set prior to calling **initialiseBluetooth** if you want to set it some value other than the default.

The default value is 1024 bytes and you might consider increasing this if the device you're communicating with over SPP or RFCOMM transfers data in chunks larger then 1024  bytes. It's more of an efficiency consideration. One interaction of, say, 4096 bytes, versus 4 of 1024 bytes. 

### Type: ###

Property

### Values: ###

- The SPP Buffer Size value is an **Integer**. If you don't set this value explicitly then it will default to a value of 1024. It should be positive and a multiple of 1024 bytes. Values that are not multiples of 1024 will be rounded up to the next multiple. Values <= 0 will be set to the default value of 1024.

### Example Usage: ###

Set the SPP Buffer SIze to a value to 4096:

```javascript
com.blackberry.community.simplebtsppplugin.sppBufferSize = 4096;
```

Get the current SPP Buffer Size value:

```javascript
var sppBufferSize;
sppBufferSize = com.blackberry.community.simplebtsppplugin.sppBufferSize;
```

## Example 1 - Plugin used as an SPP client ##

Here's an example of these APIs in use in a simple application where the application connects to the Bluetooth device supporting SPP.

```javascript
var app = {

	initialize: function() {
		this.bindEvents();
	},

	bindEvents: function() {
		document.addEventListener('deviceready', this.onDeviceReady, false);
	},

	onDeviceReady: function() {
		app.receivedEvent('deviceready');
	},

	receivedEvent: function(id) {
		var parentElement = document.getElementById(id);
		var listeningElement = parentElement.querySelector('.listening');
		var receivedElement = parentElement.querySelector('.received');

		listeningElement.setAttribute('style', 'display:none;');
		receivedElement.setAttribute('style', 'display:block;');

		console.log('Received Event: ' + id);
		app.testPluginCalls();
	},

	testPluginCalls: function() {
		if (com && com.blackberry.community.simplebtsppplugin) {

			app.writeOut('Plugin Version is: ' + com.blackberry.community.simplebtsppplugin.pluginVersion());

			app.writeOut('Initialise Bluetooth: ' + com.blackberry.community.simplebtsppplugin.initialiseBluetooth());
			app.writeOut('Start scanning: ' + com.blackberry.community.simplebtsppplugin.startDeviceScan(app.scanCallback));

		} else {
			app.writeOut("Plugin was not found");
		}
	},

	scanCallback: function(data) {
		console.log(data);
		app.writeOut(data);

		var bluetoothAddress = '94:eb:cd:64:a5:31';

		var json = JSON.parse(data);

		console.log("json.status " + json.status);

		if (json.status === 'OK') {
			console.log("json.devices.length " + json.devices.length);
			if (json.devices.length > 0) {
				for (var i = 0; i < json.devices.length; i++) {
					console.log("json.devices[i].address " + json.devices[i].address);
    				if (json.devices[i].name === 'BLACKBERRY-612D') {
						bluetoothAddress = json.devices[i].address;
						break;
    				}
				}
			}
		}
		com.blackberry.community.simplebtsppplugin.bluetoothAddress = bluetoothAddress;
		app.writeOut('Bluetooth Address is now: ' + com.blackberry.community.simplebtsppplugin.bluetoothAddress);

		app.writeOut('Connect to device: ' + com.blackberry.community.simplebtsppplugin.connectToDevice(app.connectionCallback));
	},

	connectionCallback: function(data) {
		if (app.connectionStarted) {
			console.log(data);
			app.itemcount++;
			app.writeOut("Item Count: " + app.itemcount);
			var json = JSON.parse(data);
			app.writeOut(data);
			app.writeOut('send message: ' + com.blackberry.community.simplebtsppplugin.sendMessage(JSON.stringify({ command: [0x06]})));
			if (app.itemcount > 10) {
				app.writeOut('disconnect from device: ' + com.blackberry.community.simplebtsppplugin.disconnectFromDevice());
				app.connectionStarted = false;
				app.writeOut('Terminate Bluetooth: ' + com.blackberry.community.simplebtsppplugin.terminateBluetooth());
			}
		} else {
			app.connectionStarted = true;
			app.writeOut(data);
			app.itemcount = 1;
			app.writeOut("Item Count: " + app.itemcount);
			app.writeOut('send message: ' + com.blackberry.community.simplebtsppplugin.sendMessage(JSON.stringify({ command: [0x06]})));
		}
	},

	writeOut: function(message) {
		var output = document.getElementById('results');
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement('br'));
		console.log(message);
	}
};
```

## Example 2 - Plugin used as an SPP server ##

Here's an example of these APIs in use in a simple application where the application listens for connections from the Bluetooth device supporting SPP.

```javascript
var app = {

	initialize: function() {
		this.bindEvents();
	},

	bindEvents: function() {
		document.addEventListener('deviceready', this.onDeviceReady, false);
	},

	onDeviceReady: function() {
		app.receivedEvent('deviceready');
	},

	receivedEvent: function(id) {
		var parentElement = document.getElementById(id);
		var listeningElement = parentElement.querySelector('.listening');
		var receivedElement = parentElement.querySelector('.received');

		listeningElement.setAttribute('style', 'display:none;');
		receivedElement.setAttribute('style', 'display:block;');

		console.log('Received Event: ' + id);
		app.testPluginCalls();
	},

	testPluginCalls: function() {
		if (com && com.blackberry.community.simplebtsppplugin) {

			app.writeOut('Plugin Version is: ' + com.blackberry.community.simplebtsppplugin.pluginVersion());

			app.writeOut('[1] SPP Service Uuid is: ' + com.blackberry.community.simplebtsppplugin.sppServiceUuid);

			app.writeOut('Initialise Bluetooth: ' + com.blackberry.community.simplebtsppplugin.initialiseBluetooth());
			app.writeOut('Start SPP Server: ' + com.blackberry.community.simplebtsppplugin.listenForConnection(app.inboundConnectionCallback));

		} else {
			app.writeOut("Plugin was not found");
		}
	},

	inboundConnectionCallback: function(data) {
		if (app.connectionStarted) {
			console.log(data);
			app.itemcount++;
			app.writeOut("Item Count: " + app.itemcount);
			var json = JSON.parse(data);
			app.writeOut(data);
			app.writeOut('send message: ' + com.blackberry.community.simplebtsppplugin.sendMessage(JSON.stringify({ command: [0x06]})));
			if (app.itemcount > 10) {
				app.writeOut('disconnect from device: ' + com.blackberry.community.simplebtsppplugin.disconnectFromDevice());
				app.writeOut('Stop listening: ' + com.blackberry.community.simplebtsppplugin.stopListening());
				app.connectionStarted = false;
				app.writeOut('Terminate Bluetooth: ' + com.blackberry.community.simplebtsppplugin.terminateBluetooth());
			}
		} else {
			app.connectionStarted = true;
			app.writeOut(data);
			app.itemcount = 1;
			app.writeOut("Item Count: " + app.itemcount);
			app.writeOut('send message: ' + com.blackberry.community.simplebtsppplugin.sendMessage(JSON.stringify({ command: [0x06]})));
		}
	},

	writeOut: function(message) {
		var output = document.getElementById('results');
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement('br'));
		console.log(message);
	}
};
```