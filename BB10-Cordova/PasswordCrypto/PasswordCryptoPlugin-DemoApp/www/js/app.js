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
	},
	pbkdf2CPP: function(password, salt, iterations, keylength){
		if (community && community.PasswordCrypto) {
	
			var passwdParam = {
				"password": password,  
				"salt": salt, // 
				"iterations": iterations, 
				"keyLength": keylength //  is bytes, not bits!
			};
						
			var passwdVal=community.PasswordCrypto.pbkdf2_Async(passwdParam, app.cppHashCallback); 
			
		} else {
			console.log("Could not find Plugin");
		}
			
	},
	cppHashCallback: function(result){
		console.log(result);
		if(result.success){
			var end = new Date().getTime();
			var runtime = end - app.startstopwatch;
			app.displayResult(result.pbkdf2, runtime, "C++");
		}
		else{
			console.log("Error: Could not compute password hash using pbkdf2 C++ implementation");
			console.log(passwdVal.error);
			document.getElementById("results").innerHTML="Error: Could not compute password hash using pbkdf2 C++ implementation";
		}	
	},
	displayResult: function(result, runtime, implementation){
		document.getElementById("results").innerHTML="<br><b>Runtime:</b> "+runtime+ " milliseconds"	
																+"<br><b>Implementation:</b> "+implementation				
																+"<br><b>Result:</b> "+result ;		
	},
	removeResults: function(){
		document.getElementById("results").innerHTML="Computing...";
	},
	hashPassword: function(method){
		var password = document.getElementById("password").value;
		password = password.length!=0?password:"password"; // if no password is typed into form, use the word: password
			
		var salt = document.getElementById("salt").value;
		salt = salt.length!=0?salt:"salt";
									
		var iterations = document.getElementById("iterations").value;
		iterations = iterations.length!=0?iterations:1000;
						
		var keyLength = document.getElementById("keylength").value;
		keyLength =  keyLength.length!=0?keyLength:4;
			
		var result="";
		app.removeResults();
						
		app.startstopwatch = new Date().getTime();

		if(method=="CPP"){
			app.pbkdf2CPP(password, salt, iterations, keyLength);
				
		}
		else if(method == "JS"){ // The JS file is here to compare against the speed 
								// difference with C++
			var jspbkdf2 = new PBKDF2(password, salt, iterations, keyLength);
			var status_callback = function(percent_done) {
			};
			var result_callback = function(result) {
				var end = new Date().getTime();
				var runtime = end - app.startstopwatch;								
				app.displayResult(result, runtime, "JS");
			};
			jspbkdf2.deriveKey(status_callback, result_callback);
				
		}
	},
	hashWithCPP: function() {
		app.removeResults();
		app.hashPassword("CPP");
	},
	hashWithJS: function() {
		app.removeResults();
		app.hashPassword("JS");
	}
};