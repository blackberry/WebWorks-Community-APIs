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

//deep comparison of JSON objects for testing
var Utility = {   
	areEqual: function(x, y) {
	    if((y===null) !== (x===null)) return false;
		for (var p in y) {
			if(typeof(y[p]) !== typeof(x[p])) return false;
			if((y[p]===null) !== (x[p]===null)) return false;
			switch (typeof(y[p])) {
				case 'undefined':
					if (typeof(x[p]) != 'undefined') return false;
					break;
				case 'object':
					if(y[p]!==null && x[p]!==null && (y[p].constructor.toString() !== x[p].constructor.toString() || !y[p].equals(x[p]))) return false;
					break;
				case 'function':
					if (p != 'equals' && y[p].toString() != x[p].toString()) return false;
					break;
				default:
					if (y[p] !== x[p]) return false;
			}
		}
		return true;
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
		app.testPluginCalls();
	},
	testPluginCalls: function() {
		if (community && community.gsecrypto) {

		//Test Cryptographic Hash Functions (md5, sha, sha224, sha256, sha384, sha512)
		    var testDesc;
			var testInput;
			var expectedOutput;
			var actualOutput;

		//test missing input
		    testDesc = "hash missing input";
			testInput = {"alg":"sha1"};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("error") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test no hex or b64 data
		    testDesc = "hash no input data";
			testInput = {"alg":"sha1","input":{"none":""}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("error") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test invalid hex data
		    testDesc = "hash invalid hex input";
			testInput = {"alg":"sha1","input":{"hex":"gggggggggggggggg"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("error") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test hex not mult of 2
		    testDesc = "hash odd hex input";
			testInput = {"alg":"sha1","input":{"hex":"d1ab011ca1c1cad"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("error") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test invalid b64 data
		    testDesc = "hash invalid b64 input";
			testInput = {"alg":"sha1","input":{"b64":"^asBHKHByto="}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("error") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test b64 not mult of 4
		    testDesc = "hash b64 wrong length";
			testInput = {"alg":"sha1","input":{"b64":"0asBHKHByto"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("error") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test unsupported hash algorithm
		    testDesc = "hash unsupported alg";
			testInput = {"alg":"unsupported","input":{"hex":"d1ab011ca1c1cada"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("error") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test md5 algorithm
		    testDesc = "hash md5";
			testInput = {"alg":"md5","input":{"hex":"d1ab011ca1c1cada"}};
			expectedOutput = {"output":{"b64":"jm7GNGH3w/PlnxOBpF+urw==","hex":"8e6ec63461f7c3f3e59f1381a45faeaf"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("output") ) {
				app.writeOut(testDesc + ": PASS");
			} else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test sha1 algorithm
		    testDesc = "hash sha1";
			testInput = {"alg":"sha1","input":{"hex":"d1ab011ca1c1cada"}};
			expectedOutput = {"output":{"b64":"j9gGrvjhk9wVx00cQhzonmbDqJE=","hex":"8fd806aef8e193dc15c74d1c421ce89e66c3a891"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("output") 
			  && actualOutput.output.hasOwnProperty("b64")
			  && actualOutput.output.hasOwnProperty("hex")
			  && actualOutput.output.b64 == expectedOutput.output.b64
			  && actualOutput.output.hex == expectedOutput.output.hex ) {
				app.writeOut(testDesc + ": PASS");
		    } else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}
			

		//test sha224 algorithm
			testDesc = "hash sha224";
		    testInput = {"alg":"sha224","input":{"hex":"d1ab011ca1c1cada"}};
			expectedOutput = {"output":{"b64":"E6X0JQ6zn1pMlzeXUFjHWFEFImfQPjs58VtLdQ==","hex":"13a5f4250eb39f5a4c9737975058c75851052267d03e3b39f15b4b75"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("output") 
			  && actualOutput.output.hasOwnProperty("b64")
			  && actualOutput.output.hasOwnProperty("hex")
			  && actualOutput.output.b64 == expectedOutput.output.b64
			  && actualOutput.output.hex == expectedOutput.output.hex ) {
				app.writeOut(testDesc + ": PASS");
		    } else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test sha256 algorithm
		    testDesc = "hash sha256";	
		    testInput = {"alg":"sha256","input":{"hex":"d1ab011ca1c1cada"}};
			expectedOutput = {"output":{"b64":"DeSIeJpuAMxznF/fZBeDiwbT3h+dpLpzM+nSLB5Vq0U=","hex":"0de488789a6e00cc739c5fdf6417838b06d3de1f9da4ba7333e9d22c1e55ab45"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("output") 
			  && actualOutput.output.hasOwnProperty("b64")
			  && actualOutput.output.hasOwnProperty("hex")
			  && actualOutput.output.b64 == expectedOutput.output.b64
			  && actualOutput.output.hex == expectedOutput.output.hex ) {
				app.writeOut(testDesc + ": PASS");
		    } else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test sha384 algorithm
		    testDesc = "hash sha384";	
		    testInput = {"alg":"sha384","input":{"hex":"d1ab011ca1c1cada"}};
			expectedOutput = {"output":{"b64":"VJOLaK22BogUFLVk1ZV2WO5/BofQjd46PTs3jV9+UpHZIju/hwjeiISlD++cdUrz","hex":"54938b68adb606881414b564d5957658ee7f0687d08dde3a3d3b378d5f7e5291d9223bbf8708de8884a50fef9c754af3"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("output") 
			  && actualOutput.output.hasOwnProperty("b64")
			  && actualOutput.output.hasOwnProperty("hex")
			  && actualOutput.output.b64 == expectedOutput.output.b64
			  && actualOutput.output.hex == expectedOutput.output.hex ) {
				app.writeOut(testDesc + ": PASS");
		    } else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		//test sha512 algorithm
		    testDesc = "hash sha512";	
		    testInput = {"alg":"sha512","input":{"hex":"d1ab011ca1c1cada"}};
			expectedOutput = {"output":{"b64":"IZWUTZXA74Z1cAvpUYL9iXBOpiNx9QvHtGAG2mh1IqAt5N/VSys29tCDvhVN1qrwDS45YWl541OMj09kD5CUSg==","hex":"2195944d95c0ef8675700be95182fd89704ea62371f50bc7b46006da687522a02de4dfd54b2b36f6d083be154dd6aaf00d2e39616979e3538c8f4f640f90944a"}};
			actualOutput = community.gsecrypto.hash(testInput);

			if ( actualOutput.hasOwnProperty("output") 
			  && actualOutput.output.hasOwnProperty("b64")
			  && actualOutput.output.hasOwnProperty("hex")
			  && actualOutput.output.b64 == expectedOutput.output.b64
			  && actualOutput.output.hex == expectedOutput.output.hex ) {
				app.writeOut(testDesc + ": PASS");
		    } else {
				app.writeOut(testDesc + ": FAIL: " + JSON.stringify(actualOutput) );
			}

		} else {
			app.writeOut("Plugin was not found");
		}
	},
	writeOut: function(message) {
		var output = document.getElementById('results');
		output.innerText = output.innerText + message;
		output.appendChild(document.createElement('br'));
		console.log(message);
	}

};
