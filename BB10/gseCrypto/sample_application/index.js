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
(function() {
	var meta = document.createElement("meta");
	meta.setAttribute('name', 'viewport');
	var content = 'initial-scale=';
	content += 1 / window.devicePixelRatio;
	content += ',user-scalable=no';
	meta.setAttribute('content', content);
	document.getElementsByTagName('head')[0].appendChild(meta);
})();
var rippleLoaded = false;

function log(toLog) {
	var logDiv = document.getElementById("log");
	logDiv.innerHTML += toLog;
	logDiv.className = "output";
}

function hash() {
	var val = document.getElementById('toHash').value;
	var select = document.getElementById('hashAlg');
	var alg = select.options[select.selectedIndex].value;
	var output = document.getElementById('hashOutput');
	output.value = "";

	var result = community.gseCrypto.hash({
		input : {
			raw : val
		},
		alg : alg
	});
	if ("error" in result) {
		alert(result.error);
		return;
	}
	output.value = result.output.hex;
}

function generateAESKey() {
	var select = document.getElementById("aesKeySize");
	var keySize = parseInt(select.options[select.selectedIndex].value);
	var aesKeyInput = document.getElementById("aesKey");
	aesKeyInput.value = "";
	var result = community.gseCrypto.generateKey({
		alg : "aes",
		keySize : keySize
	});
	if ("error" in result) {
		alert(result.error);
		return;
	}
	aesKeyInput.value = result.key.hex;
}

function generateAESIV() {
	var select = document.getElementById("aesIV");
	select.value = "";
	var result = community.gseCrypto.random({
		size : 16
	});
	if ("error" in result) {
		alert(result.error);
		return;
	}
	select.value = result.output.hex;
}

function encryptAES() {
	var data64 = pkcs5Pad(document.getElementById('aesText').value);

	var iv = document.getElementById("aesIV").value;

	var aesKey = document.getElementById("aesKey").value;

	var params = {
		alg : "aes",
		mode : "cbc",
		key : {
			hex : aesKey
		},
		input : {
			b64 : data64
		},
		iv : {
			hex : iv
		}
	};
	var cipherText = document.getElementById("aesCipherText");
	cipherText.value = "";
	var result = community.gseCrypto.encrypt(params);
	if ("error" in result) {
		alert(result.error);
		return;
	}
	cipherText.value = result.output.hex;
}

function decryptAES() {
	var cipherText = document.getElementById("aesCipherText").value;

	var iv = document.getElementById("aesIV").value;

	var aesKey = document.getElementById("aesKey").value;

	var params = {
		alg : "aes",
		mode : "cbc",
		key : {
			hex : aesKey
		},
		input : {
			hex : cipherText
		},
		iv : {
			hex : iv
		}
	};

	var plainText = document.getElementById("aesPlainText");
	plainText.value = "";
	
	var result = community.gseCrypto.decrypt(params);
	if ("error" in result) {
		alert(result.error);
		return;
	}
	try {
		plainText.value = pkcs5Verify(result.output.b64);
	} catch (e) {
		alert(e);
	}
}

document.addEventListener('webworksready', function(e) {
	// This is a hack for Ripple calling webworksready multiple times
	if (rippleLoaded)
		return;
	rippleLoaded = true;
}, false);
