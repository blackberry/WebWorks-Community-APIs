/*
* Copyright 2013-2015 BlackBerry Limited.
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

(function(cordova) {
	var _ID = "com.blackberry.community.barcodescanner";
	var exec = cordova.require("cordova/exec");

	var reading, timeout = null;
	var codefoundCallback, errorfoundCallback;
	var sleepPrevented = false;

	function BarcodeScanner() {	}

	BarcodeScanner.prototype.startRead = function (succ, fail, args) {
		if (reading) return "Stop Scanning before scanning again";

		blackberry.app.lockOrientation("portrait-primary");

		var success = function (data, response) {
			var arData = data.split(" "),
				receivedEvent = arData[0]
				
			if(arData.length > 2){
				var i;
				for(i=2; i<arData.length; i++)
					arData[1] += " " + arData[i];
			}

			if(receivedEvent == "community.barcodescanner.started.native") {
				reading = true;
			}
			else if(receivedEvent == "community.barcodescanner.codefound.native") {
				var codeFoundData = JSON.parse(arData[1]);
				console.log(codeFoundData);
				succ(codeFoundData);
			}
			else if(receivedEvent == "community.barcodescanner.errorfound.native") {
				var errorData = JSON.parse(arData[1]);
				fail(errorData);
			}
		},
		failure = function (data, response) {
			fail(data);
		};

		// Turn on prevent sleep, if it's in the app
		if (typeof community !== "undefined" && typeof community.preventsleep !== "undefined") {
			if (!community.preventsleep.isSleepPrevented) {
				community.preventsleep.setPreventSleep(true);
				sleepPrevented = true;
			}
		}
		exec(success, failure, _ID, "startRead", null, false);
	};

	BarcodeScanner.prototype.stopRead = function (succ, fail) {
		reading = false;
		blackberry.app.unlockOrientation();
		
		var success = function (data, response) {
			var arData = data.split(" "),
				receivedEvent = arData[0]

			if(receivedEvent == "community.barcodescanner.ended.native") {
				var successData = JSON.parse(arData[1]);
				succ(successData);
			}
		},
		failure = function (data, response) {
			fail(data)
		};
		// Return sleep setting to original if changed.
		if (typeof community !== "undefined" && typeof community.preventsleep !== "undefined") {
			if (sleepPrevented === true) {
				community.preventsleep.setPreventSleep(false);
				sleepPrevented = false;
			}
		}
		exec(success, failure, _ID, "stopRead", null, false)
	};

	
	cordova.addConstructor(function() {
		if(!window.plugins) window.plugins = {};
		window.plugins.barcodeScanner = new BarcodeScanner();
	});

})(window.cordova || window.Cordova);
