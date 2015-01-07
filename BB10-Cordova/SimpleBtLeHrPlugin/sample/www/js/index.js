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
var btScanning = false;
var btScanned = false;
var bluetoothAddress = "";

var heartRateStream;
var heartRate;
var heartRateMax;
var heartRateMin;
var heartRateSlidingWindow;
var slidingWindowSize = 10;

var btLeHrHandler = {

    // BT LE HR Plugin Initialise Bluetooth
    //
    initialise: function() {
		if (com && com.blackberry.community.simplebtlehrplugin) {

        	console.log('XXXX Plugin Version is: ' + com.blackberry.community.simplebtlehrplugin.pluginVersion());

			var json = JSON.parse(com.blackberry.community.simplebtlehrplugin.initialiseBluetooth());

			console.log('XXXX json.status: ' + json.status);

			if (json.status === 'OK') {
				app.logMessage('Bluetooth initialised');
				bluetoothInitialised = true;
 				btScanning = false;
 				btScanned = false;

				heartRateStream = new Bacon.Bus();

				heartRate = heartRateStream.skipDuplicates().toProperty(0);

				heartRateMax = heartRateStream.scan(0, function(a, b) {
					if (a === 0) {
						return b;
					} else { 
						return Math.max(a, b);
					}
				}).toProperty();

				heartRateMin = heartRateStream.scan(0, function(a, b) {
					if (a === 0) {
						return b;
					} else { 
						return Math.min(a, b);
					}
				}).toProperty();

				heartRateSlidingWindow = heartRateStream.filter(
					function(a) { return (a > 0); }
				).slidingWindow(slidingWindowSize);

		        	app.setScanningMode(false);
			} else {
				app.logMessage('Bluetooth failed to initialise: ' + json.desc);
			}
		} else {
			app.logMessage('SimpleBtleHrPlugin was not found');
		}
    },

    // BT LE HR Plugin terminate Bluetooth
    //
    terminate: function() {
		var json = JSON.parse(com.blackberry.community.simplebtlehrplugin.terminateBluetooth());

		console.log('XXXX json.status: ' + json.status);

		if (json.status === 'OK') {
			app.logMessage('Bluetooth terminated');
		} else {
			app.logMessage('Bluetooth failed to terminated: ' + json.desc);
		}

		bluetoothInitialised = false;
		btScanning = false;

    	app.disableScanButton();
    },

	startScan: function() {
		app.logMessage('Starting to scan for devices');
		scannedDevices = [];
		var json = JSON.parse(com.blackberry.community.simplebtlehrplugin.startDeviceScan(btLeHrHandler.scanCallback));
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

	startMonitoring: function() {
		app.logMessage('Requesting to start monitoring');
		com.blackberry.community.simplebtlehrplugin.bluetoothAddress = bluetoothAddress;

		var json = JSON.parse(com.blackberry.community.simplebtlehrplugin.startMonitoringHr(btLeHrHandler.monitorCallback));
		if (json.status === 'OK') {
			app.logMessage('Monitoring started ...');
		} else {
			app.logMessage('Failed to start monitoring: ' + json.desc);
		}
	},

	monitorCallback: function(data) {
		console.log('XXXX DATA: ' + data);
		var json = JSON.parse(data);
		if (json.status === 'OK') {
			if (json.event === 'CONNECTION') {
				app.logMessage(json.desc);
			} else if(json.event === 'DISCONNECTION') {
				app.logMessage(json.desc);
				heartRateStream.end();
				btLeHrHandler.stopMonitoring();
			    app.stopMonitorMode();
			} else if(json.event === 'HRDATA') {
				app.logMessage("HR> " + json.data.heart_rate);
				heartRateStream.push(json.data.heart_rate);
			}
		} else if (json.status === 'ERROR') {
			if (json.event === 'CONNECTION') {
				app.logMessage(json.desc);
			} else if(json.event === 'HRDATA') {
				app.logMessage(json.desc);
			} else {
				app.logMessage('Unrecognised error on callback');
			}
		} else {
			heartRateStream.error();
			app.logMessage('JSON parse error');
		}
	},

	stopMonitoring: function() {
		app.logMessage('Requesting to stop monitoring');
		var json = JSON.parse(com.blackberry.community.simplebtlehrplugin.stopMonitoringHr());
		if (json.status === 'OK') {
			app.logMessage('Monitoring stopping ...');
		} else {
			app.logMessage('Failed to stop monitoring: ' + json.desc);
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
      	$("#btnStartMonitor").click(app.startMonitorMode);
      	$("#btnStopMonitor").click(app.stopMonitorMode);
      	$(document).one("mobileinit", function () {
	        app.receivedEvent('deviceready');
      	});
      	$.getScript('js/jquery.mobile-1.4.4.min.js');
    },

    // Update DOM on a Received Event
    receivedEvent: function(id) {
        console.log('XXXX Received Event: ' + id);
	$("#btnStartMonitor").attr('class', 'button-visible');
	$("#btnStopMonitor").attr('class', 'button-invisible');
    	$("#btnScan").prop('disabled', true);
    	$("#btnStopMonitor").prop('disabled', true);
    	$("#btnStartMonitor").prop('disabled', true);

        btLeHrHandler.initialise();

	heartRate.onValue(function(hr) {
		$("#heart-rate").text("Heart Rate: ").append(hr).append(" bpm");
	});

	heartRateMax.onValue(function(hr) {
		$("#heart-rate-max").text("Max Rate: ").append(hr).append(" bpm");
	});

	heartRateMin.onValue(function(hr) {
		$("#heart-rate-min").text("Min Rate: ").append(hr).append(" bpm");
	});

	heartRateSlidingWindow.onValue(function(hrWindow){
		if (hrWindow.length < slidingWindowSize) {
			if (hrWindow.length === 0) {
				$("#heart-rate-ave").text("... calculating average ");
			} else {
				$("#heart-rate-ave").append(".");
			}
		} else {
			var sum = 0;
			for(i=0; i < hrWindow.length; i++) { 
		 		sum += hrWindow[i];
			} 
			$("#heart-rate-ave").text("Average Rate: ").append(Math.floor(sum/hrWindow.length)).append(" bpm");
		}
	});
    },

    startScan: function() {
    	console.log("XXXX startScan");
    	btLeHrHandler.startScan();
    },

    startMonitorMode: function() {

	var selectedValue = $('#deviceList option:selected').val();
	var selectedText = $('#deviceList option:selected').text();

	bluetoothAddress = selectedValue;

	app.logMessage('Using device: ' + selectedText);
	app.logMessage('Using device: ' + selectedValue);


	$("#btnStartMonitor").attr('class', 'button-invisible');
	$("#btnStopMonitor").attr('class', 'button-visible');
	$("#select-block").attr('class', 'select-item-invisible');
	$("#info-block").attr('class', 'select-item-visible');
    	$("#btnScan").prop('disabled', true);
    	$("#btnStartMonitor").prop('disabled', true);
    	$("#btnStopMonitor").prop('disabled', false);
    	app.logMessage("Starting Monitor Mode");
    	app.setScanningMode(true);
    	btLeHrHandler.startMonitoring();
    },

    stopMonitorMode: function() {
	$("#btnStartMonitor").attr('class', 'button-visible');
	$("#btnStopMonitor").attr('class', 'button-invisible');
	$("#server-button-row").attr('class', 'button-row-invisible');
	$("#client-button-row").attr('class', 'button-row-visible');
	$("#select-block").attr('class', 'select-item-visible');
	$("#info-block").attr('class', 'select-item-invisible');
    	$("#btnScan").prop('disabled', false);
    	app.logMessage("Stopping Monitor Mode");
    	if (btScanned) {
	    	$("#btnStartMonitor").prop('disabled', false);
    	} else {
	    	$("#btnStartMonitor").prop('disabled', true);
    	}
    	app.setScanningMode(false);
    	clientMode = true;
    	btLeHrHandler.stopMonitoring();
    },

    setScanningMode: function(modeToBeSet) {
    	if (modeToBeSet) {
	    	$("#btnScan").prop('disabled', true);
	    	$("#btnStartMonitor").prop('disabled', true);
    	} else {
	    	$("#btnScan").prop('disabled', false);
	    	if (btScanned) {
		    	$("#btnStartMonitor").prop('disabled', false);
	    	} else {
		    	$("#btnStartMonitor").prop('disabled', true);
	    	}
    	}
    },

    updateSelection: function() {
	$("#deviceList").empty();
	$.each(scannedDevices, function(index, value) {
		$("#deviceList").append(new Option(value.name, value.address));	
	});
	app.enableSelection();
    },

    enableSelection: function() {
    	$("#deviceList").prop('disabled', false);
    	$("#btnStartMonitor").prop('disabled', false);
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
