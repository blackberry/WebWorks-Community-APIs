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

var pgaudio;

module.exports = {
    preloadAudio: function (success, fail, args, env) {
        var result = new PluginResult(args, env),
            data = JSON.parse(decodeURIComponent(args.data)),
            path = JSON.parse(decodeURIComponent(args.path)),
            voices = JSON.parse(decodeURIComponent(args.voices)),
            response = pgaudio.getInstance().preloadAudio(path, data, voices);
        result.ok(response, false);
    },
    unload: function (success, fail, args, env) {
        var result = new PluginResult(args, env),
            data = JSON.parse(decodeURIComponent(args.data)),
            response = pgaudio.getInstance().unload(data);
        result.ok(response, false);
    },
    play: function (success, fail, args, env) {
        var result = new PluginResult(args, env),
            data = JSON.parse(decodeURIComponent(args.data)),
            response = pgaudio.getInstance().play(data);
        result.ok(response, false);
    },
    stop: function (success, fail, args, env) {
        var result = new PluginResult(args, env),
            data = JSON.parse(decodeURIComponent(args.data)),
            response = pgaudio.getInstance().stop(data);
        result.ok(response, false);
    },
    loop: function (success, fail, args, env) {
        var result = new PluginResult(args, env),
            data = JSON.parse(decodeURIComponent(args.data)),
            response = pgaudio.getInstance().loop(data);
        result.ok(response, false);
    }

};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin
///////////////////////////////////////////////////////////////////

JNEXT.PGaudio = function () {
    var self = this,
        hasInstance = false;

    self.preloadAudio = function (path, file, voices) {
        //This is how Javascript calls into native
        return JNEXT.invoke(self.m_id, "load " + path + " " + file + " " + voices);
    };
    self.unload = function (file) {
        //This is how Javascript calls into native
        return JNEXT.invoke(self.m_id, "unload " + file);
    };
    self.play = function (file) {
        //This is how Javascript calls into native
        return JNEXT.invoke(self.m_id, "play " + file);
    };
    self.stop = function (file) {
        //This is how Javascript calls into native
        return JNEXT.invoke(self.m_id, "stop " + file);
    };
    self.loop = function (file) {
        //This is how Javascript calls into native
        return JNEXT.invoke(self.m_id, "loop " + file);
    };


    self.init = function () {
        //Checks that the jnext library is present and loads it
        if (!JNEXT.require("libpgaudio")) {
            return false;
        }

        //Creates the native object that this interface will call
        self.m_id = JNEXT.createObject("libpgaudio.PGaudio");

        if (self.m_id === "") {
            return false;
        }

        //Registers for the JNEXT event loop
        JNEXT.registerEvents(self);
    };

    self.m_id = "";

    //Used by JNEXT library to get the ID
    self.getId = function () {
        return self.m_id;
    };

    //Not truly required but useful for instance management
    self.getInstance = function () {
        if (!hasInstance) {
            self.init();
            hasInstance = true;
        }
        return self;
    };
};

pgaudio = new JNEXT.PGaudio();
