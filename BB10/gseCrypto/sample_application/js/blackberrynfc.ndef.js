/*

Copied from:

https://raw.github.com/blackberry/BB10-WebWorks-Community-Samples/master/nfc/ndefTagReading/js/blackberrynfc.ndef.js
	
*/

/* Copyright 2012 Research In Motion Limited.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
var blackberrynfc = new Array();
blackberrynfc.ndef = new Array();
blackberrynfc.ndef.uri = new Array();
blackberrynfc.ndef.uri.prefixesByNumber = new Array();
blackberrynfc.ndef.uri.prefixesByNumber[0] = "";
blackberrynfc.ndef.uri.prefixesByNumber[1] = "http://www.";
blackberrynfc.ndef.uri.prefixesByNumber[2] = "https://www.";
blackberrynfc.ndef.uri.prefixesByNumber[3] = "http://";
blackberrynfc.ndef.uri.prefixesByNumber[4] = "https://";
blackberrynfc.ndef.uri.prefixesByNumber[5] = "tel:";
blackberrynfc.ndef.uri.prefixesByNumber[6] = "mailto:";
blackberrynfc.ndef.uri.prefixesByNumber[7] = "ftp://anonymous:anonymous@";
blackberrynfc.ndef.uri.prefixesByNumber[8] = "ftp://ftp.";
blackberrynfc.ndef.uri.prefixesByNumber[9] = "ftps://";
blackberrynfc.ndef.uri.prefixesByNumber[10] = "sftp://";
blackberrynfc.ndef.uri.prefixesByNumber[11] = "smb://";
blackberrynfc.ndef.uri.prefixesByNumber[12] = "nfs://";
blackberrynfc.ndef.uri.prefixesByNumber[13] = "ftp://";
blackberrynfc.ndef.uri.prefixesByNumber[14] = "dav://";
blackberrynfc.ndef.uri.prefixesByNumber[15] = "news:";
blackberrynfc.ndef.uri.prefixesByNumber[16] = "telnet://";
blackberrynfc.ndef.uri.prefixesByNumber[17] = "imap:";
blackberrynfc.ndef.uri.prefixesByNumber[18] = "rtsp://";
blackberrynfc.ndef.uri.prefixesByNumber[19] = "urn:";
blackberrynfc.ndef.uri.prefixesByNumber[20] = "pop:";
blackberrynfc.ndef.uri.prefixesByNumber[21] = "sip:";
blackberrynfc.ndef.uri.prefixesByNumber[22] = "sips:";
blackberrynfc.ndef.uri.prefixesByNumber[23] = "tftp:";
blackberrynfc.ndef.uri.prefixesByNumber[24] = "btspp://";
blackberrynfc.ndef.uri.prefixesByNumber[25] = "btl2cap://";
blackberrynfc.ndef.uri.prefixesByNumber[26] = "btgoep://";
blackberrynfc.ndef.uri.prefixesByNumber[27] = "tcpobex://";
blackberrynfc.ndef.uri.prefixesByNumber[28] = "irdaobex://";
blackberrynfc.ndef.uri.prefixesByNumber[29] = "file://";
blackberrynfc.ndef.uri.prefixesByNumber[30] = "urn:epc:id:";
blackberrynfc.ndef.uri.prefixesByNumber[31] = "urn:epc:tag:";
blackberrynfc.ndef.uri.prefixesByNumber[32] = "urn:epc:pat:";
blackberrynfc.ndef.uri.prefixesByNumber[33] = "urn:epc:raw:";
blackberrynfc.ndef.uri.prefixesByNumber[34] = "urn:epc:";
blackberrynfc.ndef.uri.prefixesByNumber[35] = "urn:nfc:";

blackberrynfc.ndef.uri.prefixesByValue = {
	"" : 0,
	"http://www." : 1,
	"https://www." : 2,
	"http://" : 3,
	"https://" : 4,
	"tel:" : 5,
	"mailto:" : 6,
	"ftp://anonymous:anonymous@" : 7,
	"ftp://ftp." : 8,
	"ftps://" : 9,
	"sftp://" : 10,
	"smb://" : 11,
	"nfs://" : 12,
	"ftp://" : 13,
	"dav://" : 14,
	"news:" : 15,
	"telnet://" : 16,
	"imap:" : 17,
	"rtsp://" : 18,
	"urn:" : 19,
	"pop:" : 20,
	"sip:" : 21,
	"sips:" : 22,
	"tftp:" : 23,
	"btspp://" : 24,
	"btl2cap://" : 25,
	"btgoep://" : 26,
	"tcpobex://" : 27,
	"irdaobex://" : 28,
	"file://" : 29,
	"urn:epc:id:" : 30,
	"urn:epc:tag:" : 31,
	"urn:epc:pat:" : 32,
	"urn:epc:raw:" : 33,
	"urn:epc:" : 34,
	"urn:nfc:" : 35
};

blackberrynfc.ndef.tnf = {
		EMPTY : 0,
		WELL_KNOWN : 1,
		MEDIA : 2,
		ABSOLUTE_URI : 3,
		EXTERNAL : 4,
		UNKNOWN : 5,
		UNCHANGED : 6,
		RESERVED : 7
	};

blackberrynfc.ndef.recommendedAction = {
		DO_THE_ACTION : 0,
		SAVE_FOR_LATER : 1,
		OPEN_FOR_EDIT :2
};

blackberrynfc.ndef.message = new Object();

blackberrynfc.ndef.message.Record = function() {
	blackberrynfc.ndef.util.log.log(">> Record()", 1);
	this.encoded = new Array();
	blackberrynfc.ndef.util.log.log("<< Record()", -1);
};

blackberrynfc.ndef.message.Record.prototype.getEncoded = function(encoding) {
	blackberrynfc.ndef.util.log.log(">> getEncoded()", 1);
	try {
		if ("base64"==encoding) {
			return blackberrynfc.ndef.util.base64.intArrayToBase64(this.encoded);
		}
		if ("invoke"==encoding) {
			var toReturn = "";
			for (var i=0; i<this.encoded.length; ++i) {
				toReturn += String.fromCharCode(this.encoded[i]);
			}
			return toReturn;
		}
		return this.encoded;
	} finally {
		blackberrynfc.ndef.util.log.log("<< getEncoded", -1);
	}
};

blackberrynfc.ndef.message.bytesAsHTMLHex = function(array,offset,length) {
	var hex="0123456789abcdef";
	var toReturn = "<span class='hex'>";
	while(length!=0) {
		var h = "";
		var c = "";
		for (var i=0; i<16 && length!=0; ++i) {
			var b = array[offset++];
			if (i!=0) {
				h += " ";
			}
			h += hex[(b>>4)&0xf];
			h += hex[b&0xf];
			if (b>=32 && b<=127) {
				c += "&#" + b + ";";
			} else {
				c += "<span class='unknown'>.</span>";
			}
			length--;
		}
		while(h.length<52) {
			h+= " ";
		}
		toReturn += h + c;
		if (length!=0) {
			toReturn += "<br />";
		}
	}
	toReturn += "</span>";
	return toReturn;
};

blackberrynfc.ndef.message.Record.prototype.getEncodedAsHTML = function() {
	var offset = 0;
	var encoded = this.getEncoded();
	var toReturn = "";

	toReturn += "<div class='blackberrynfc'>";
	toReturn += "<div class='pair'>";
	toReturn += "<div class='title'>Flags</div><div class='value'>";
	var flags = encoded[offset++];
	if ((flags & 128)!=0) {
		toReturn += "MB ";
	}
	if ((flags & 64)!=0) {
		toReturn += "ME ";
	}
	if ((flags & 32)!=0) {
		toReturn += "CF ";
	}
	var sr = (flags & 16)!=0;
	toReturn += sr ? "SR " : "";
	
	var il = (flags & 8)!=0;
	toReturn += il ? "IL " : "";
	
	toReturn += "TNF: " + this.getTNF();
	
	toReturn += "</div></div>";
	
	toReturn += "<div class='pair'>";
	toReturn += "<div class='title'>Type Length</div><div class='value'>";
	toReturn += blackberrynfc.ndef.message.bytesAsHTMLHex(encoded,offset,1);
	toReturn += "</div></div>";
	var typeLength = encoded[offset++];
	
	toReturn += "<div class='pair'>";
	toReturn += "<div class='title'>Payload Length</div><div class='value'>";
	toReturn += blackberrynfc.ndef.message.bytesAsHTMLHex(encoded,offset,sr ? 1 : 4);
	toReturn += "</div></div>";
	var payloadLength = 0;
	if (sr) {
		payloadLength = encoded[offset++];
	} else {
		for (var i =0; i<4; ++i) {
			payloadLength <<= 8;
			payloadLength |= encoded[offset++];
		}
	}
	
	var idLength = 0;
	if (il) {
		idLength = encoded[offset++];
	}
	
	toReturn += "<div class='pair'><div class='title'>Type</div><div class='value'>";
	toReturn += this.getType();
	toReturn += "<br />";
	toReturn += blackberrynfc.ndef.message.bytesAsHTMLHex(encoded,offset,typeLength);
	toReturn += "</div></div>";
	offset += typeLength;
	
	if (il) {
		toReturn += "<div class='pair'><div class='title'>ID</div><div class='value'>";
		toReturn += blackberrynfc.ndef.message.bytesAsHTMLHex(encoded,offset,idLength);
		toReturn += "</div></div>";
		offset+= idLength;
	}
	
	toReturn += "<div class='pair'><div class='title'>Payload</div><div class='value'>";
	toReturn += blackberrynfc.ndef.message.bytesAsHTMLHex(encoded,offset,payloadLength);
	toReturn += "</div></div>";
	offset+=payloadLength;

	if (this.getRecords) {
		var records = this.getRecords();
		toReturn += "<div class='subrecords'>";
		toReturn += "<h2>Subrecords</h2>";
		toReturn += "Count: " + records.length + "<br />";
		for (var i=0; i<records.length; ++i) {
			toReturn += "<h3>" + i + "</h3>";
			toReturn += records[i].getEncodedAsHTML();
		}
		toReturn += "</div>";
	}
	
	toReturn += "</div>";
	
	return toReturn;
};

blackberrynfc.ndef.message.Record.prototype.setEncoded = function(encoded) {
	blackberrynfc.ndef.util.log.log(">> setEncoded(" + encoded + ")", 1);
	try {
		this.encoded = new Array();
		for ( var i = 0; i < encoded.length; ++i) {
			this.encoded.push(encoded[i]);
		}
	} finally {
		blackberrynfc.ndef.util.log.log("<< setEncoded", -1);
	}
};

blackberrynfc.ndef.message.Record.prototype.getEncodedAsHex = function() {
	blackberrynfc.ndef.util.log.log(">> getEncodedAsHex()", 1);
	try {
		var toReturn = "";
		var hex = "0123456789abcdef";
		for ( var i = 0; i < this.encoded.length; ++i) {
			var next = this.encoded[i];
			var a = (next >> 4) & 15;
			toReturn += hex.substring(a, a + 1);
			a = next & 15;
			toReturn += hex.substring(a, a + 1);
		}
		return toReturn;
	} finally {
		blackberrynfc.ndef.util.log.log("<< getEncodedAsHex", -1);
	}
};

blackberrynfc.ndef.message.Record.prototype.getTNF = function() {
	blackberrynfc.ndef.util.log.log(">> getTNF()", 1,3);
	try {
		this.enforceInitialised();
		return this.encoded[0] & 7;
	} finally {
		blackberrynfc.ndef.util.log.log("<< getTNF", -1,3);
	}
};

blackberrynfc.ndef.message.Record.prototype.getType = function() {
	blackberrynfc.ndef.util.log.log(">> getType()", 1);
	try {
		this.enforceInitialised();
		var flags = this.encoded[0];
		var sr = (flags & 16) != 0;
		var il = (flags & 8) != 0;

		var typeLength = this.getByte(1);
		var typeOffset = 2 + (sr ? 1 : 4) + (il ? 1 : 0);

		var toReturn = this.fromSection(typeOffset, typeLength);
		return toReturn;
	} finally {
		blackberrynfc.ndef.util.log.log("<< getType", -1);
	}
};

blackberrynfc.ndef.message.Record.prototype.isType = function(tnf,type) {
	return tnf==this.getTNF() && this.getType()==type;
};

blackberrynfc.ndef.message.Record.prototype.decorate = function() {
	blackberrynfc.ndef.util.log.log(">> decorate()", 1);
	try {
		var tnf = this.getTNF();
		var type = this.getType();
		if (tnf == blackberrynfc.ndef.tnf.WELL_KNOWN) {
			if (type == "Sp") {
				blackberrynfc.ndef.message.decorateSpRecord(this);
			} else if (type == "U") {
				blackberrynfc.ndef.message.decorateURIRecord(this);
			} else if (type == "T") {
				blackberrynfc.ndef.message.decorateTextRecord(this);
			} else if (type == "act") {
				blackberrynfc.ndef.message.decorateRecommendedActionRecord(this);
			} else if (type == "s") {
				blackberrynfc.ndef.message.decorateSizeRecord(this);
			} else if (type == "t") {
				blackberrynfc.ndef.message.decorateTypeRecord(this);
			} else {
				blackberrynfc.ndef.util.log.log("Did not know how to decorate " + type);
			}
		} else {
			blackberrynfc.ndef.util.log.log("Did not know how to decorate " + tnf + ": " + type);
		}
	} finally {
		blackberrynfc.ndef.util.log.log("<< decorate", -1);
	}
};

blackberrynfc.ndef.message.Record.prototype.fromSection = function(offset,
		length) {
	var toReturn = "";
	for ( var i = offset; i < offset + length; ++i) {
		toReturn += String.fromCharCode(this.encoded[i]);
	}
	return toReturn;
};

blackberrynfc.ndef.message.Record.prototype.getPayload = function(encoding) {
	this.enforceInitialised();
	var flags = this.encoded[0];
	var sr = (flags & 16) != 0;
	var il = (flags & 8) != 0;

	var offset = 1;
	var typeLength = this.getByte(offset++);
	var payloadLength = 0;
	if (sr) {
		payloadLength = this.getByte(offset++);
	} else {
		for ( var i = 0; i < 4; ++i) {
			payloadLength *= 256;
			payloadLength |= this.getByte(offset++);
		}
	}
	var idLength = 0;
	if (il) {
		idLength = this.getByte(offset++);
	}
	offset += typeLength;
	offset += idLength;
	
	var toReturn = this.encoded.slice(offset, offset + payloadLength);
	if ("base64"==encoding) {
		return blackberrynfc.ndef.util.base64.intArrayToBase64(toReturn);
	} else if ("data"==encoding) {
		return "data:" + this.getType() + ";base64," + blackberrynfc.ndef.util.base64.intArrayToBase64(toReturn);
	}
	return toReturn;
};

blackberrynfc.ndef.message.Record.prototype.getByte = function(which) {
	if (which < 0) {
		throw "which < 0 " + which;
	} else if (which >= this.encoded.length) {
		throw "which >= encoded.length " + which;
	}
	return this.encoded[which];
};

blackberrynfc.ndef.message.Record.prototype.enforceInitialised = function() {
	if (this.encoded.length == 0) {
		throw "Encoded record is empty";
	}
};
blackberrynfc.ndef.message.Record.prototype.getMB = function() {
	this.enforceInitialised();
	return (this.getByte(0) & 128) != 0;
};
blackberrynfc.ndef.message.Record.prototype.getME = function() {
	this.enforceInitialised();
	return (this.getByte(0) & 64) != 0;
};
blackberrynfc.ndef.message.Record.prototype.setMB = function(mb) {
	this.enforceInitialised();
	this.encoded[0] &= (255 - 128);
	this.encoded[0] |= (mb ? 128 : 0);
};
blackberrynfc.ndef.message.Record.prototype.setME = function(me) {
	this.enforceInitialised();
	this.encoded[0] &= (255 - 64);
	this.encoded[0] |= (me ? 64 : 0);
};

blackberrynfc.ndef.message.Record.prototype.setContent = function(tnf, type,
		payload) {
	this.encoded = new Array();
	var sr = payload.length <= 255 ? 1 : 0;
	this.encoded.push((tnf & 7) | (sr * 16));
	this.encoded.push(type.length);
	if (sr == 0) {
		var shift = 24;
		for ( var i = 0; i < 4; ++i) {
			this.encoded.push((payload.length >> shift) & 255);
			shift -= 8;
		}
	} else {
		this.encoded.push(payload.length);
	}
	for ( var i = 0; i < type.length; ++i) {
		this.encoded.push(type.charCodeAt(i));
	}
	for ( var i = 0; i < payload.length; ++i) {
		this.encoded.push(payload[i]);
	}
	this.setMB(true);
	this.setME(true);
};

blackberrynfc.ndef.message.createRecord = function(tnf,type,payload) {
	var toReturn = new blackberrynfc.ndef.message.Record();
	toReturn.setContent(tnf,type,payload);
	toReturn.decorate();
	return toReturn;
};

blackberrynfc.ndef.message.createURIRecord = function(uri) {
	blackberrynfc.ndef.util.log.log(">> createURIRecord(" + uri + ")", 1);
	try {
		var toReturn = new blackberrynfc.ndef.message.Record();
		var payload = new Array();
		var bestMatch = -1;
		var bestMatchLength = -1;
		for ( var i = 0; i < blackberrynfc.ndef.uri.prefixesByNumber.length; ++i) {
			var candidate = blackberrynfc.ndef.uri.prefixesByNumber[i];
			if (uri.indexOf(candidate) == 0) {
				if (bestMatch == -1 || bestMatchLength < candidate.length) {
					bestMatch = i;
					bestMatchLength = candidate.length;
				}
			}
		}
		if (bestMatch == -1) {
			bestMatch = 0;
			bestMatchLength = 0;
		}
		payload.push(bestMatch);
		for ( var i = bestMatchLength; i < uri.length; ++i) {
			payload.push(uri.charCodeAt(i));
		}

		toReturn.setContent(blackberrynfc.ndef.tnf.WELL_KNOWN, "U", payload);
		toReturn.decorate();

		return toReturn;
	} finally {
		blackberrynfc.ndef.util.log.log("<< createURIRecord", -1);
	}
};

blackberrynfc.ndef.message.createSizeRecord = function(size) {
	blackberrynfc.ndef.util.log.log(">> createSizeRecord(" + size + ")",1);
	try {
		var toReturn = new blackberrynfc.ndef.message.Record();
		var payload = new Array();
		payload[0] = (size>>24) & 0xff;
		payload[1] = (size>>16) & 0xff;
		payload[2] = (size>>8) & 0xff;
		payload[3] = size & 0xff;
		
		toReturn.setContent(blackberrynfc.ndef.tnf.WELL_KNOWN,"s",payload);
		toReturn.decorate();
		
		return toReturn;
	} finally {
		blackberrynfc.ndef.util.log.log("<< createSizeRecord",-1);
	}
};

blackberrynfc.ndef.message.createTypeRecord = function(type) {
	blackberrynfc.ndef.util.log.log(">> createTypeRecord(" + type + ")",1);
	try {
		var toReturn = new blackberrynfc.ndef.message.Record();
		var payload = blackberrynfc.ndef.util.utf8.stringToUnicode(type);
		
		toReturn.setContent(blackberrynfc.ndef.tnf.WELL_KNOWN,"t",payload);
		return toReturn;
	} finally {
		blackberrynfc.ndef.util.log.log("<< createTypeRecord",-1);
	}
};

blackberrynfc.ndef.message.createRecommendedActionRecord = function(action) {
	blackberrynfc.ndef.util.log.log(">> createRecommendedActionRecord(" + action + ")",1);
	try {
		var toReturn = new blackberrynfc.ndef.message.Record();
		var payload = new Array();
		payload.push(action);
		
		toReturn.setContent(blackberrynfc.ndef.tnf.WELL_KNOWN, "act", payload);
		toReturn.decorate();
		
		return toReturn;
	} finally {
		blackberrynfc.ndef.util.log.log("<< createRecommendedActionRecord",-1);
	}
	
};

blackberrynfc.ndef.message.decodeRecords = function(encoding) {
	blackberrynfc.ndef.util.log.log(">> decodeRecords(" + encoding + ")");
	var toReturn = new Array();
	var offset = 0;
	
	encoding = blackberrynfc.ndef.util.encodingToIntArray(encoding);

	while (offset < encoding.length) {
		var start = offset;
		var remaining = encoding.length - offset;
		var flags = encoding[offset++];
		var minLength = 1 + 1;
		var sr = (flags & 16) != 0;
		var il = (flags & 8) != 0;
		minLength += sr ? 1 : 4;
		minLength += il ? 1 : 0;
		if (minLength > remaining) {
			// ***** RPW
			// do something...
		} else {
			var typeLength = encoding[offset++];
			var payloadLength = 0;
			if (sr) {
				payloadLength = encoding[offset++];
			} else {
				for ( var i = 0; i < 4; ++i) {
					payloadLength <<= 8;
					payloadLength |= encoding[offset++];
				}
			}
			var idLength = 0;
			if (il) {
				idLength = encoding[offset++];
			}
			var totalLength = minLength + typeLength + payloadLength + idLength;
			if (totalLength > remaining) {
				// ***** RPW
				// do something...
			} else {
				var nextRecord = new blackberrynfc.ndef.message.Record();
				var nextRecordEncoding = encoding.slice(start, start
						+ totalLength);
				blackberrynfc.ndef.util.log.log("Next Record Encoding: " + nextRecordEncoding);
				nextRecord.setEncoded(nextRecordEncoding);
				nextRecord.decorate();
				toReturn.push(nextRecord);

				offset = start + totalLength;
			}
		}
	}
	blackberrynfc.ndef.util.log.log("<< decodeRecords");
	return toReturn;
};

blackberrynfc.ndef.message.decorateURIRecord = function(record) {
	record.getURI = function() {
		var payload = this.getPayload();
		if (payload.length == 0) {
			return "";
		}
		var toReturn = "";
		var prefix = blackberrynfc.ndef.uri.prefixesByNumber[payload[0]];
		if (prefix) {
			toReturn += prefix;
		}
		for ( var i = 1; i < payload.length; ++i) {
			toReturn += String.fromCharCode(payload[i]);
		}
		return toReturn;
	};
};

blackberrynfc.ndef.message.createTextRecord = function(language, text) {
	var toReturn = new blackberrynfc.ndef.message.Record();
	var payload = new Array();
	payload.push(language.length & 31);
	for ( var i = 0; i < language.length & 31; ++i) {
		payload.push(language.charCodeAt(i));
	}
	var utf8 = blackberrynfc.ndef.util.utf8.stringToUnicode(text);
	payload = payload.concat(utf8);
	toReturn.setContent(blackberrynfc.ndef.tnf.WELL_KNOWN, "T", payload);
	toReturn.decorate();

	return toReturn;
};

blackberrynfc.ndef.message.decorateRecommendedActionRecord = function(record) {
	record.getRecommendedAction = function() {
		var payload = this.getPayload();
		if (payload.length==0) {
			return undefined;
		}
		return payload[0];
	};
};

blackberrynfc.ndef.message.decorateSizeRecord = function(record) {
	record.getSize = function() {
		var payload = this.getPayload();
		var toReturn = 0;
		for (var i=0; i<4 && i < payload.length; ++i) {
			toReturn <<= 8;
			toReturn |= payload[i] & 0xff;
		}
		return toReturn;
	};
};

blackberrynfc.ndef.message.decorateTypeRecord = function(record) {
	record.getMimeType = function() {
		var payload = this.getPayload();
		return blackberrynfc.ndef.util.utf8.unicodeToString(payload);
	};
};

blackberrynfc.ndef.message.decorateTextRecord = function(record) {
	record.getLanguage = function() {
		var payload = this.getPayload();
		if (payload.length == 0) {
			return "";
		}
		var toReturn = "";
		var length = payload[0] & 31;
		for ( var i = 0; i < length; ++i) {
			toReturn += String.fromCharCode(payload[i + 1]);
		}
		return toReturn;
	};
	record.getText = function() {
		var payload = this.getPayload();
		if (payload.length == 0) {
			return "";
		}
		var offset = 1 + (payload[0] & 31);
		var toReturn = blackberrynfc.ndef.util.utf8.unicodeToString(payload, offset);
		return toReturn;
	};
};

blackberrynfc.ndef.message.createSpRecord = function(records) {
	blackberrynfc.ndef.util.log.log(">> createSpRecord(" + records + ")", 1);
	try {
		var toReturn = new blackberrynfc.ndef.message.Record();
		blackberrynfc.ndef.message.decorateSpRecord(toReturn);
		toReturn.setRecords(records);
		return toReturn;
	} finally {
		blackberrynfc.ndef.util.log.log("<< createSpRecord", -1);
	}
};

blackberrynfc.ndef.message.decorateSpRecord = function(record) {
	blackberrynfc.ndef.util.log.log(">> blackberrynfc.ndef.emssage.decorateSpRecord");
	record.getURI = function() {
		for ( var i = 0; i < this.records.length; ++i) {
			var record = this.records[i];
			if (record.getTNF() == blackberrynfc.ndef.tnf.WELL_KNOWN
					&& record.getType() == "U") {
				return record.getURI();
			} else {
				// this isn't the URI record you're looking for.
			}
		}
		return undefined;
	};
	record.getText = function(language) {
		if (language == undefined) {
			language = "";
		}
		for ( var i = 0; i < this.records.length; ++i) {
			var record = this.records[i];
			if (record.getTNF() == blackberrynfc.ndef.tnf.WELL_KNOWN
					&& record.getType() == "T") {
				if (language == "" || language == record.getLanguage()) {
					return record.getText();
				}
			} else {
				// this isn't the T record you're looking for.
			}
		}
		return undefined;
	};
	record.getRecommendedAction = function() {
		for (var i =0; i<this.records.length; ++i) {
			var record = this.records[i];
			if (record.getTNF() == blackberrynfc.ndef.tnf.WELL_KNOWN && record.getType()=="act") {
				return record.getRecommendedAction();
			}
		}
		return undefined;
		
	};
	record.getSize = function() {
		for (var i=0; i<this.records.length; ++i) {
			var record = this.records[i];
			if (record.getTNF()==blackberrynfc.ndef.tnf.WELL_KNOWN && record.getType()=="s") {
				return record.getSize();
			}
		}
		return undefined;
	};
	record.getMimeType = function() {
		for (var i=0; i<this.records.length; ++i) {
			var record = this.records[i];
			if (record.getTNF()==blackberrynfc.ndef.tnf.WELL_KNOWN && record.getType()=="t") {
				return record.getMimeType();
			}
		}
		return undefined;
	};
	record.getIcons = function() {
		var toReturn = new Array();
		for (var i=0; i<this.records.length; ++i) {
			var record = this.records[i];
			if (record.getTNF()==blackberrynfc.ndef.tnf.MEDIA) {
				toReturn.push(record);
			}
		}
		return toReturn;
	};
	record.getIcon = function() {
		for (var i=0; i<this.records.length; ++i) {
			var record = this.records[i];
			if (record.getTNF()==blackberrynfc.ndef.tnf.MEDIA) {
				return record;
			}
		}
		return undefined;
	};
	record.setRecords = function(records) {
		var payload = new Array();
		var recordsCopy = new Array();
		for ( var i = 0; i < records.length; ++i) {
			var record = records[i];
			var toAdd = new blackberrynfc.ndef.message.Record();
			toAdd.setEncoded(record.getEncoded());
			toAdd.setMB(i == 0);
			toAdd.setME(i == records.length - 1);
			toAdd.decorate();
			recordsCopy.push(toAdd);
			var recordPayload = toAdd.getEncoded();
			for ( var j = 0; j < recordPayload.length; ++j) {
				payload.push(recordPayload[j]);
			}
		}
		this.records = recordsCopy;
		this.setContent(blackberrynfc.ndef.tnf.WELL_KNOWN, "Sp", payload);
	};
	record.getRecords = function() {
		return this.records;
	};
	if (!record.records) {
		record.records = new Array();
	}
	blackberrynfc.ndef.util.log.log("About to get encoded");
	try {
		if (record.getEncoded().length != 0) {
			record.records = blackberrynfc.ndef.message.decodeRecords(record
					.getPayload());
		}
	} catch (e) {
		blackberrynfc.ndef.util.log.log(e);
		throw e;
	}
	blackberrynfc.ndef.util.log.log("How did that go?");
	blackberrynfc.ndef.util.log.log("<< blackberrynfc.ndef.message.decorateSpRecord");
};

blackberrynfc.ndef.util = new Object();

blackberrynfc.ndef.util.log = new Object();
blackberrynfc.ndef.util.log.logIndent = 0;
blackberrynfc.ndef.util.log.log = function(args, direction, logLevel) {
//	if (-1 === direction) {
//		blackberrynfc.ndef.util.log.logIndent--;
//	}
//	if (3===logLevel) {
//		
//	} else {
//		var message = "<pre>";
//		for ( var i = 0; i < blackberrynfc.ndef.util.log.logIndent; ++i) {
//			message += ("    ");
//		}
//		message += args;
//		message += "</pre>";
//		message += "\n";
//		document.write(message);
//	}
//	if (1 == direction) {
//		blackberrynfc.ndef.util.log.logIndent++;
//	}
};


// _____
//( ___ \
//( ___ <
//(_____/ase 64
//
blackberrynfc.ndef.util.base64 = new Object();

blackberrynfc.ndef.util.base64.base64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

blackberrynfc.ndef.util.base64.base64ToIntArray = function(input) {
	var toReturn = new Array();
	var workValue = new Array();
	var bytes = new Array();
	var offset = 0;
	while(offset < input.length) {
		for (var i=0; i<4; ++i) {
			if (offset>=input.length) {
				workValue[i] = 64;
			} else {
				var code = input.substring(offset++,offset);
				var index = blackberrynfc.ndef.util.base64.base64Alphabet.indexOf(code);
				if (index==-1) {
					--i;
					continue;
				}
				workValue[i] = index;
			}
		}
		bytes[0] = (workValue[0] << 2 | workValue[1] >> 4) & 255;
		bytes[1] = (workValue[1] << 4 | workValue[2] >> 2) & 255; 
		bytes[2] = (workValue[2] << 6 | workValue[3]) & 255;
		
		if (workValue[3]==64 && workValue[2]==64) {
			toReturn.push(bytes[0]);
		} else if (workValue[3]==64) {
			toReturn.push(bytes[0]);
			toReturn.push(bytes[1]);
		} else {
			toReturn.push(bytes[0]);
			toReturn.push(bytes[1]);
			toReturn.push(bytes[2]);
		}
	}
	
	return toReturn;
};

blackberrynfc.ndef.util.base64.intArrayToBase64 = function(input,offset,length) {
	if (input==undefined) {
		return undefined;
	}
	if (offset==undefined) {
		offset = 0;
	}
	if (length==undefined) {
		length = input.length;
	}
	
	if (offset < 0) {
		offset = 0;
	} else if (offset>input.length) {
		return "";
	}
	
	if (length<0) {
		return "";
	} else if (offset+length > input.length) {
		return "";
	}
	
	var toReturn = "";
	
	var values = Array();
	
	for (var i=0; i<length; i+=3) {
		var a = input[i+offset];
		values[0] = (a>>2) & 63;
		values[1] = (a<<4) & 63;
		
		if (i+offset+1>=length) {
			values[2] = 64;
			values[3] = 64;
		} else {
			var b = input[i+offset+1];
			values[1] |= (b >> 4) & 63;
			values[2] = (b << 2) & 63;
			
			if (i+offset+2>=length) {
				values[3] = 64;
			} else {
				var c = input[i+offset+2];
				values[2] |= (c >> 6) & 63;
				values[3] = c & 63;
			}
		}
		
		for (var j=0; j<4; ++j) {
			toReturn +=
				blackberrynfc.ndef.util.base64.base64Alphabet.substring(values[j],values[j]+1); 
		}
	}
	
	return toReturn;
};

// _   _
//( ) ( )
//( )_( )
// \___/TF8
//
blackberrynfc.ndef.util.utf8 = new Object();

blackberrynfc.ndef.util.utf8.prefixes = Array(0, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc);

blackberrynfc.ndef.util.utf8.processCharCode = function(charCode, array) {
	if (charCode < 0x80) {
		array.push(charCode);
	} else {
		var bitCount = 0;
		var j = charCode;
		var shift = 0;
		while (j != 0) {
			++bitCount;
			j >>= 1;
		}
		;
		if (j > 31) {
			// too long for UTF8.
			return;
		}
		var count = Math.floor((bitCount + 3) / 5);
		shift = (count - 1) * 6;
		for (var i = 0; i < count; ++i) {
			var firstBit = i == 0 ? blackberrynfc.ndef.util.utf8.prefixes[count - 1] : 0x80;
			array.push(((charCode >> shift) & 0x3f) | (firstBit));
			shift -= 6;
		}
	}
};

blackberrynfc.ndef.util.utf8.stringToUnicode = function(input) {
	var toReturn = Array();

	var lastCharCode = 0;
	var lastSurrogate = false;
	for ( var i = 0; i < input.length; ++i) {
		var charCode = input.charCodeAt(i);
		if (lastSurrogate) {
			lastSurrogate = false;
			if (charCode >= 0xdc00 && charCode < 0xe000) {
				var surrogatePair = 0x10000 | ((lastCharCode & 0x3ff) << 10 ) | (charCode & 0x3ff);
				blackberrynfc.ndef.util.utf8.processCharCode(surrogatePair, toReturn);
			} else {
				blackberrynfc.ndef.util.utf8.processCharCode(lastCharCode, toReturn);
				blackberrynfc.ndef.util.utf8.processCharCode(charCode, toReturn);
			}
		} else {
			if (charCode >= 0xd800 && charCode < 0xdc00) {
				lastCharCode = charCode;
				lastSurrogate = true;
			} else {
				blackberrynfc.ndef.util.utf8.processCharCode(charCode, toReturn);
			}
		}
	}

	return toReturn;
};

blackberrynfc.ndef.util.utf8.unicodeToString = function(input,offset,length) {
	var toReturn = "";
	var remainCount = 0;
	var runningValue = 0;
	if (offset==undefined) {
		offset = 0;
	} else if (offset < 0) {
		offset = 0;
	}
	
	if (length==undefined) {
		length = input.length;
	} else if (length > input.length) {
		length = input.length;
	}
	
	for (var i=0; i<length && offset+i < input.length; ++i) {
		var c = input[i+offset];
		if (c<0 || c>=0xfe) {
			// ignore this one.
		} else if (remainCount>0) {
			if (c<128 || c > 0xbf) {
				// not a valid continuation...
				remainCount = 0;
				--i;
			} else {
				runningValue <<= 6;
				runningValue |= (c & 0x3f);
				remainCount--;
				if (remainCount==0) {
					if (runningValue >=0x10000) {
						runningValue -= 0x10000;
						var a = 0xd800 | ((runningValue>>10) & 0x3ff);
						var b = 0xdc00 | ((runningValue & 0x3ff));
						toReturn += String.fromCharCode(a,b);
					} else {
						toReturn += String.fromCharCode(runningValue);
					}
					runningValue = 0;
				}
			}
		} else {
			if (c<128) {
				toReturn += String.fromCharCode(c);
			} else if (c>0xbf) {
				if (c>=0xfc) {
					remainCount = 5;
					runningValue = (c & 1);
				} else if (c>=0xf8) {
					remainCount = 4;
					runningValue = (c & 3);
				} else if (c>=0xf0) {
					remainCount = 3;
					runningValue = (c & 7);
				} else if (c>=0xe0) {
					remainCount = 2;
					runningValue = (c & 0xf);
				} else {
					remainCount = 1;
					runningValue = (c & 0x1f);
				}
			} else {
				// not a valid value in mid stream.
			}
		} 
	}
	return toReturn;
};

blackberrynfc.ndef.util.encodingToIntArray = function(encoding) {
	blackberrynfc.ndef.util.log.log(">> encodingToIntArray()",1);
	try {
		if (typeof encoding == "string") {
			encoding = encoding.replace(/\s/g,"");
			if (""==encoding) {
				return new Array();
			}
			return blackberrynfc.ndef.util.base64.base64ToIntArray(encoding);
		} else if (typeof encoding == "object" && encoding.length != undefined && typeof encoding.length == "number") {
			for (var i=0; i<encoding.length; ++i) {
				if (typeof encoding[i] != "number") {
					blackberrynfc.ndef.util.log.log("Array does not exclusivley contain numbers");
					return new Array();
				}
			}
			return encoding;
		} else {
			blackberrynfc.ndef.util.log.log("Don't know how to process " + encoding);
			return new Array();
		}
	}
	finally {
		blackberrynfc.ndef.util.log.log("<< encodingToIntArray()",-1);
	}
};