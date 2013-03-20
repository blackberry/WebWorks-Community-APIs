/**
 * UTF-8 encode a String, PKCS#5 pad it, return the base64 encoding of the
 * result.
 */
function pkcs5Pad(data, blocksize) {
	if (blocksize == undefined) {
		blocksize = 16;
	}

	var dataEncoded = blackberrynfc.ndef.util.utf8.stringToUnicode(data);
	var paddingLength = blocksize - (dataEncoded.length % blocksize);
	do {
		dataEncoded.push(paddingLength);
	} while (dataEncoded.length % blocksize != 0);

	return blackberrynfc.ndef.util.base64.intArrayToBase64(dataEncoded);
}

function pkcs5Verify(data64, blocksize) {
	if (blocksize == undefined) {
		blocksize = 16;
	}

	var dataArray = blackberrynfc.ndef.util.base64.base64ToIntArray(data64);
	if (dataArray.length == 0) {
		throw "Failed to verify (length==0)";
	}

	var paddingLength = dataArray[dataArray.length - 1];
	if (paddingLength < 1 || paddingLength > blocksize) {
		throw "Failed to verify - padding length was " + paddingLength;
	}
	for ( var i = 0; i < paddingLength; ++i) {
		var last = dataArray.pop();
		if (last != paddingLength) {
			throw "Failed to verify - unexpected padding";
		}
	}

	return blackberrynfc.ndef.util.utf8.unicodeToString(dataArray);
}
