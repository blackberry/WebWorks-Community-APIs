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

var barcodescanner = require("./barcodescannerJNEXT").barcodescanner,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// methods to start and stop scanning
	startRead: function (success, fail) {
		barcodescanner.getInstance().startRead();
		success();
	},
	stopRead: function (success, fail) {
		barcodescanner.getInstance().stopRead();
		success();
	}
};