/*
* Copyright 2010-2012 Research In Motion Limited.
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

package webworks.persistent.storage;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.PersistentObject;
import net.rim.device.api.system.PersistentStore;
import net.rim.device.api.util.StringUtilities;

public class StorageNamespace extends Scriptable
{
    public static final String FIELD_WRITE = "write";
    public static final String FIELD_READ = "read";
    public static final String FIELD_DELETE = "delete";
    public Object getField(String name) throws Exception
    {
        if (name.equals(FIELD_WRITE))
        {
            return new StorageWrite();
        }
        if (name.equals(FIELD_READ))
        {
            return new StorageRead();
        }
        
        if (name.equals(FIELD_DELETE))
        {
            return new StorageDelete();
        }
        return super.getField(name);
    }

    public class StorageDelete extends ScriptableFunction
    {
        private static final String NAME = "delete";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args.length == 1)
            {
                PersistentObject myPersistentData;
                PersistentStore.destroyPersistentObject(StringUtilities.stringHashToLong(args[0].toString()));
            }
            return UNDEFINED;
        }
    }
    
    public class StorageWrite extends ScriptableFunction
    {
        private static final String NAME = "write";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            if (args.length == 2)
            {
                PersistentObject myPersistentData;
                myPersistentData = PersistentStore.getPersistentObject(StringUtilities.stringHashToLong(args[0].toString()));
                synchronized(myPersistentData)
                {
                    myPersistentData.setContents(args[1]);
                    myPersistentData.commit();
                }
            }
            return UNDEFINED;
        }
    }
    
    public class StorageRead extends ScriptableFunction
    {
        private static final String NAME = "read";
        public Object invoke(Object obj, Object[] args) throws Exception
        {
            String answer = "";
            if (args.length == 1)
            {
                
                PersistentObject myPersistentData;
                myPersistentData = PersistentStore.getPersistentObject(StringUtilities.stringHashToLong(args[0].toString()));
                synchronized(myPersistentData)
                {
                    String data = (String) myPersistentData.getContents();
                    if (data != null)
                    {
                        answer = data.toString();
                    }
                }
            }
            return answer.toString();
        }
    }
}