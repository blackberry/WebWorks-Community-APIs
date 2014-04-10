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
		app.receivedEvent('deviceready');
	},
	// Update DOM on a Received Event
	receivedEvent: function(id) {
		var parentElement = document.getElementById(id);
		var listeningElement = parentElement.querySelector('.listening');
		var receivedElement = parentElement.querySelector('.received');

		listeningElement.setAttribute('style', 'display:none;');
		receivedElement.setAttribute('style', 'display:block;');

		console.log('Received Event: ' + id);
		
	},
	
	writeOut: function(message) {
		var output = document.getElementById('results');
		output.innerText = message;
		console.log(message);
	},

	
	record: function(){
		if (community.audiorecorder.mediaState() == "0") {
			app.setOutput();//set the file output if recording has not started
			community.audiorecorder.record();//begin recording(this will call prepare in native code)
			app.writeOut("Recording...");//write status
		}
		else if (community.audiorecorder.mediaState() == "1" || community.audiorecorder.mediaState() == "3"){
			community.audiorecorder.record();//start recording if prepared or paused
			app.writeOut("Recording...");//write status
		}
		else if (community.audiorecorder.mediaState() == "2"){
		community.audiorecorder.pause();//pause only if currently recording
		app.writeOut("Paused");//write status
		}
	},
	
	setOutput: function(){
		var file = "file:///accounts/1000/shared/voice/recording.m4a";//set name for recording file here
		community.audiorecorder.setOutput(file);
	},
	
	reset: function(){
		if (community.audiorecorder.mediaState() != '4'){
			community.audiorecorder.reset();//stop the recording, reset the recorder
			app.writeOut("Recording Complete");//write status
		}
	},

};
