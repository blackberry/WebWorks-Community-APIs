/*
 * Copyright (c) 2013-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 var bluetoothInitialised = false;
 var scannedDevices = [];
 var clientMode = true;
 var sppListening = false;
 var sppConnected = false;
 var btScanning = false;
 var btScanned = false;
 var bluetoothAddress = "";

 var sppHandler = {

    // SPP Plugin Initialise Bluetooth
    //
    initialise: function() {
		if (com && com.blackberry.community.simplebtsppplugin) {

        	console.log('XXXX Plugin Version is: ' + com.blackberry.community.simplebtsppplugin.pluginVersion());
        	console.log('XXXX SPP Service Uuid is: ' + com.blackberry.community.simplebtsppplugin.sppServiceUuid);

			var json = JSON.parse(com.blackberry.community.simplebtsppplugin.initialiseBluetooth());

			console.log('XXXX json.status: ' + json.status);

			if (json.status === 'OK') {
				app.logMessage('Bluetooth initialised');
				bluetoothInitialised = true;
				sppListening = false;
 				sppConnected = false;
 				btScanning = false;
 				btScanned = false;
		        app.setScanningMode(false);
			} else {
				app.logMessage('Bluetooth failed to initialise: ' + json.desc);
			}
		} else {
			app.logMessage('SimpleBtSppPlugin was not found');
		}
    },

    // SPP Plugin terminate Bluetooth
    //
    terminate: function() {
		var json = JSON.parse(com.blackberry.community.simplebtsppplugin.terminateBluetooth());

		console.log('XXXX json.status: ' + json.status);

		if (json.status === 'OK') {
			app.logMessage('Bluetooth terminated');
		} else {
			app.logMessage('Bluetooth failed to terminated: ' + json.desc);
		}

		bluetoothInitialised = false;
		sppListening = false;
		sppConnected = false;
		btScanning = false;

    	app.disableScanButton();
    },

	startScan: function() {
		app.logMessage('Starting to scan for devices');
		scannedDevices = [];
		var json = JSON.parse(com.blackberry.community.simplebtsppplugin.startDeviceScan(sppHandler.scanCallback));
		if (json.status === 'OK') {
			app.logMessage('Started Scanning ... please wait ...');
			btScanning = true;
            app.setScanningMode(true);
		} else {
			app.logMessage('Failed to start scanning: ' + json.desc);
		}
	},

	scanCallback: function(data) {
		app.logMessage('... finished ... scan completed');
		var json = JSON.parse(data);
		btScanning = true;
        app.setScanningMode(false);
        app.enableSelection();

		if (json.status === 'OK') {
			btScanned = true;
			console.log('XXXX json.devices.length ' + json.devices.length);
			if (json.devices.length > 0) {
				for (var i = 0; i < json.devices.length; i++) {
					var thisDevice = {};
					thisDevice.name = json.devices[i].name;
					thisDevice.address = json.devices[i].address;
					scannedDevices.push(thisDevice);
					console.log('XXXX Name: ' + json.devices[i].name);
					console.log('XXXX Address: ' + json.devices[i].address);
				}
				app.updateSelection();
			} else {
				app.logMessage('No suitable devices');
			}
		} else {
			app.logMessage('Scanning error: ' + json.desc);
		}
	},

	connectRequest: function() {
		com.blackberry.community.simplebtsppplugin.bluetoothAddress = bluetoothAddress;

		var json = JSON.parse(com.blackberry.community.simplebtsppplugin.connectToDevice(sppHandler.connectionCallback));
		if (json.status === 'OK') {
			sppConnected = true;
			app.logMessage('Connected to device');
			app.setConnectedMode(true);

		} else {
			sppConnected = false;
			app.logMessage('Failed to connect to device: ' + json.desc);
			app.setConnectedMode(false);
			sppHandler.disconnectRequest();
		}
	},

	connectionCallback: function(data) {
		var json = JSON.parse(data);

		if (json.status === 'OK') {
			app.setConnectedMode(true);
			if (json.event !== 'DISCONNECTION') {
				console.log('XXXX data: ' + data);
				if (json.format ===  "RAW") {

					var dataArray = json.data;
					var dataArrayLen = json.data.length;

  					var dataArraybuf = new ArrayBuffer(dataArrayLen);
  					var dataArrayBufView = new Uint8Array(dataArraybuf);

			  		for (var i=0; i < dataArrayLen; i++) {
			    		dataArrayBufView[i] = dataArray[i];
			  		}

					var message = sppHandler.ab2str(dataArraybuf);
					app.logMessage('>> ' + message);

				} else {
					app.logMessage('>> ' + json.data);
				}
			} else {
				app.logMessage('Disconnect Received');
				app.setConnectedMode(false);
				sppConnected = false;
				sppHandler.disconnectRequest();
			}
		} else {
		}
	},

	disconnectRequest: function() {
		var json = JSON.parse(com.blackberry.community.simplebtsppplugin.disconnectFromDevice());
		if (json.status === 'OK') {
			app.logMessage('Disconnected');
		} else {
			app.logMessage('Disconnect error: ' + json.desc);
		}
		app.setConnectedMode(false);
		sppConnected = false;

		if (!clientMode) {
			sppHandler.stopListening();
		}
	},

    sendMessage: function(message) {

    	console.log('XXXX message: ' + message);

    	var messageArray = [];
    	var messageArrayBuffer = sppHandler.str2ab(message);

    	var messageBytes = new Uint8Array(messageArrayBuffer)
  		var messageByteLength = messageBytes.byteLength;

  		if (messageByteLength%2 != 0) {
  			messageByteLength -= 1;
  		}

  		for (var i=0; i < messageByteLength; i++) {
    		messageArray.push(messageBytes[i]);
  		}

    	console.log('XXXX messageArray: ' + messageArray );
    	var messageObject = {command: messageArray };
    	console.log('XXXX messageObject: ' + messageObject);
    	var messageString = JSON.stringify(messageObject);
    	console.log('XXXX messageString: ' + messageString);

		var json = JSON.parse(com.blackberry.community.simplebtsppplugin.sendMessage(messageString));
		if (json.status === 'OK') {
			app.logMessage('<< ' + message);
		} else {
			app.logMessage('Send error: ' + json.desc);
		}
    },


	ab2str: function(buf) {
  		return String.fromCharCode.apply(null, new Uint16Array(buf));
	},

	str2ab: function(str) {
  		var buf = new ArrayBuffer(str.length*2);
  		var bufView = new Uint16Array(buf);
  		for (var i=0, strLen=str.length; i < strLen; i++) {
    		bufView[i] = str.charCodeAt(i);
  		}
  		return buf;
	},

	startListening: function() {

		var json = JSON.parse(com.blackberry.community.simplebtsppplugin.listenForConnection(sppHandler.inboundConnectionCallback));
		if (json.status === 'OK') {
			app.logMessage('Listening for connections ...');
			sppListening = true;
			app.setListeningMode(true);
		} else {
			app.logMessage('Listen initiation error: ' + json.desc);
		}
	},
	
	stopListening: function(){
		var json = JSON.parse(com.blackberry.community.simplebtsppplugin.stopListening());
		if (json.status === 'OK') {
			app.logMessage('... stopped Listening');
			sppListening = false;
			app.setListeningMode(false);
		} else {
			app.logMessage('Stop Listening error: ' + json.desc);
		}
	},

	inboundConnectionCallback: function(data) {
		console.log('XXXX inboundConnectionCallback() - ' + data);

		var json = JSON.parse(data);
		if (json.status === 'OK') {
			if (json.event === 'CONNECTION') {
				app.logMessage('Inbound connection from ' + json.address);
				sppConnected = true;
				app.setConnectedMode(true);
			} else if (json.event === 'DISCONNECTION') {
				app.logMessage('Disconnected from remote device');
				app.setConnectedMode(false);
				sppConnected = false;
				if (!clientMode) {
					sppHandler.stopListening();
				}
			} else {
				console.log('XXXX data: ' + data);
				if (json.format ===  "RAW") {

					var dataArray = json.data;
					var dataArrayLen = json.data.length;

					if (dataArrayLen > 0) {
  						var dataArraybuf = new ArrayBuffer(dataArrayLen);
 	 					var dataArrayBufView = new Uint8Array(dataArraybuf);

			  			for (var i=0; i < dataArrayLen; i++) {
			    			dataArrayBufView[i] = dataArray[i];
			  			}
						var message = sppHandler.ab2str(dataArraybuf);
						app.logMessage('>> ' + message);
					}
				} else {
					app.logMessage('>> ' + json.data);
				}
			}
		} else {
			app.logMessage('Callback error: ' + json.desc);
		}
    }
 };

 var app = {

    // Application Constructor
    initialize: function() {
        this.bindEvents();
    },

    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },

    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicity call 'app.receivedEvent(...);'
    onDeviceReady: function() {
      	$("#btnScan").click(app.startScan);
      	$("#btnServerMode").click(app.setServerMode);
      	$("#btnClientMode").click(app.setClientMode);
      	$("#btnConnect").click(app.connectRequest);
      	$("#btnDisconnect").click(app.disconnectRequest);
      	$("#btnSend").click(app.sendMessage);
      	$("#btnStartListening").click(app.startListening);
      	$("#btnStopListening").click(app.stopListening);
      	$(document).one("mobileinit", function () {
	        app.receivedEvent('deviceready');
      	});
      	$.getScript('js/jquery.mobile-1.4.4.min.js');
    },

    // Update DOM on a Received Event
    receivedEvent: function(id) {
        console.log('XXXX Received Event: ' + id);
        sppHandler.initialise();
    },

    startScan: function() {
    	console.log("XXXX startScan");
    	sppHandler.startScan();
    },

    sendMessage: function() {
		sppHandler.sendMessage($('#inputField').val());
		$('#inputField').val('');
    },

    setServerMode: function() {
		$("#btnServerMode").attr('class', 'button-invisible');
		$("#btnClientMode").attr('class', 'button-visible');
		$("#server-button-row").attr('class', 'button-row-visible');
		$("#client-button-row").attr('class', 'button-row-invisible');
		$("#select-block").attr('class', 'select-item-invisible');
		$("#input-block").attr('class', 'input-item-visible');
    	$("#btnScan").prop('disabled', true);
    	app.logMessage("Using server Mode");
    	app.setScanningMode(true);
    	clientMode = false;
    },

    setClientMode: function() {
		$("#btnServerMode").attr('class', 'button-visible');
		$("#btnClientMode").attr('class', 'button-invisible');
		$("#server-button-row").attr('class', 'button-row-invisible');
		$("#client-button-row").attr('class', 'button-row-visible');
		$("#select-block").attr('class', 'select-item-visible');
		$("#input-block").attr('class', 'input-item-invisible');
    	$("#btnScan").prop('disabled', false);
    	app.logMessage("Using client Mode");
    	if (btScanned) {
	    	$("#btnConnect").prop('disabled', false);
    	} else {
	    	$("#btnConnect").prop('disabled', true);
    	}
    	app.setScanningMode(false);
    	clientMode = true;
    },

    setScanningMode: function(modeToBeSet) {
    	if (modeToBeSet) {
	    	$("#btnScan").prop('disabled', true);
	    	$("#btnServerMode").prop('disabled', true);
	    	$("#btnConnect").prop('disabled', true);
    	} else {
	    	$("#btnScan").prop('disabled', false);
	    	$("#btnServerMode").prop('disabled', false);
	    	$("#btnConnect").prop('disabled', false);
    	}
    },

    updateSelection: function() {
		$("#deviceList").empty();
		$.each(scannedDevices, function(index, value) {
			$("#deviceList").append(new Option(value.name, value.address));	
		});
    },

    enableSelection: function() {
    	$("#deviceList").prop('disabled', false);
    	$("#btnConnect").prop('disabled', false);
    },

    setListeningMode: function(modeToBeSet) {
    	if (modeToBeSet) {
	    	$("#btnStartListening").prop('disabled', true);
    		$("#btnStopListening").prop('disabled', false);
    	} else {
	    	$("#btnStartListening").prop('disabled', false);
    		$("#btnStopListening").prop('disabled', true);
    	}
    },

    setConnectingMode: function(modeToBeSet) {
    	if (modeToBeSet) {
	    	$("#btnConnect").prop('disabled', true);
    		$("#btnDisconnect").prop('disabled', false);
	        app.setScanningMode(true);
    	} else {
	    	$("#btnConnect").prop('disabled', false);
    		$("#btnDisconnect").prop('disabled', true);
	        app.setScanningMode(false);
    	}
    },

    setConnectedMode: function(modeToBeSet) {
    	if (modeToBeSet) {
	    	$("#btnConnect").prop('disabled', true);
    		$("#btnDisconnect").prop('disabled', false);
			$("#server-button-row").attr('class', 'button-row-invisible');
			$("#client-button-row").attr('class', 'button-row-visible');
			$("#select-block").attr('class', 'select-item-invisible');
			$("#input-block").attr('class', 'input-item-visible');
	    	$("#btnSend").prop('disabled', false);
	    	$("#inputField").prop('disabled', false);
	        app.setScanningMode(true);
    	} else {
	    	$("#btnConnect").prop('disabled', false);
	    	$("#btnDisconnect").prop('disabled', true);
	    	$("#btnSend").prop('disabled', true);
	    	$("#inputField").prop('disabled', true);
	        if (clientMode) {
				$("#server-button-row").attr('class', 'button-row-invisible');
				$("#client-button-row").attr('class', 'button-row-visible');
				$("#select-block").attr('class', 'select-item-visible');
				$("#input-block").attr('class', 'input-item-invisible');
	        	app.setScanningMode(false);
		    } else {
				$("#server-button-row").attr('class', 'button-row-visible');
				$("#client-button-row").attr('class', 'button-row-invisible');
				$("#select-block").attr('class', 'select-item-invisible');
				$("#input-block").attr('class', 'input-item-visible');
    	        app.setScanningMode(false);
			}
    	}
    },

	setListeningMode: function(modeToBeSet) {
    	if (modeToBeSet) {
	    	$("#btnStartListening").prop('disabled', true);
    		$("#btnStopListening").prop('disabled', false);
    		$("#btnClientMode").prop('disabled', true);
    	} else {
	    	$("#btnStartListening").prop('disabled', false);
    		$("#btnStopListening").prop('disabled', true);
    		$("#btnClientMode").prop('disabled', false);
    	}
	},

    connectRequest: function() {
		var selectedValue = $('#deviceList option:selected').val();
		var selectedText = $('#deviceList option:selected').text();

		bluetoothAddress = selectedValue;

		app.logMessage('Using device: ' + selectedText);
		app.logMessage('Using device: ' + selectedValue);

		sppHandler.connectRequest();
    },

    disconnectRequest: function() {
		sppHandler.disconnectRequest();
	},

	startListening: function() {
		sppHandler.startListening();
	},
	
	stopListening: function(){
		sppHandler.stopListening();
	},

	disableScanButton: function() {
    	$("#btnScan").prop('disabled', true);
	},

    // Log to screen
    //
    logMessage: function(message) {
		console.log('XXXX ' + message);
		$('#content').append('<br>').append(message);
		$('#logBlock').scrollTop($('#logBlock').prop('scrollHeight'));
	}
};
