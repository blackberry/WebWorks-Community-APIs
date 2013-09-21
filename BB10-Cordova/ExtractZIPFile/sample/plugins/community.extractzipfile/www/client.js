/*
* Copyright (c) 2013 BlackBerry Limited
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
	_ID = "community.extractzipfile",
	exec = cordova.require("cordova/exec");


_self.extractFile = function (zipFile, successCallback,
					failureCallback,
					filename) {

	var success = function (data, response) {
			successCallback(JSON.parse(data)["result"]);
		},
		fail = function (data, response) {
			failureCallback(JSON.parse(data)["result"]);
		};
	exec(success, fail, _ID, "extractFile", { zip: zipFile, file: filename });
};

module.exports = _self;
