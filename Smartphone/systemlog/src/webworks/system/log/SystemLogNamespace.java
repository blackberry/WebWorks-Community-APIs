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

public class NotificationNamespace  extends Scriptable
{
    public static final String FIELD_WRITE = "write";
    public Object getField(String name) throws Exception
    {
        if (name.equals(FIELD_WRITE))
        {
            return new WriteSystemLog();
        }
        return super.getField(name);
    }

    private class WriteSystemLog extends ScriptableFunction
    {
        private static final String NAME = "write";
        private Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 3)
            {
                final long myGuid = StringUtilities.stringHashToLong(args[0].toString());
                final String myAppName = args[1].toString();
                String messageToLog = args[2].toString();
            }
            else
            {
                final long myGuid = StringUtilities.stringHashToLong("webworks.system.log");
                final String myAppName = "WebWorks System Log API";
                String messageToLog = "ERROR 4419-2 Impropper API useage";
            }

            byte[] message = messageToLog.getBytes();
            boolean registerErrorEventLogger = EventLogger.register(myGuid, myAppName, EventLogger.VIEWER_STRING);
            if (registerErrorEventLogger == true)
            {
                EventLogger.logEvent(errorGuid, errorMessage);
            }
            else
            {
                final long errorGuid = StringUtilities.stringHashToLong("webworks.system.log");
                final String errMesage = "ERROR 4419-1 Failed to register Application Name";
                final String errorLogName = "WebWorks System Log API";

                byte[] errorMessage = errMessage.getBytes();
                boolean registerErrorEventLogger = EventLogger.register(errorGuid, errorLogName, EventLogger.VIEWER_STRING);
                if (registerErrorEventLogger == true)
                {
                    EventLogger.logEvent(errorGuid, errorMessage);
                }
            }
            return UNDEFINED;
        }
    }
}