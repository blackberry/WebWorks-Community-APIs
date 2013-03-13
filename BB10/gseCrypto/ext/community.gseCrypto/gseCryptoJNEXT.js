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

var gseCrypto,
	_event = require("../../lib/event");

JNEXT.GSECrypto = function () {
	var self = this,
		hasInstance = false;

	self.getId = function () {
		return self.m_id;
	};

	self.init = function () {
		if (!JNEXT.require("libgseCrypto")) {
			return false;
		}

		self.m_id = JNEXT.createObject("libgseCrypto.GSECryptoJS");

		if (self.m_id === "") {
			return false;
		}

		JNEXT.registerEvents(self);
	};

	self.hash = function (input) {
		return JSON.parse(JNEXT.invoke(self.m_id, "hash " + JSON.stringify(input)));
	};

	self.generateKey = function (input) {
		return JSON.parse(JNEXT.invoke(self.m_id, "generateKey " + JSON.stringify(input)));
	};

/*	
	self.encrypt = function (input) {
		return JSON.parse(JNEXT.invoke(self.m_id, "encrypt " + JSON.stringify(input)));
	};

	self.decrypt = function (input) {
		return JSON.parse(JNEXT.invoke(self.m_id, "decrypt " + JSON.stringify(input)));
	};
	
	self.sign = function (input) {
		return JSON.parse(JNEXT.invoke(self.m_id, "sign " + JSON.stringify(input)));
	};
	
	self.verify = function (input) {
		return JSON.parse(JNEXT.invoke(self.m_id, "verify " + JSON.stringify(input)));
	};

*/

	self.getInstance = function () {
		if (!hasInstance) {
			hasInstance = true;
			self.init();
		}
		return self;
	};

};

gseCrypto = new JNEXT.GSECrypto();

module.exports = {
	gseCrypto: gseCrypto
};
