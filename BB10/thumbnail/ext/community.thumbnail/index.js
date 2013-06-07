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
var thumbNailJNext,
    _event = require("../../lib/event");

module.exports = {
    createThumbNail: function (success, fail, args, env) {
        try {
			var quality;
			if(args.quality != null){
				quality = args.quality;
			}
			else{
				quality = 100;
			}
			if(args.width != null && args.height != null){
            	success(thumbNailJNext.createThumbNailJNext(JSON.parse(decodeURIComponent(args.path)), args.width, args.height, quality));
			}
			else if(args.width != null){
				success(thumbNailJNext.createThumbNailWidthJNext(JSON.parse(decodeURIComponent(args.path)), args.width, quality));
			}
			else{
				success(thumbNailJNext.createThumbNailHeightJNext(JSON.parse(decodeURIComponent(args.path)), args.height, quality));
			}
        } catch (e) {
            fail(-1, e.message);
        }
    },
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin
///////////////////////////////////////////////////////////////////

JNEXT.ThumbNailJNext = function ()
{   
    var _self = this;

    _self.createThumbNailJNext = function (path, width, height, quality) {
         return JNEXT.invoke(_self._id, "createThumbNailNative " + path + " " + width + " " + height + " " + quality);
    };
    _self.createThumbNailWidthJNext = function (path, width, quality) {
         return JNEXT.invoke(_self._id, "createThumbNailWidthNative " + path + " " + width + " " + quality);
    };
    _self.createThumbNailHeightJNext = function (path, height, quality) {
         return JNEXT.invoke(_self._id, "createThumbNailHeightNative " + path + " " + height+ " " + quality);
    };

    _self.getId = function () {
        return _self._id;
    };

    _self.init = function () {
        if (!JNEXT.require("libThumbnail")) {
            return false;
        }

        _self._id = JNEXT.createObject("libThumbnail.ThumbNail");

        if (!_self._id || _self._id === "") {
            return false;
        }

        JNEXT.registerEvents(_self);
    };

    
    _self._id = "";
    
    _self.init();
};

thumbNailJNext = new JNEXT.ThumbNailJNext();
