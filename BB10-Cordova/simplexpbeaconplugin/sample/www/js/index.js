/*
 * Copyright (c) 2013-2015 BlackBerry Limited.
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
var beaconList = {};
var beaconLossIntervalCheck = 10000; // milli-seconds
var beaconStream = new Bacon.Bus();

var discoveredBeacons = beaconStream.flatMap(function(beacon){
	var secondsNow = Math.floor(new Date().getTime() / 1000);
	var beaconHash = btLeHandler.hashFromBeacon(beacon.uuid, beacon.major, beacon.minor);
	var currentRange = 0;

	if (beacon.proximity && beacon.accuracy) {
		currentRange = Math.floor(beacon.accuracy*100)/100;
	} else {
		currentRange = btLeHandler.getRange(beacon.txpower, beacon.rssi);
	}
	if (!(beaconHash in beaconList)) {
	    beaconList[beaconHash] = { "time" : secondsNow, "currentRange": currentRange,
	                               "previousRange": currentRange, "event" : beacon};
    	return new Bacon.Next(beacon);
	} else {
		var previousRange = beaconList[beaconHash].currentRange;
        beaconList[beaconHash] = { "time" : secondsNow, "currentRange": currentRange,
                                   "previousRange": previousRange, "event" : beacon};
        return Bacon.never();
	}
});

var lostBeacons = Bacon.fromBinder(function(sink) {
    var id = setInterval(function() {
        var secondsNow = Math.floor(new Date().getTime() / 1000);
        for (var beaconHash in beaconList) {
            if ((secondsNow - beaconList[beaconHash].time) > 20) {
                var event = beaconList[beaconHash].event;
                sink(new Bacon.Next(event));
                delete beaconList[beaconHash];
            }
        }
    }, beaconLossIntervalCheck);
    return function() {
        clearInterval(id);
    };
});

var movedBeacons = beaconStream.flatMap(function(beacon) {
    var beaconHash = btLeHandler.hashFromBeacon(beacon.uuid, beacon.major, beacon.minor);
    if (beaconHash in beaconList) {
    	var range = btLeHandler.getRange(beacon.txpower, beacon.rssi);
		if (beacon.proximity && beacon.accuracy) {
			range = Math.floor(beacon.accuracy*100)/100;

		}
        if (btLeHandler.proximity(range) !== btLeHandler.proximity(beaconList[beaconHash].previousRange)) {
                return new Bacon.Next(beacon);
        }
    }
    return Bacon.never();
});

beaconStream.onValue(function(beacon) {
	if (beacon.accuracy && beacon.proximity) {
		app.updateInfoIos(beacon.uuid, beacon.major, beacon.minor, beacon.rssi, Math.floor(beacon.accuracy*100)/100, beacon.proximity);
	} else if (beacon.rssi && beacon.txpower) {
		app.updateInfo(beacon.uuid, beacon.major, beacon.minor, beacon.rssi, beacon.rssi - beacon.txpower);
	}
});

discoveredBeacons.onValue(function(beacon){
    app.logMessage("Discovered: " + beacon.uuid + " / "  + beacon.major + " / "  + beacon.minor);
});

lostBeacons.onValue(function(beacon){
    app.logMessage("Lost: " + beacon.uuid + " / "  + beacon.major + " / "  + beacon.minor);
});

movedBeacons.onValue(function(beacon){

	var range = btLeHandler.getRange(beacon.txpower, beacon.rssi);
	if (beacon.proximity && beacon.accuracy) {
		range = Math.floor(beacon.accuracy*100)/100;
	}

	var beaconHash = btLeHandler.hashFromBeacon(beacon.uuid, beacon.major, beacon.minor);
	app.logMessage("Move: " + beacon.uuid + " / "  + beacon.major + " / "  + beacon.minor);
	app.logMessage("From: " + btLeHandler.proximity(beaconList[beaconHash].previousRange) +
		           " -> " + btLeHandler.proximity(range));
});

var btLeHandler = {

    // BT LE Beacon Plugin Initialise Bluetooth
    //
    initialise: function() {
		if (com && com.blackberry.community.simplexpbeaconplugin) {

        	com.blackberry.community.simplexpbeaconplugin.pluginVersion(
        		function(data){

					console.log('XXXX DATA: ' + data);
					var json = JSON.parse(data);
					console.log('XXXX json.status: ' + json.status);				

					if (json.status === 'OK') {
						app.logMessage('Plugin Version: ' + json.plugin_version);
					} else {
						app.logMessage('Failed to get Plugin Version: ' + json.desc);
					}
	        	}
	        );

			com.blackberry.community.simplexpbeaconplugin.initialiseBluetooth(
				function(data){

					console.log('XXXX DATA: ' + data);
					var json = JSON.parse(data);
					console.log('XXXX json.status: ' + json.status);				

					if (json.status === 'OK') {
						app.logMessage('Bluetooth initialised');
						bluetoothInitialised = true;
					} else {
						app.logMessage('Bluetooth failed to initialise: ' + json.desc);
					}
				}
			);

			com.blackberry.community.simplexpbeaconplugin.addBeaconUuidToMonitor(
				"8AEFB031-6C32-486F-825B-E26FA193487D",
				function(data){

					console.log('XXXX DATA: ' + data);
					var json = JSON.parse(data);
					console.log('XXXX json.status: ' + json.status);				

					if (json.status === 'OK') {
						app.logMessage('Beacon added');
						bluetoothInitialised = true;
					} else {
						app.logMessage('Failed to add beacon: ' + json.desc);
					}
				}
			);
		} else {
			app.logMessage('SimplexpXpBeaconPlugin was not found');
		}
    },

    // BT LE HR Plugin terminate Bluetooth
    //
    terminate: function() {
		com.blackberry.community.simplexpbeaconplugin.removeBeaconUuidToMonitor(
			"8AEFB031-6C32-486F-825B-E26FA193487D",
			function(data){

				console.log('XXXX DATA: ' + data);
				var json = JSON.parse(data);
				console.log('XXXX json.status: ' + json.status);				

				if (json.status === 'OK') {
					app.logMessage('Beacon removed');
					bluetoothInitialised = true;
				} else {
					app.logMessage('Failed to remove beacon: ' + json.desc);
				}
			}
		);

		com.blackberry.community.simplexpbeaconplugin.terminateBluetooth(
			function(data){

				console.log('XXXX DATA: ' + data);
				var json = JSON.parse(data);
				console.log('XXXX json.status: ' + json.status);				
		
				if (json.status === 'OK') {
					app.logMessage('Bluetooth terminated');
				} else {
					app.logMessage('Bluetooth failed to terminated: ' + json.desc);
				}
				bluetoothInitialised = false;
			}
		);
    },

	startMonitoring: function() {
		app.logMessage('Requesting to start monitoring');
		beaconList = {};
		com.blackberry.community.simplexpbeaconplugin.startMonitoring(btLeHandler.monitorCallback);
	},

	monitorCallback: function(data) {
		console.log('XXXX DATA: ' + data);
		var json = JSON.parse(data);
		if (json.status === 'OK') {
			if(json.event === 'IBEACON') {
				beaconStream.push(json.data);
			} else if(json.event === 'IBEACON_PROXIMITY') {
				beaconStream.push(json.data);
			} else if(json.event === 'STARTED') {
				app.logMessage('Monitoring started ...');
			} else {
				app.logMessage('Unrecognised event: ' + json.desc);
			}
		} else if (json.status === 'ERROR') {
			if(json.event === 'IBEACON') {
				app.logMessage(json.desc);
			} else if(json.event === 'STARTED') {
				app.logMessage('Monitoring failed to start ...');
			} else {
				app.logMessage('Unrecognised error on callback');
			}
		} else {
			beaconStream.error();
			app.logMessage('JSON parse error');
		}
	},

	stopMonitoring: function() {
		app.logMessage('Requesting to stop monitoring');
		com.blackberry.community.simplexpbeaconplugin.stopMonitoring(
			function(data){

				console.log('XXXX DATA: ' + data);
				var json = JSON.parse(data);
				console.log('XXXX json.status: ' + json.status);				
		
				if (json.status === 'OK') {
					app.logMessage('Monitoring stopped');
				} else {
					app.logMessage('Failed to stop monitoring: ' + json.desc);
				}
				beaconList = {};
			}
		);
   	},
	
	hashFromBeacon: function(uuid, major, minor) {
		return uuid + "/" + major + "/"  + minor;
	},

	getRange: function(txPower, rssi) {
    	var ratio_db = txPower - rssi;
    	var ratio_linear = Math.pow(10, ratio_db / 10);

    	var r = Math.sqrt(ratio_linear);
    	return Math.floor(r*100)/100;
	},

	proximity: function(range) {
		if (range < 1.00) {
	    	return "very close";
		} else if(1.00 <= range && range < 5.00) {
	    	return "near";
		} else {
	    	return "far";
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
    	$("#btnStartMonitor").prop('disabled', false);
    	$("#btnStopMonitor").prop('disabled', true);

        btLeHandler.initialise();
    },

    startMonitorMode: function() {
		$("#info-block").attr('class', 'button-visible');
    	$("#btnStartMonitor").prop('disabled', true);
    	$("#btnStopMonitor").prop('disabled', false);
    	app.logMessage("Starting Monitor Mode");
    	btLeHandler.startMonitoring();
    },

	stopMonitorMode: function() {
    	app.logMessage("Stopping Monitor Mode");
		$("#info-block").attr('class', 'button-invisible');
    	$("#btnStartMonitor").prop('disabled', false);
    	$("#btnStopMonitor").prop('disabled', true);
    	btLeHandler.stopMonitoring();
    },

    updateInfo: function(uuid, major, minor, rssi, loss) {
		$("#beacon-uuid").text("UUID: ").append(uuid);
		$("#beacon-details").text("Major: ").append(major).append(" ")
		                  .append("Minor: ").append(minor).append(" ")
		                  .append("RSSI: ").append(rssi).append(" dBm ")
		                  .append("Loss: ").append(loss).append(" dB");
	},

    updateInfoIos: function(uuid, major, minor, rssi, accuracy, proximity) {
		$("#beacon-uuid").text("UUID: ").append(uuid);
		$("#beacon-details").text("Major: ").append(major).append(" ")
		                  .append("Minor: ").append(minor).append(" ")
		                  .append("RSSI: ").append(rssi).append(" dBm ")
		                  .append("Accuracy: ").append(accuracy).append(" m");
	},

    // Log to screen
    //
    logMessage: function(message) {
		console.log('XXXX ' + message);
		$('#content').append('<br>').append(message);
		$('#logBlock').scrollTop($('#logBlock').prop('scrollHeight'));
	}
};
