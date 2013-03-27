var qrcodereader = require("./qrcodereaderJNEXT").qrcodereader,
	_event = require("../../lib/event"),
    _utils = require("../../lib/utils");

module.exports = {

	// Thread methods to start and stop
	QRCodeReaderStart: function (success, fail) {
		var views = qnx.webplatform.getWebViews();
		var handle = null;
		for (var i = 0; i < views.length; i++) {
			if (views[i].constructor.name == "UIWebView"){
				handle = views[i].jsScreenWindowHandle;
			}
		}
		qrcodereader.getInstance().QRCodeReaderStart( handle );
		success();
	},
	QRCodeReaderStop: function (success, fail) {
		qrcodereader.getInstance().QRCodeReaderStop();
		success();
	}
};