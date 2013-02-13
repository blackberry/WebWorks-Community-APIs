var qrcodereader = require("./qrcodereaderJNEXT").qrcodereader,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// Thread methods to start and stop
	startRead: function (success, fail) {
		qrcodereader.getInstance().QRCodeReaderStart();
		success();
	},
	stopRead: function (success, fail) {
		qrcodereader.getInstance().QRCodeReaderStop();
		success();
	}
};