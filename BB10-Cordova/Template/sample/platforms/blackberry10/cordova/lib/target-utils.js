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

var _self,
    exec = require('child_process').exec,
    bb10_utils = require('./utils'),
    blackberryProperties = bb10_utils.getProperties();

_self = {
    getTargetList : function (type, pruneDisconnected, callback) {
        var targList = [],
            count = 0,
            targets = blackberryProperties.targets,
            addItem = function (t) {
                targets[t].name = t;
                targList.push(targets[t]);
            },
            checkConnection = function (name) {
                _self.checkConnection(targets[name].ip, type, function (connected) {
                    count++;
                    if (connected) {
                        addItem(name);
                    }
                    complete();
                });
            },
            complete = function () {
                if (count === Object.keys(targets).length) {
                    callback(targList);
                }
            };
        if (targets) {
            for (t in targets) {
                if (targets.hasOwnProperty(t) && targets[t].type === type) {
                    if (pruneDisconnected) {
                        checkConnection(t);
                    }
                    else {
                        addItem(t);
                        count++;
                    }
                } else {
                    count++;
                }
            } 
        }
        complete();
    },

    checkConnection: function(ip, type, callback) {
        exec('blackberry-deploy -test ' + ip, function(error, stdout, stderr) {
            // error code 3 corresponds to a connected device, null corresponds to connected sim
            callback((type === 'simulator' && error === null) || (type == 'device' && error.code === 3));
        });
    },

    listTargets : function(type, pruneDisconnected) {
        _self.getTargetList(type, pruneDisconnected, function (targets) {
            for (t in targets) {
                console.log(targets[t].name + ' ip: ' + targets[t].ip + (pruneDisconnected ? ' status: connected' : ''));
            }
        });
    }

};

module.exports = _self;
