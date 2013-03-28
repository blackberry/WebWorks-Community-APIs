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

var _self = {},
	_ID = require("./manifest.json").namespace;

	_self.startRead = function (codeFound, errorFound, frameAvailable, successStart) {
		if ( typeof(successStart) == "function" ) {
			window.webworks.event.once(_ID, "community.barcodescanner.started", successStart);
		}
		if ( typeof(frameAvailable) == "function" ) {
			window.webworks.event.add(_ID, "community.barcodescanner.frameavailable", frameAvailable);
		}
		if ( typeof(errorFound) == "function" ) {
			window.webworks.event.once(_ID, "community.barcodescanner.errorfound", errorFound);
		}
		if ( typeof(codeFound) == "function" ) {
			window.webworks.event.once(_ID, "community.barcodescanner.codefound", codeFound);
		}
		return window.webworks.execAsync(_ID, "startRead", null);
	};

	_self.stopRead = function (codeFound, errorFound, frameAvailable, successfulEnd) {
		if ( typeof(errorFound) == "function" ) {
			window.webworks.event.once(_ID, "community.barcodescanner.errorfound", errorFound);
		}
		if ( typeof(successfulEnd) == "function" ) {
			window.webworks.event.once(_ID, "community.barcodescanner.ended", successfulEnd);
		}
		if ( typeof(frameAvailable) == "function" ) {
			window.webworks.event.remove(_ID, "community.barcodescanner.frameavailable", frameAvailable);
		}
		if ( typeof(codeFound) == "function" ) {
			window.webworks.event.remove(_ID, "community.barcodescanner.codefound", codeFound);
		}
		return window.webworks.execAsync(_ID, "stopRead", null);
	};

module.exports = _self;
