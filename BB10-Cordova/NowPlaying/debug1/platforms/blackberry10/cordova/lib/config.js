/*
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
    PROPERTY_FILE_NAME = 'blackberry10.json',
    CORDOVA_DIR = '.cordova',
    DEFAULT_PROPERTY_FILE = {
        targets: {
        }
    },
    getCordovaDir = function () {
        var homePath = process.env[(process.platform === 'win32') ? 'USERPROFILE' : 'HOME'],
            cordovaPath = path.join(homePath, CORDOVA_DIR);
        if (!fs.existsSync(cordovaPath)) {
            fs.mkdirSync(cordovaPath);
        }
        return cordovaPath;
    };

module.exports = {
    getProperties: function () {
        var props, 
            propertiesPath = path.join(getCordovaDir(), PROPERTY_FILE_NAME);
        if (!fs.existsSync(propertiesPath)) {
            this.writeProperties(DEFAULT_PROPERTY_FILE);
        }
        props = require(propertiesPath);
        if (!props.targets) {
            props.targets = {};
        }
        return props;
    },
    writeProperties: function (data) {
        var contents = JSON.stringify(data, null, 4) + "\n",
            propertiesPath = path.join(getCordovaDir(), PROPERTY_FILE_NAME);
        fs.writeFileSync(propertiesPath, contents, 'utf-8');
    }
};
