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

var fs = require("fs"),
    path = require("path"),
    utils = require("./utils"),
    targetUtils = require("./target-utils.js"),
    localize = require("./localize"),
    pkgrUtils = require("./packager-utils"),
    debugTokenHelper = require("./debugtoken-helper"),
    xml2js = require('xml2js'),
    logger = require("./logger"),
    async = require("async"),
    session = require("./session"),
    properties = utils.getProperties(),
    workingdir = path.normalize(__dirname + "/..");

//Options looking for are: (Device | Emulator, query, devicepass). Calls back with:  (error || options object, target object)
function getTargetName(options, done) {
    var ipFinder = options.device ? targetUtils.findConnectedDevice : targetUtils.findConnectedSimulator,
        targetType = options.device ? "device" : "emulator";

    if (options.target) {
        done(null, options, options.target);
    } else if (options.device || options.emulator) {
        if (options.device && options.emulator) {
            localize.translate("WARN_RUN_DEVICE_OVERRIDES_EMULATOR");
        }

        async.series(
        {
            ip: function (done) {
                ipFinder(function (ip) {
                   done(ip ? null : "No connected BlackBerry 10 " + targetType + " found", ip);
                });
            },
            devicePass: function (done) {
                if (!options.devicepass && options.devicepass !== "") {
                    if (options.query) {
                        var description = options.device ? "Please enter your " + targetType +  " password: " : "Please enter your " + targetType +  " password (For no password press Enter): ";
                        utils.prompt({description: description, hidden: true}, done);
                    } else if (!options.emulator) {
                        done("Please provide device password using --devicepass");
                    } else {
                        done("");
                    }
                } else {
                    done(null, options.devicepass);
                }
            }
        },
        function (err, results) {
            if (err) {
                done(err);
            } else {
                options.devicepass = results.devicePass;
                _self.checkDeviceInfo(options, results.ip, targetType, results.devicePass, done);
            }
        });
    } else {
        done(null, options, properties.defaultTarget);
    }
}

//Options looking for are: (query, devicepass). Calls back with:  (error || target object)
function validateTarget(options, targetName, allDone) {
    var deployTarget,
        err,
        runTasks = [];

    if (!targetName) {
        err = "No target exists, to add that target please run: target add <name> <ip> [-t | --type <device | simulator>] [-p <password>] [--pin <devicepin>]";
    } else if (!properties.targets[targetName]) {
        err = "The target \"" + targetName + "\" does not exist, to add that target please run: target add " + targetName + " <ip> [-t | --type <device | simulator>] [-p <password>] [--pin <devicepin>]";
    } else {
        deployTarget = utils.clone(properties.targets[targetName]);
        deployTarget.name = targetName;

        if (!deployTarget.ip) {
            if (options.query) {
                runTasks.push(function (done) {
                    utils.prompt({description: "Please enter the IP address for target " + deployTarget.name + ": "}, function (e, ip) {
                        deployTarget.ip = ip;
                        done(e);
                    });
                });
            } else {
                err = "IP is not defined in target \"" + target + "\"";
            }
        }

        if (!deployTarget.password && deployTarget.password !== "") {
            if (options.devicepass || options.devicepass === "") {
                deployTarget.password = options.devicepass;
            } else {
                if (options.query) {
                    runTasks.push(function (done) {
                        var description = options.device ? "Please enter your " + deployTarget.type +  " password: " : "Please enter your " + deployTarget.type +  " password (For no password press Enter): ";
                        utils.prompt({description: description, hidden: true}, function (e, devicePass) {
                            deployTarget.password = devicePass;
                            done(e);
                        });
                    });
                } else if (!options.emulator) {
                    err = "Please provide device password using --devicepass or add one to the target " + deployTarget.name + " defined at " + utils.getPropertiesFilePath();
                }
            }
        }

        if (!deployTarget.pin) {
            runTasks.push(function (done) {
                targetUtils.getDeviceInfo.bind(deployTarget.ip, deployTarget.devicePass, function (err, result) {
                    deployTarget.pin = result.pin;
                    done(err);
                });
            });
        }
    }

    async.series(runTasks, function (e) {
        var finalErr = err || e;
        if (!finalErr && deployTarget) {
            logger.info("Target " + deployTarget.name + " selected");
        }
        allDone(err || e, options, deployTarget);
    });
}
//Options looking for are: (keystorepass, query). Calls back with:  (error || target object)
function handleDebugToken(options, deployTarget, allDone) {
    options.keystorepass = session.getKeyStorePass(options);

    // if target has no pin, skip the debug token feature
    if (deployTarget.pin && !options.emulator) {
        async.waterfall(
            [
                debugTokenHelper.checkDebugToken.bind(this, deployTarget.pin),
                function (done) {
                    //If no keystorepass is provided debugTokenHelper will throw an error.
                    if (!options.keystorepass && options.query) {
                        utils.prompt({description: "Please enter your keystore password: ", hidden: true}, function (err, result) {
                            options.keystorepass = result;
                            done(err, result);
                        });
                    } else {
                        done(null, options.keystorepass);
                    }
                },
                debugTokenHelper.createToken.bind(this, properties, "all")
            ],
            function (err, results) {
                // If the error is true, then the debug token is valid and creation was skipped.
                if (err === true) {
                    logger.info(localize.translate("PROGRESS_DEBUG_TOKEN_IS_VALID"));
                    //Clear the error so it is still deployed
                    err = null;
                }

                if (!err) {
                    debugTokenHelper.deployToken(deployTarget.name, deployTarget.ip, deployTarget.password, function (code) {
                        allDone(code, deployTarget);
                    });
                } else {
                    allDone(err);
                }
            }
        );
    } else {
        allDone(null, deployTarget);
    }
}

function generateDeployOptions(options, deployTarget, uninstall) {
    var deployOptions = [],
        barPath = pkgrUtils.escapeStringForShell(
            path.normalize(__dirname + "/../../build/" +
                (deployTarget.type === "device" ? "device" : "simulator") +
                "/" + utils.genBarName() + ".bar"));

    deployOptions.push("-device");
    deployOptions.push(deployTarget.ip);

    if (deployTarget.password) {
        deployOptions.push("-password");
        deployOptions.push(deployTarget.password);
    }

    deployOptions.push("-package");
    deployOptions.push(barPath);

    if (uninstall) {
        deployOptions.push("-uninstallApp");
        return deployOptions;
    } else {

        deployOptions.push("-installApp");

        if (options.launch) {
            deployOptions.push("-launchApp");
        }

        return deployOptions;
    }
}

function execNativeDeploy(deployOptions, callback) {
    var script = path.normalize(path.join(process.env.CORDOVA_BBTOOLS, "blackberry-deploy"));

    utils.exec(script, deployOptions, {
        "cwd": workingdir,
        "env": process.env
    }, callback);
}

_self = {
    //options looking for are: (query, devicepass, password, target, (device || emulator)) Function returns (error || deployTarget)
    getValidatedTarget : function (options, callback) {
        async.waterfall(
            [
                getTargetName.bind(this, options),
                validateTarget,
                handleDebugToken
            ], callback
        );
    },
    //options looking for are: (launch) Function returns (error || null)
    deployToTarget : function (options, deployTarget, callback) {
        execNativeDeploy(generateDeployOptions(options, deployTarget, false));
    },

    //options looking for are: (uninstall) Function returns (error || null)
    uninstall : function (options, deployTarget, allDone) {
        var script = path.join(process.env.CORDOVA_BBTOOLS, "blackberry-deploy"),
            args = [
                "-listInstalledApps",
                "-device",
                deployTarget.ip
            ],
            projectRootDir = path.normalize(path.join(__dirname, "..")),
            installedAppsOutput,
            runTasks = [];

        if (options.uninstall) {
            if (deployTarget.password) {
                args.push("-password", deployTarget.password);
            }
            runTasks = [
                utils.exec.bind(this, script, args, { "cwd": projectRootDir, _customOptions: {silent: true}}),
                function listInstalledAppsOutput (stdout, stderr, done) {
                    installedAppsOutput = stdout;
                    fs.readFile(path.join(__dirname + "/../../www/", "config.xml"), done);
                },
                function configXMLOutput (result, done) {
                    var parser = new xml2js.Parser();
                    parser.parseString(result, done);
                },
                function parsedConfigXMLOutput (result, done) {
                    if (installedAppsOutput.indexOf(result['@'].id) !== -1) {
                        var deployOptions = generateDeployOptions(options, deployTarget, true);
                        execNativeDeploy(deployOptions, done);
                    } else {
                        done();
                    }
                }
            ];
        }

        async.waterfall(runTasks,
            function (err, results) {
                //Absorb error for uninstallation
                allDone(null, deployTarget);
            }
        );
    },

    //Function returns (error || deployTarget)
    checkBuild : function (deployTarget, allDone) {
        barPath = path.normalize(__dirname + "/../../build/" +
                (deployTarget.type === "device" ? "device" : "simulator") +
                "/" + utils.genBarName() + ".bar");
        if (fs.existsSync(barPath)) {
            allDone(null, deployTarget);
        } else {
            allDone(err = "No build file exists, please run: build [--debug] [--release] [-k | --keystorepass] [-b | --buildId <number>] [-p | --params <json>] [-ll | --loglevel <level>] ");
        }

    },

    //No options needed within function Function returns (error || options, targetName)
    checkDeviceInfo : function (options, ip, deviceType, devicePass, done) {
        var props = utils.getProperties(),
            targetName;

        targetUtils.getDeviceInfo(ip, devicePass, function (err, device) {
            if (!err) {
                targetName = device.name + "-" + device.pin;
                props.targets[targetName] = {
                    ip: ip,
                    pin: device.pin,
                    type: deviceType
                };
                utils.writeToPropertiesFile(props);
            }
            done(err, options, targetName);
        });
    }
};

module.exports = _self;
