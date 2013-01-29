/*
* Copyright 2013 Research In Motion Limited.
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

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

var vibration,
    _event = require("../../lib/event");

JNEXT.Vibration = function () {
    var self = this,
        hasInstance = false;

    self.getId = function () {
        return self.m_id;
    };

    self.init = function () {
        if (!JNEXT.require("libVibration")) {
            return false;
        }

        self.m_id = JNEXT.createObject("libVibration.VibrationJS");

        if (self.m_id === "") {
            return false;
        }

        JNEXT.registerEvents(self);
    };

    // ************************
    // Enter your methods here
    // ************************

    // calls into InvokeMethod(string command) in vibration_js.cpp
    
    self.vibration_request = function (input) {
        return JNEXT.invoke(self.m_id, "vibration_request " + JSON.stringify(input));
    };

    // Fired by the Event framework (used by asynchronous callbacks)
    self.onEvent = function (strData) {
        var arData = strData.split(" "),
            strEventDesc = arData[0],
            jsonData;
        // Event names are set in native code when fired,
        // and must be checked here.
        if (strEventDesc === "vibration_requestCallbackResult") {
            // Slice off the event name and the rest of the data is our JSON
            jsonData = arData.slice(1, arData.length).join(" ");
            _event.trigger("vibration_requestCallbackResult", JSON.parse(jsonData));
        }
    };

    // ************************
    // End of methods to edit
    // ************************
    self.m_id = "";

    self.getInstance = function () {
        if (!hasInstance) {
            hasInstance = true;
            self.init();
        }
        return self;
    };

};

vibration = new JNEXT.Vibration();

module.exports = {
    vibration: vibration
};
