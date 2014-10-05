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
	clearApplicationLog: "_clear_",
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
		var butt = document.getElementById("select");
		butt.addEventListener("click", this.onClicked, false);
	},
	testMetaDataExtraction: function(path) {
		if (com && com.blackberry.community.audiometadata) {
			com.blackberry.community.audiometadata.audioMetaDataGetMetaData(path, app.metadataCallback);
		} else {
			alert("Plugin was not found");
		}
	},
	onClicked: function(event) {
		if (blackberry && blackberry.invoke.card) {
			// filepicker options
			var details = {
					mode: blackberry.invoke.card.FILEPICKER_MODE_PICKER,
					type: [blackberry.invoke.card.FILEPICKER_TYPE_MUSIC],
					filer: ["*.mp3"],
					viewMode: blackberry.invoke.card.FILEPICKER_VIEWER_MODE_GRID,
					sortBy: blackberry.invoke.card.FILEPICKER_SORT_BY_NAME,
					sortOrder: blackberry.invoke.card.FILEPICKER_SORT_ORDER_DESCENDING
			};
			blackberry.invoke.card.invokeFilePicker(details, function(path) {
				app.testMetaDataExtraction(path);
			},
			// cancel callback
			function(reason) {
				alert("closed reason " + reason);
			},
			// error callback
			function(error) {
				if(error) {
					alert("invoke error " + error);
				} else {
					console.log("invoke success ");
				}
			});
		}
	},
	writeOut: function(message) {
		var output = document.getElementById('results');
		if (message == app.clearApplicationLog) {
			output.innerText = "";
		} else {
			output.innerText = output.innerText + message;
			output.appendChild(document.createElement('br'));
			console.log(message);
		}
	},
	metadataCallback: function(data) {
		if (data) {
			app.writeOut(app.clearApplicationLog);
			for (var key in data) {
				if (data.hasOwnProperty(key)) {
					app.writeOut(key + " : " + data[key]);
				}
			}
			app.writeOut("\n");
		} else {
			alert("no data");
		}
	}
};
