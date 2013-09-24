require('lib/webview');

(function(webviews){
	var to_overlay_webview = '\
	document.addEventListener("DOMNodeInserted", function(e) {\
		if(e.target.className == "dialog-content-container") {\
			e.target.getElementsByTagName("input")[0].type = "password";\
		}\
	})';
	var webviews_interval = setInterval(webviews_test, 200);

	function webviews_test() {
		if(webviews.length == 3) {
			clearInterval(webviews_interval);
			addJSToOverlayWebview();
		}
	}
	function addJSToOverlayWebview() {
		var overlay_webview = webviews[2];
		console.log(overlay_webview);
		overlay_webview.executeJavaScript(to_overlay_webview);
	}
})(qnx.webplatform.webViews)
