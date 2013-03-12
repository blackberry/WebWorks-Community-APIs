
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

var barcodereader,
    _event = require("../../lib/event");

JNEXT.BarcodeReader = function () {
    var self = this,
        hasInstance = false;

    self.getId = function () {
        return self.m_id;
    };

    self.init = function () {
        if (!JNEXT.require("libBarcodeReader")) {
            return false;
        }

        self.m_id = JNEXT.createObject("libBarcodeReader.BarcodeReaderJS");

        if (self.m_id === "") {
            return false;
        }

        JNEXT.registerEvents(self);
    };

    // ************************
    // Enter your methods here
    // ************************

  
    // Fired by the Event framework (used by asynchronous callbacks)
    self.onEvent = function (strData) {
        var arData = strData.split(" "),
            strEventDesc = arData[0],
            jsonData;
        // Event names are set in native code when fired,
        // and must be checked here.
        i
        if (strEventDesc === "community.barcodereader.codefound.native") {
             // Slice off the event name and the rest of the data is our JSON
            jsonData = arData.slice(1, arData.length).join(" ");
            _event.trigger("community.barcodereader.codefound", JSON.parse(jsonData));
        }
        else if ( strEventDesc === "community.barcodereader.errorfound.native") {
             jsonData = arData.slice(1, arData.length).join(" ");
            _event.trigger("community.barcodereader.errorfound", JSON.parse(jsonData));
        }
        else if ( strEventDesc === "community.barcodereader.frameavailable.native") {
             jsonData = arData.slice(1, arData.length).join(" ");
            _event.trigger("community.barcodereader.frameavailable", JSON.parse(jsonData));
        }
         else if ( strEventDesc === "community.barcodereader.started.native"){
              jsonData = arData.slice(1, arData.length).join(" ");
            _event.trigger("community.barcodereader.started", JSON.parse(jsonData));
        } 
        else if ( strEventDesc === "community.barcodereader.ended.native") {
             jsonData = arData.slice(1, arData.length).join(" ");
            _event.trigger("community.barcodereader.ended", JSON.parse(jsonData));
        }
    };

    // Thread methods
    self.startRead = function () {
        return JNEXT.invoke(self.m_id, "startRead");
    };
    self.stopRead = function () {
        return JNEXT.invoke(self.m_id, "stopRead");
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

barcodereader = new JNEXT.BarcodeReader();

module.exports = {
    barcodereader: barcodereader
};
