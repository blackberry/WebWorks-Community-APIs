require('../../lib/webview');

module.exports = {
	prompt: function(success, fail, args, env) {
		var field_type = JSON.parse(decodeURIComponent(args[2]))

		if(field_type == "password") {
			// inject DOMNodeInserted listener into overlay webview
			var to_overlay_webview = '\
			document.addEventListener("DOMNodeInserted", function(e) {\
				if(e.target.className == "dialog-content-container" &&\
				   e.target.getElementsByTagName("input").length > 0) {\
					console.log("WRITE ONCE");\
					e.target.getElementsByTagName("input")[0].type = "password";\
					this.removeEventListener("DOMNodeInserted", arguments.callee);\
				}\
			});';

			var overlay_webview = qnx.webplatform.getWebViews()[2];
			overlay_webview.executeJavaScript(to_overlay_webview);
		}

		var result = new PluginResult(args, env);
		result.ok(true,false);
	}
};
