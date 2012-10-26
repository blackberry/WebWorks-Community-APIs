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
var compassJNext,
    _event = require("../../lib/event");

module.exports = 
{
    startMonitoringCompassServer: function (success, fail, args, env) 
    {
        try 
        {
            success(compassJNext.startMonitoringCompassJNext());
        } 
        catch (e) 
        {
            fail(-1, e);
        }
    },
    
    stopMonitoringCompassServer: function (success, fail, args, env)
    {
    	try 
        {
            success(compassJNext.stopMonitoringCompassJNext());
        } 
        catch (e) 
        {
            fail(-1, e);
        }
    },
    
    readCompassServer: function (success, fail, args, env)
    {
    	 try 
         {
             success(compassJNext.readCompassJNext());
         } 
         catch (e) 
         {
             fail(-1, e);
         }
    },
};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin
///////////////////////////////////////////////////////////////////

JNEXT.CompassJNext = function ()
{   
    var _self = this;

    _self.startMonitoringCompassJNext = function () 
    {
        return JNEXT.invoke(_self._id, "startMonitoringNative");
    };
    
    _self.stopMonitoringCompassJNext = function () 
    {
        return JNEXT.invoke(_self._id, "stopMonitoringNative");
    };
    
    _self.readCompassJNext = function () 
    {
        return JNEXT.invoke(_self._id, "readCompassNative");
    };

    _self.getId = function () 
    {
        return _self._id;
    };

    _self.init = function () 
    {
        if (!JNEXT.require("compassJnext")) 
        {
            return false;
        }

        _self._id = JNEXT.createObject("compassJnext.Compass");

        if (!_self._id || _self._id === "") 
        {
            return false;
        }

        JNEXT.registerEvents(_self);
    };

    _self.onEvent = function (strData) 
    {
        var arData = strData.split(" "),
            strEventId = arData[0],
            arg = arData[1];

        // Trigger the event handler of specific Push events
        if (strEventId === "CompassLeading") 
        {
            _event.trigger("community.compass.compassEvent", arg);
        }
    };
    
    _self._id = "";
    
    _self.init();
};

compassJNext = new JNEXT.CompassJNext();
