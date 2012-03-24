/*
* Copyright 2011 Research In Motion Limited.
* Copyright 2011-2012 Matthew Haag Verivo Software.
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

package blackberry.system.log;

import net.rim.device.api.system.EventLogger;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.util.StringUtilities;

public class SystemLogNamespace extends Scriptable
{
    private static String FIELD_WRITE = "write";
    private static String FIELD_SETUP = "setup";
    private static String FIELD_NONE = "none";
    private static String FIELD_EVENT_LOG = "event_log";
    private static String FIELD_STANDARD_OUT = "standard_out";
    private static String FIELD_BOTH = "both";
    private static int NONE = 0;
    private static int EVENT_LOG  = 1;
    private static int STANDARD_OUT  = 2;
    private static int BOTH  =  3;
    public Object getField(String name) throws Exception
    {
        if (name.equals(FIELD_WRITE))
        {
            return new WriteSystemLog();
        } else if (name.equals(FIELD_SETUP)){
                return new SetupLogging();
        }
        else if (name.equals(FIELD_NONE)){
                return new Integer(NONE);
        }
        else if (name.equals(FIELD_EVENT_LOG)){
                return new Integer(EVENT_LOG);
        }
        else if (name.equals(FIELD_STANDARD_OUT)){
                return new Integer(STANDARD_OUT);
        }
        else if (name.equals(FIELD_BOTH)){
                return new Integer(BOTH);
        }
        return super.getField(name);
    }

    public class WriteSystemLog extends ScriptableFunction
    {
        private static final String NAME = "write";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 1)
            {
                writeToLog(args[0].toString());
            } else if (args != null && args.length == 3)
            {
                writeToLog(args[0].toString(), args[1].toString(), args[2].toString());
            }
            else
            {
                writeToLog("blackberry.system.log", "WebWorks System Log API", "ERROR 4419 Improper API useage");
            }
            return UNDEFINED;
        }
    }
    
    public class SetupLogging extends ScriptableFunction
    {
        private static final String NAME = "setup";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 3)
            {
                setupLogging(args[0].toString(), args[1].toString(), ((Integer)args[2]));
            }
            else
            {
                writeToLog("webworks.system.log", "WebWorks System Log API", "ERROR 4419 Improper API useage");
            }
            return UNDEFINED;
        }
    }
    static long myGuid = 0l;
    static String myAppName;
    static int mode = NONE;
    static boolean registerEventLogger;
    private static void writeToLog(String appMessage)
    {
        
        
        byte[] logMessage = appMessage.getBytes();
        
        if (registerEventLogger == true && (EVENT_LOG & mode) == EVENT_LOG)
        {
            EventLogger.logEvent(myGuid, logMessage);
        }
        
        if((STANDARD_OUT & mode) == STANDARD_OUT)
                System.out.println(appMessage);
    }
    private static void writeToLog(String guid,String appName,String appMessage)
    {
        setupLogging(guid, appName, new Integer(EVENT_LOG));
        writeToLog(appMessage);
    }
    private static void setupLogging(String guid, String appName, Integer loggingMode) {
        myGuid = StringUtilities.stringHashToLong(guid.toString());
        myAppName = appName.toString();
        registerEventLogger = EventLogger.register(myGuid, myAppName, EventLogger.VIEWER_STRING);
        mode = loggingMode.intValue();
        
        }
}
