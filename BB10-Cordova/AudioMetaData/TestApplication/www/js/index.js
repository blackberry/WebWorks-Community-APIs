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
	pathGlobal: "",
	attributes: new Object(),
	dropDownGlobal: null,
	textfieldGlobal: null,
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
		
		app.dropDownGlobal = document.getElementById("dropdown");
		app.textfieldGlobal = document.getElementById("textfield");

		console.log('Received Event: ' + id);
		var selectButt = document.getElementById("select");
		selectButt.addEventListener("click", this.onClicked, false);
		var getButt = document.getElementById("getButton");
		getButt.addEventListener("click", this.testMetaDataExtraction, false);
		var attributeButt = document.getElementById("setButton");
		attributeButt.addEventListener("click", this.testMetaDataSet, false);
		var removeButt = document.getElementById("removeButton");
		removeButt.addEventListener("click", this.testMetaDataRemove, false);
		var selectButt = document.getElementById("setter");
		selectButt.addEventListener("click", this.setAttribute, false);
	},
	testMetaDataExtraction: function(event) {
		if (com && com.blackberry.community.audiometadata) {
			if (app.pathGlobal.length > 0) {
				com.blackberry.community.audiometadata.audioMetaDataGetMetaData(app.pathGlobal, app.metadataCallback);
			} else {
				alert("No file selected");
			}
		} else {
			alert("Plugin was not found");
		}
	},
	testMetaDataSet: function(event) {
		if (com && com.blackberry.community.audiometadata) {
			if (app.pathGlobal.length > 0) {
				if (Object.keys(app.attributes).length > 1 && app.attributes["path"] == app.pathGlobal) {
					app.writeOut(app.clearApplicationLog);
					app.writeOut(com.blackberry.community.audiometadata.audioMetaDataSetTagData(app.attributes));
				} else {
					alert("Not enough inputs");
				}
			} else {
				alert("No file selected");
			}
		} else {
			alert("Plugin was not found");
		}
	},
	testMetaDataRemove: function(event) {
		if (com && com.blackberry.community.audiometadata) {
			if (app.pathGlobal.length > 0) {
				app.writeOut(app.clearApplicationLog);
				app.writeOut(com.blackberry.community.audiometadata.audioMetaDataRemoveTag(app.pathGlobal));
			} else {
				alert("No file selected");
			}
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
				app.pathGlobal = path;
				app.attributes = new Object();
				app.attributes["path"] = app.pathGlobal;
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
	setAttribute: function(event) {
		app.attributes[app.dropDownGlobal.value] = app.textfieldGlobal.value;
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
					app.writeOut(key + " : " + decodeURIComponent(escape(data[key])));
				}
			}
			app.writeOut("\n");
		} else {
			alert("no data");
		}
	}
};
