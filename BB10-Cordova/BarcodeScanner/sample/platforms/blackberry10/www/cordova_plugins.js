cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/com.blackberry.io/www/client.js",
        "id": "com.blackberry.io.client",
        "clobbers": [
            "blackberry.io"
        ]
    },
    {
        "file": "plugins/community.BarcodeScanner/www/client.js",
        "id": "community.BarcodeScanner.client",
        "clobbers": [
            "community.BarcodeScanner"
        ]
    }
]
});