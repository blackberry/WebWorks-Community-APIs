var barcodereader = require("./barcodereaderJNEXT").barcodereader,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// Thread methods to start and stop
	startRead: function (success, fail) {
		barcodereader.getInstance().startRead();
		success();
	},
	stopRead: function (success, fail) {
		barcodereader.getInstance().stopRead();
		success();
	}
};