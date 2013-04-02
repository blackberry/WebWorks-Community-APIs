
/*
* Copyright 2013 Research In Motion Limited.
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

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

var barcodescanner,
	_event = require("../../lib/event");

JNEXT.BarcodeScanner = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libBarcodeScanner")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libBarcodeScanner.BarcodeScannerJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	// ************************
	// Enter your methods here
	// ************************

	// Fired by the Event framework (used by asynchronous callbacks)
	self.onEvent = function (strData) {
		var arData = strData.split(" "),
			strEventDesc = arData[0],
			jsonData;
		// Event names are set in native code when fired,
		// and must be checked here.
		if (strEventDesc === "community.barcodescanner.codefound.native") {
			// Slice off the event name and the rest of the data is our JSON
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.barcodescanner.codefound", JSON.parse(jsonData));
			// self.stopRead();
		}
		else if ( strEventDesc === "community.barcodescanner.errorfound.native") {
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.barcodescanner.errorfound", JSON.parse(jsonData));
			// self.stopRead();
		}
		else if ( strEventDesc === "community.barcodescanner.frameavailable.native") {
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.barcodescanner.frameavailable", JSON.parse(jsonData));
		}
		else if ( strEventDesc === "community.barcodescanner.started.native"){
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.barcodescanner.started", JSON.parse(jsonData));
		}
		else if ( strEventDesc === "community.barcodescanner.ended.native") {
			jsonData = arData.slice(1, arData.length).join(" ");
			_event.trigger("community.barcodescanner.ended", JSON.parse(jsonData));
		}
	};

	// Thread methods
	self.startRead = function () {
		return JNEXT.invoke(self.m_id, "startRead");
	};
	self.stopRead = function () {
		return JNEXT.invoke(self.m_id, "stopRead");
	};

	// ************************
	// End of methods to edit
	// ************************
	self.m_id = "";

	self.getInstance = function () {
		if (!hasInstance) {
			hasInstance = true;
			self.init();
		}
		return self;
	};

};

barcodescanner = new JNEXT.BarcodeScanner();

module.exports = {
	barcodescanner: barcodescanner
};
