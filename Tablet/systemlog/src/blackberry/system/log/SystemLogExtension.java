/*
* Copyright 2010-2011 Research In Motion Limited.
*
* Additionally
* Copyright 2011 Matthew Haag.
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

import org.w3c.dom.Document;
import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.util.SimpleSortingVector;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetException;
import net.rim.device.api.util.StringUtilities;
import net.rim.device.api.system.EventLogger;
import net.rim.device.api.web.WidgetExtension;
import blackberry.common.util.JSUtilities;
import blackberry.common.util.json4j.JSONObject;
import blackberry.core.IJSExtension;
import blackberry.core.JSExtensionRequest;
import blackberry.core.JSExtensionResponse;
import blackberry.core.JSExtensionReturnValue;


public final class SystemLogExtension implements IJSExtension
{
    private String      FEATURE_SYSTEM_DISPLAY  = "blackberry.ext.system.log";
    private static String[] JS_FILES = { "systemlog_dispatcher.js", "systemlog_ns.js" };
        private String WRITE = "write";
        private String SETUP = "setup";
        private static int NONE = 0;
        private static int EVENT_LOG  = 1;
        private static int STANDARD_OUT  = 2;
        private static int BOTH  =  3;
    
        public String[] getFeatureList()
        {
            String[] result = new String[] { FEATURE_SYSTEM_DISPLAY };
                        return result;
        }

        /**
     * Implements invoke() of interface IJSExtension. Methods of extension will be called here.
     * @throws WidgetException if specified method cannot be recognized
     */
    public void invoke( JSExtensionRequest request, JSExtensionResponse response ) throws WidgetException {
        String method = request.getMethodName();
        Object[] args = request.getArgs();
        String msg = "";
        int code = JSExtensionReturnValue.SUCCESS;
        JSONObject data = new JSONObject();
        JSONObject returnValue = null;
        
        try {
            if( method.equals( SETUP ) ) {
                if (args != null && args.length == 3)
                {
                    setupLogging(args[0].toString(), args[1].toString(), Integer.parseInt(args[2].toString()));
                }
                else
                {
                    writeToLog("webworks.system.log", "WebWorks System Log API", "ERROR 4419 Improper API useage");
                }
            
                                
            } else if( method.equals( WRITE ) ) {
                              
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
                        }
        } catch( Exception e ) {
            msg = e.getMessage();
            code = JSExtensionReturnValue.FAIL;
        }

        returnValue = new JSExtensionReturnValue( msg, code, data ).getReturnValue();

        response.setPostData( returnValue.toString().getBytes() );
    }
        
    public void loadFeature( String feature, String version, Document document, ScriptEngine scriptEngine,
            SimpleSortingVector jsInjectionPaths ) {
        JSUtilities.loadJS( scriptEngine, JS_FILES, jsInjectionPaths );
                
    }

    public void register(WidgetConfig config, BrowserField bf)
    {
        // TODO Auto-generated method stub
    }

    public void unloadFeatures()
    {
        // TODO Auto-generated method stub
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
        setupLogging(guid, appName, EVENT_LOG);
        writeToLog(appMessage);
    }
    private static void setupLogging(String guid, String appName, int loggingMode) {
        myGuid = StringUtilities.stringHashToLong(guid.toString());
        myAppName = appName.toString();
        registerEventLogger = EventLogger.register(myGuid, myAppName, EventLogger.VIEWER_STRING);
        mode = loggingMode;
        
    }
}
