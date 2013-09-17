/*
 *  Copyright 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

var fs = require('fs'),
    path = require('path'),
    wrench = require('wrench'),
    localize = require("./localize"),
    os = require('os'),
    DEFAULT_BAR_NAME = "project",
    PROPERTY_FILE_NAME = 'blackberry10.json',
    CORDOVA_DIR = '.cordova',
    DEFAULT_PROPERTY_FILE = {
        targets: {
        }
    },
    _self;

function swapBytes(buffer) {
    var l = buffer.length,
        i,
        a;

    if (l % 2 === 0x01) {
        throw localize.translate("EXCEPTION_BUFFER_ERROR");
    }

    for (i = 0; i < l; i += 2) {
        a = buffer[i];
        buffer[i] = buffer[i + 1];
        buffer[i + 1] = a;
    }

    return buffer;
}

_self = {
    writeFile: function (fileLocation, fileName, fileData) {
        //If directory does not exist, create it.
        if (!fs.existsSync(fileLocation)) {
            wrench.mkdirSyncRecursive(fileLocation, "0755");
        }

        fs.writeFile(path.join(fileLocation, fileName), fileData, function (err) {
            if (err) throw err;
        });
    },

    copyFile: function (srcFile, destDir, baseDir) {
        var filename = path.basename(srcFile),
            fileBuffer = fs.readFileSync(srcFile),
            fileLocation;

        //if a base directory was provided, determine
        //folder structure from the relative path of the base folder
        if (baseDir && srcFile.indexOf(baseDir) === 0) {
            fileLocation = srcFile.replace(baseDir, destDir);
            wrench.mkdirSyncRecursive(path.dirname(fileLocation), "0755");
            fs.writeFileSync(fileLocation, fileBuffer);
        } else {
            if (!fs.existsSync(destDir)) {
                wrench.mkdirSyncRecursive(destDir, "0755");
            }

            fs.writeFileSync(path.join(destDir, filename), fileBuffer);
        }
    },

    listFiles: function (directory, filter) {
        var files = wrench.readdirSyncRecursive(directory),
            filteredFiles = [];

        files.forEach(function (file) {
            //On mac wrench.readdirSyncRecursive does not return absolute paths, so resolve one.
            file = path.resolve(directory, file);

            if (filter(file)) {
                filteredFiles.push(file);
            }
        });

        return filteredFiles;
    },

    isWindows: function () {
        return os.type().toLowerCase().indexOf("windows") >= 0;
    },

    isArray: function (obj) {
        return obj.constructor.toString().indexOf("Array") !== -1;
    },

    isEmpty : function (obj) {
        for (var prop in obj) {
            if (obj.hasOwnProperty(prop))
                return false;
        }
        return true;
    },

    toBoolean: function (myString, defaultVal) {
        // if defaultVal is not passed, default value is undefined
        return myString === "true" ? true : myString === "false" ? false : defaultVal;
    },

    parseUri : function (str) {
        var i, uri = {},
            key = [ "source", "scheme", "authority", "userInfo", "user", "password", "host", "port", "relative", "path", "directory", "file", "query", "anchor" ],
            matcher = /^(?:([^:\/?#]+):)?(?:\/\/((?:(([^:@]*)(?::([^:@]*))?)?@)?([^:\/?#]*)(?::(\d*))?))?((((?:[^?#\/]*\/)*)([^?#]*))(?:\?([^#]*))?(?:#(.*))?)/.exec(str);

        for (i = key.length - 1; i >= 0; i--) {
            uri[key[i]] = matcher[i] || "";
        }

        return uri;
    },

    // uri - output from parseUri
    isAbsoluteURI : function (uri) {
        if (uri && uri.source) {
            return uri.relative !== uri.source;
        }

        return false;
    },

    isLocalURI : function (uri) {
        return uri && uri.scheme && uri.scheme.toLowerCase() === "local";
    },

    // Convert node.js Buffer data (encoded) to String
    bufferToString : function (data) {
        var s = "";
        if (Buffer.isBuffer(data)) {
            if (data.length >= 2 && data[0] === 0xFF && data[1] === 0xFE) {
                s = data.toString("ucs2", 2);
            } else if (data.length >= 2 && data[0] === 0xFE && data[1] === 0xFF) {
                swapBytes(data);
                s = data.toString("ucs2", 2);
            } else if (data.length >= 3 && data[0] === 0xEF && data[1] === 0xBB && data[2] === 0xBF) {
                s = data.toString("utf8", 3);
            } else {
                s = data.toString("ascii");
            }
        }

        return s;
    },

    // Wrap object property in an Array if the property is defined and it is not an Array
    wrapPropertyInArray : function (obj, property) {
        if (obj && obj[property] && !(obj[property] instanceof Array)) {
            obj[property] = [ obj[property] ];
        }
    },

    loadModule: function (path) {
        return require(path);
    },

    findHomePath : function () {
        return process.env.HOME || process.env.HOMEPATH || process.env.USERPROFILE;
    },

    getCordovaDir: function () {
        var cordovaPath = path.join(_self.findHomePath(), CORDOVA_DIR);

        if (!fs.existsSync(cordovaPath)) {
            fs.mkdirSync(cordovaPath);
        }

        return cordovaPath;
    },

    getPropertiesFilePath: function () {
        var propertiesFile = path.join(_self.getCordovaDir(), PROPERTY_FILE_NAME);

        if (!fs.existsSync(propertiesFile)) {
            _self.writeToPropertiesFile(DEFAULT_PROPERTY_FILE);
        }

        return propertiesFile;
    },

    getPropertiesFileName: function () {
        return PROPERTY_FILE_NAME;
    },

    getProperties: function () {
        return require(_self.getPropertiesFilePath());
    },

    writeToPropertiesFile: function (data) {
        var contents = JSON.stringify(data, null, 4) + "\n",
            propertiesFile = path.join(_self.getCordovaDir(), PROPERTY_FILE_NAME);

        fs.writeFileSync(propertiesFile, contents, 'utf-8');
    },

    genBarName: function() {
        return "DEFAULT_BAR_NAME";
    }

};

module.exports = _self;
