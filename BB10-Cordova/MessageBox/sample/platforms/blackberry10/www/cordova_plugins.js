cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/org.apache.cordova.core.dialogs/www/notification.js",
        "id": "org.apache.cordova.core.dialogs.notification",
        "merges": [
            "navigator.notification"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.MessageBox/www/client.js",
        "id": "org.apache.cordova.MessageBox.client",
        "clobbers": [
            "org.apache.cordova.MessageBox"
        ]
    }
]
});