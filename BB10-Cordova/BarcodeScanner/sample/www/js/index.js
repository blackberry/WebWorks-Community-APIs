/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
var app = {
	barcodeScanner: null,
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
		console.log(window.plugins);
		app.barcodeScanner = window.plugins.barcodeScanner;
		app.startBarcodeRead();
	},

	errorFound: function(data){
		console.log("Error : "+data.error + " description : "+ data.description);
	},

	codeFound: function(data) {
		console.log(data);
		if (gotCode === false) {
			gotCode = true;
			stopBarcodeRead();
			// blackberry.ui.toast.show("Detected : "+data.value);
		}
	},

	onStartRead: function(data){
		console.log("Started : "+data.successful);
	},

	onStopRead: function(data){
		console.log("Stopped : " +data.successful);
	},

	startBarcodeRead: function(){
		gotCode = false;
		// blackberry.app.lockOrientation("portrait-primary", false);
		app.barcodeScanner.startRead(app.codeFound, app.errorFound, "myCanvas", app.onStartRead);
		scanTimeout = setTimeout(scanTimeoutHalt, 60000);
	},

	stopBarcodeRead: function(){
		community.barcodescanner.stopRead(onStopRead, errorFound);
		clearTimeout(scanTimeout);
		scanTimeout = null;
		// blackberry.app.unlockOrientation();
	},

	scanTimeoutHalt: function() {
		console.log("No Code found in 60s. Stopping Scanner");
		// blackberry.ui.toast.show("No Code found in 60s. Stopping Scanner");
		stopBarcodeRead();
	},

	onPause: function() {
		if (scanTimeout !== null) {
			showResumeToast = true;
			stopBarcodeRead();
		}
	},

	onResume: function() {
		if (showResumeToast === true) {
			console.log("Application Minimized. Scanner Stopped");
			// blackberry.ui.toast.show("Application Minimized. Scanner Stopped");
			showResumeToast = false;
		}
	}
};
