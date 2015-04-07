/*
 *  Copyright 2014 BlackBerry Limited.
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

var path = require("path"),
    fs = require("fs"),
    utils = require("./utils"),
    localize = require("./localize");

function getParams(toolName, cmdline) {
    var properties = utils.getProperties(),
        params = properties[toolName],
        paramsPath,
        cmdParams;

    if (cmdline && cmdline.params) {
        if (!cmdParams) {
            paramsPath = path.resolve(cmdline.params);

            if (fs.existsSync(paramsPath)) {
                try {
                    cmdParams = require(paramsPath);
                } catch (e) {
                    throw localize.translate("EXCEPTION_PARAMS_FILE_ERROR", paramsPath);
                }
            } else {
                throw localize.translate("EXCEPTION_PARAMS_FILE_NOT_FOUND", paramsPath);
            }
        }
    }

    if (cmdParams && cmdParams[toolName]) {
        if (params) {
            params = utils.mixin(cmdParams[toolName], params);
        } else {
            params = cmdParams[toolName];
        }
    }

    return params;
}


module.exports = getParams;
