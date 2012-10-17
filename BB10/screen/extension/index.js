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

var qtest;

module.exports = {
    setWallpaper: function (success, fail, args, env) {
        debugger;
        try {
            var args = { file: JSON.parse(decodeURIComponent(args["file"])) };
            success(qtest.setWallpaper(args));
        } catch (e) {
            fail(-1, e);
        }
    }
};


///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT QTest plugin
///////////////////////////////////////////////////////////////////

JNEXT.QTest = function()
{
    var self = this;

    self.setWallpaper = function (args) {
        return JNEXT.invoke(self.m_id, "setWallpaper " + JSON.stringify(args));
    };

    self.getId = function()
    {
        return self.m_id;
    }

    self.init = function()
    {
        // The line below loads the plugin named qtest.dll or qtest.so
        // from the jnext directory if it has not already been loaded
        if ( !JNEXT.require( "libqtest4" ) )
        {
            alert("qtest.so not loaded");
            return false;
        }

        // The line below creates a new QTest object from the QTest plugin
        self.m_id = JNEXT.createObject( "libqtest4.QTest" );
        if ( self.m_id == "" )
        {
            alert( "error initializing qtest" );
            return false;
        }

        // This registers this QTest object to receive any asyncronous events
        // (unused in the QTest sample - see the sockets sample for more details)
        JNEXT.registerEvents( self );
    }

    self.m_id = "";
    self.init();
}
qtest = new JNEXT.QTest();
