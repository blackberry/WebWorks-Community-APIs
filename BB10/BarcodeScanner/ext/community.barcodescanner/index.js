var barcodescanner = require("./barcodescannerJNEXT").barcodescanner,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// Thread methods to start and stop
	startRead: function (success, fail) {
		barcodescanner.getInstance().startRead();
		success();
	},
	stopRead: function (success, fail) {
		barcodescanner.getInstance().stopRead();
		success();
	}
};