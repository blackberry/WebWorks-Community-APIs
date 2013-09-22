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
	options: {
		alert: {
			title: 'Title',
			message: 'Message'
		},
		prompt: {
			title: 'Title',
			message: 'Message'
		},
		confirm: {
			title: 'Title',
			message: 'Message'
		}
	},

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
	receivedEvent: function(id) {
		var btns = {
			alert : document.getElementById("messagebox_alert"),
			prompt : document.getElementById("messagebox_prompt"),
			confirm : document.getElementById("messagebox_confirm")
		}

		for(var btn in btns) {
			btns[btn].addEventListener("click", this.messagebox_btnClick, false)
		}
	},

	messagebox_btnClick: function(evt) {
		evt.preventDefault();
		var btn_id_prepend = "messagebox_";
		var messageBox = window.plugins.messageBox
		var fn_name = evt.target.id.substring(btn_id_prepend.length);

		var cb = function(return_val1, return_val2) {
			var results = document.getElementById("results");
			var results_cnt = "";

			results_cnt += "return_val1: " + (return_val1 ? return_val1 : "") + "<br />";
			results_cnt += "return_val2: " + (return_val2 ? return_val2 : "");

			results.innerHTML = results_cnt; 
		}

		messageBox[fn_name](app.options[fn_name], cb);
	}
};
