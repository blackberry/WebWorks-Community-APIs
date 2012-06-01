/*
* Copyright 2010-2011 Research In Motion Limited.
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

package webworks.event.globalEvent;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Application;
import net.rim.device.api.system.ApplicationManager;
import net.rim.device.api.util.StringUtilities;

public class GlobalEventNamespace extends Scriptable
{
    public static final String FIELD_LISTEN = "listen";
    public static final String FIELD_FIRE = "fire";
    public Object getField(String name) throws Exception
    {
        if (name.equals(FIELD_LISTEN))
        {
            return new GlobalListen();
        }
        if (name.equals(FIELD_FIRE))
        {
            return new GlobalFireEvent();
        }
        return super.getField(name);
    }

    public class GlobalListen extends ScriptableFunction
    {
        private static final String NAME = "listen";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args.length == 2)
            {
            	//the event guid we are listening for will be the first param
                final String myKey = args[0].toString();
                
                //the second param will be the callback
                final ScriptableFunction callback = (ScriptableFunction)args[1];
                
                Application.getApplication().addGlobalEventListener(new MyGlobalEventListener(myKey, callback));
            }
            return UNDEFINED;
        }
    }
    
    public class GlobalFireEvent extends ScriptableFunction
    {
        private static final String NAME = "fire";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            boolean result = false;
            if (args.length == 1)
            {
            	long myLong = StringUtilities.stringHashToLong(args[0].toString());
        		result = ApplicationManager.getApplicationManager().postGlobalEvent(myLong);
                
            }
            //return if event was successfully posted.
            return new Boolean(result);
        }
    }
}