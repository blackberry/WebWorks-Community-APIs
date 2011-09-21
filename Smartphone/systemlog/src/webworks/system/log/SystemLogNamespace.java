/*
* Copyright 2011 Research In Motion Limited.
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

package webworks.system.log;

import net.rim.device.api.system.EventLogger;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.util.StringUtilities;

public class SystemLogNamespace extends Scriptable
{
    private static final String FIELD_WRITE = "write";
    public Object getField(String name) throws Exception
    {
        if (name.equals(FIELD_WRITE))
        {
            return new WriteSystemLog();
        }
        return super.getField(name);
    }

    public class WriteSystemLog extends ScriptableFunction
    {
        private static final String NAME = "write";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 3)
            {
                writeToLog(args[0].toString(), args[1].toString(), args[2].toString());
            }
            else
            {
                writeToLog("webworks.system.log", "WebWorks System Log API", "ERROR 4419 Improper API useage");
            }
            return UNDEFINED;
        }
    }

    private static void writeToLog(String guid, String appName, String appMessage)
    {
        final long myGuid = StringUtilities.stringHashToLong(guid.toString());
        final String myAppName = appName.toString();
        byte[] logMessage = appMessage.getBytes();
        boolean registerEventLogger = EventLogger.register(myGuid, myAppName, EventLogger.VIEWER_STRING);
        if (registerEventLogger == true)
        {
            EventLogger.logEvent(myGuid, logMessage);
        }
    }
}