var qrcodereader = require("./qrcodereaderJNEXT").qrcodereader,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// Thread methods to start and stop
	QRCodeReaderStart: function (success, fail) {
		qrcodereader.getInstance().QRCodeReaderStart();
		success();
	},
	QRCodeReaderStop: function (success, fail) {
		qrcodereader.getInstance().QRCodeReaderStop();
		success();
	}
};