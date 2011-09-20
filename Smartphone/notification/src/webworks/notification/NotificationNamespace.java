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

package webworks.notification;

import net.rim.blackberry.api.messagelist.ApplicationIndicator;
import net.rim.blackberry.api.messagelist.ApplicationIndicatorRegistry;
import net.rim.device.api.notification.NotificationsConstants;
import net.rim.device.api.notification.NotificationsManager;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.util.StringUtilities;

public class NotificationNamespace  extends Scriptable
{
    public static final String FIELD_REGISTER = "register";
    public static final String FIELD_FIRE = "fire";
    public static final String FIELD_CANCEL = "cancel";
    public static final String FIELD_UNREGISTER = "unregister";

    public Object getField(String name) throws Exception
    {
        if (name.equals(FIELD_REGISTER))
        {
            return new RegisterNotificationObjects();
        }
        else if (name.equals(FIELD_FIRE))
        {
            return new FireEvent();
        }
        else if (name.equals(FIELD_CANCEL))
        {
            return new CancelEvent();
        }
        else if (name.equals(FIELD_UNREGISTER))
        {
            return new UnregisterNotificationObjects();
        }
        return super.getField(name);
    }

    private class RegisterNotificationObjects  extends ScriptableFunction
    {
        private static final String NAME = "register";
        private Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 2)
            {
                final long sourceID = StringUtilities.stringHashToLong(args[0].toString());
                final String notificationName =  args[1].toString();
                NotificationsManager.registerSource( 
                    sourceID, new Object()
                    {
                        public String toString()
                        {
                            return notificationName;
                        }
                    }
                    , NotificationsConstants.IMPORTANT );
            }
            return UNDEFINED;
        }
    }

    private class FireEvent  extends ScriptableFunction
    {
        private static final String NAME = "fire";
        private Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 1)
            {
                final long sourceID = StringUtilities.stringHashToLong(args[0].toString());
                NotificationsManager.triggerImmediateEvent( sourceID, 0, this, null );
            }
            return UNDEFINED;
        }
    }

    private class CancelEvent  extends ScriptableFunction
    {
        private static final String NAME = "cancel";
        private Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 1)
            {
                final long sourceID = StringUtilities.stringHashToLong(args[0].toString());
                NotificationsManager.cancelImmediateEvent( sourceID, 0, this, null );
            }
            return UNDEFINED;
        }
    }

    private class UnregisterNotificationObjects  extends ScriptableFunction
    {
        private static final String NAME = "unregister";
        private Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args != null && args.length == 1)
            {
                final long sourceID = StringUtilities.stringHashToLong(args[0].toString());
                NotificationsManager.deregisterSource(sourceID);
            }
            return UNDEFINED;
        }
    }
}