/*
 * Copyright 2012 Research In Motion Limited.
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
var ledJNext,
    _event = require("../../lib/event");

module.exports = {
    flashLedStart: function (success, fail, args, env) {
        try {
		    var stringArgs = "";
			if(isNaN(parseInt(JSON.parse(decodeURIComponent(args["color"])), 16))){
				fail(-1, "invalid Hexdecimal color code");
				return;
			}
			
		    for (key in args) {
	           stringArgs += " " + JSON.parse(decodeURIComponent(args[key]));
	        }
            success(ledJNext.flashLedStartJNext(stringArgs));
        } catch (e) {
            fail(-1, e);
        }
    },
    flashLedStop: function (success, fail, args, env) {
        try {
		    var stringArgs = "";
		    for (key in args) {
	           stringArgs += " " + JSON.parse(decodeURIComponent(args[key]));
	        }
            success(ledJNext.flashLedStopJNext(stringArgs));
        } catch (e) {
            fail(-1, e);
        }
    }
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin
///////////////////////////////////////////////////////////////////

JNEXT.LedJNext = function ()
{   
    var _self = this;

    _self.flashLedStartJNext = function (args) {
        return JNEXT.invoke(_self._id, "flashLedStartNative" + args);
    };

    _self.flashLedStopJNext = function (args) {
        return JNEXT.invoke(_self._id, "flashLedStopNative" + args);
    };

    _self.getId = function () {
        return _self._id;
    };

    _self.init = function () {
        if (!JNEXT.require("libLED")) {
            return false;
        }

        _self._id = JNEXT.createObject("libLED.FlashLed");

        if (!_self._id || _self._id === "") {
            return false;
        }

        JNEXT.registerEvents(_self);
    };

    _self.onEvent = function (strData) {
        var arData = strData.split(" "),
            strEventId = arData[0],
            arg = arData[1];
    };
    
    _self._id = "";
    
    _self.init();
};

ledJNext = new JNEXT.LedJNext();
