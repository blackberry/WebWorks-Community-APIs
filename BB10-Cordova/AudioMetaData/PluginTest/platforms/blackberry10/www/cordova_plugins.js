cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/org.apache.cordova.file/www/DirectoryEntry.js",
        "id": "org.apache.cordova.file.DirectoryEntry",
        "clobbers": [
            "window.DirectoryEntry"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/DirectoryReader.js",
        "id": "org.apache.cordova.file.DirectoryReader",
        "clobbers": [
            "window.DirectoryReader"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/Entry.js",
        "id": "org.apache.cordova.file.Entry",
        "clobbers": [
            "window.Entry"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/File.js",
        "id": "org.apache.cordova.file.File",
        "clobbers": [
            "window.File"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/FileEntry.js",
        "id": "org.apache.cordova.file.FileEntry",
        "clobbers": [
            "window.FileEntry"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/FileError.js",
        "id": "org.apache.cordova.file.FileError",
        "clobbers": [
            "window.FileError"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/FileReader.js",
        "id": "org.apache.cordova.file.FileReader",
        "clobbers": [
            "window.FileReader"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/FileSystem.js",
        "id": "org.apache.cordova.file.FileSystem",
        "clobbers": [
            "window.FileSystem"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/FileUploadOptions.js",
        "id": "org.apache.cordova.file.FileUploadOptions",
        "clobbers": [
            "window.FileUploadOptions"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/FileUploadResult.js",
        "id": "org.apache.cordova.file.FileUploadResult",
        "clobbers": [
            "window.FileUploadResult"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/FileWriter.js",
        "id": "org.apache.cordova.file.FileWriter",
        "clobbers": [
            "window.FileWriter"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/Flags.js",
        "id": "org.apache.cordova.file.Flags",
        "clobbers": [
            "window.Flags"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/LocalFileSystem.js",
        "id": "org.apache.cordova.file.LocalFileSystem",
        "clobbers": [
            "window.LocalFileSystem"
        ],
        "merges": [
            "window"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/Metadata.js",
        "id": "org.apache.cordova.file.Metadata",
        "clobbers": [
            "window.Metadata"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/ProgressEvent.js",
        "id": "org.apache.cordova.file.ProgressEvent",
        "clobbers": [
            "window.ProgressEvent"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/requestFileSystem.js",
        "id": "org.apache.cordova.file.requestFileSystem",
        "clobbers": [
            "window.requestFileSystem"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/resolveLocalFileSystemURI.js",
        "id": "org.apache.cordova.file.resolveLocalFileSystemURI",
        "merges": [
            "window"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/fileUtils.js",
        "id": "org.apache.cordova.file.BB10Utils"
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/DirectoryEntry.js",
        "id": "org.apache.cordova.file.BB10DirectoryEntry",
        "clobbers": [
            "window.DirectoryEntry"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/DirectoryReader.js",
        "id": "org.apache.cordova.file.BB10DirectoryReader",
        "clobbers": [
            "window.DirectoryReader"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/Entry.js",
        "id": "org.apache.cordova.file.BB10Entry",
        "clobbers": [
            "window.Entry"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/File.js",
        "id": "org.apache.cordova.file.BB10File",
        "clobbers": [
            "window.File"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/FileEntry.js",
        "id": "org.apache.cordova.file.BB10FileEntry",
        "clobbers": [
            "window.FileEntry"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/FileReader.js",
        "id": "org.apache.cordova.file.BB10FileReader",
        "clobbers": [
            "window.FileReader"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/FileSystem.js",
        "id": "org.apache.cordova.file.BB10FileSystem",
        "clobbers": [
            "window.FileSystem"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/FileWriter.js",
        "id": "org.apache.cordova.file.BB10FileWriter",
        "clobbers": [
            "window.FileWriter"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/requestFileSystem.js",
        "id": "org.apache.cordova.file.BB10requestFileSystem",
        "clobbers": [
            "window.requestFileSystem"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/resolveLocalFileSystemURI.js",
        "id": "org.apache.cordova.file.BB10resolveLocalFileSystemURI",
        "clobbers": [
            "window.resolveLocalFileSystemURI"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.file/www/blackberry10/resolveLocalFileSystemURL.js",
        "id": "org.apache.cordova.file.BB10resolveLocalFileSystemURL",
        "clobbers": [
            "window.resolveLocalFileSystemURL"
        ]
    },
    {
        "file": "plugins/com.blackberry.invoke.card/www/client.js",
        "id": "com.blackberry.invoke.card.client",
        "merges": [
            "blackberry.invoke.card"
        ]
    },
    {
        "file": "plugins/com.blackberry.invoke.card/www/btoa.js",
        "id": "com.blackberry.invoke.card.btoa",
        "merges": [
            "blackberry.invoke.card.btoa"
        ]
    },
    {
        "file": "plugins/com.blackberry.community.audiometadata/www/client.js",
        "id": "com.blackberry.community.audiometadata.client",
        "clobbers": [
            "com.blackberry.community.audiometadata"
        ]
    },
    {
        "file": "plugins/com.blackberry.io/www/client.js",
        "id": "com.blackberry.io.client",
        "merges": [
            "blackberry.io"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "org.apache.cordova.file": "1.1.0",
    "com.blackberry.invoke.card": "1.0.0",
    "com.blackberry.community.audiometadata": "1.0.0",
    "com.blackberry.io": "1.0.0"
}
// BOTTOM OF METADATA
});