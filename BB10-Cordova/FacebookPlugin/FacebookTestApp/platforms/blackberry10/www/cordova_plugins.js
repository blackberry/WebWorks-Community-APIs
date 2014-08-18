cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/com.blackberry.community.facebookplugin/www/client.js",
        "id": "com.blackberry.community.facebookplugin.client",
        "clobbers": [
            "community.facebookplugin"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "org.apache.cordova.inappbrowser": "0.3.0",
    "com.blackberry.community.facebookplugin": "1.0.0"
}
// BOTTOM OF METADATA
});