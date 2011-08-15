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

package webworks.message.list.impl;


import java.util.Vector;

import net.rim.blackberry.api.messagelist.ApplicationMessageFolder;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolderRegistry;
import net.rim.device.api.collection.ReadableList;
import net.rim.device.api.system.PersistentObject;
import net.rim.device.api.system.PersistentStore;
import net.rim.device.api.util.Persistable;
import net.rim.device.api.util.ToIntHashtable;

public final class CustomMessageStore
{

    private static CustomMessageStore _instance;
    private ReadableListImpl _inboxMessages;

    private static long _GUID;        

    /**
     * Creates a new CustomMessageStore object
     */
    private CustomMessageStore(long GUID)
    {
        this(GUID, new ReadableListImpl());
    }
    
    private CustomMessageStore(long GUID, ReadableListImpl inboxMessages)
    {
        _inboxMessages = inboxMessages;
        _GUID = GUID;
        inboxMessages.init();
    }


    /**
     * Gets the singleton instance of the CustomMessageStore
     * 
     * @return The singleton instance of the MessagelistDemoStore
     */
    public static synchronized CustomMessageStore getInstance(long GUID)
    {

        if(_instance == null)
        {
        	PersistentObject store = PersistentStore.getPersistentObject(GUID);
        	if(store.getContents() == null){
        		_instance = new CustomMessageStore(GUID);
        		synchronized(store){        			
        			store.setContents(_instance.getInboxMessages());
        			store.commit();
        		}
        	}else{
        		_instance = new CustomMessageStore(GUID, (ReadableListImpl)store.getContents());
        	}
        }
        return _instance;
    }

    /**
     * Retrieves the inbox folder
     * 
     * @return The inbox folder
     */
    ApplicationMessageFolder getInboxFolder()
    {
        return ApplicationMessageFolderRegistry.getInstance().getApplicationFolder(CustomMessage.INBOX_FOLDER_ID);
    }

    /**
     * Moves a message into the deleted folder
     * 
     * @param message The message to move to the deleted folder
     */
    void deleteInboxMessage(CustomMessage message)
    {
        message.messageDeleted();
        _inboxMessages.removeMessage(message);
        persist();

    }


    /**
     * Commits a message to the persistent store
     * 
     * @param message The message to commit
     */
    void commitMessage(CustomMessage message)
    {
    	persist();
    }
    
    private void persist(){
    	PersistentObject store = PersistentStore.getPersistentObject(_GUID);    		
		synchronized(store){
			store.setContents(this.getInboxMessages());
			store.commit();
		}
    }


    /**
     * Adds a message to the inbox
     * 
     * @param message The message to add to the inbox
     */
    void addInboxMessage(CustomMessage message)
    {
        _inboxMessages.addMessage(message);
        persist();
    }

    /**
     * Retrieves the inbox messages as a readable list
     * 
     * @return The readable list of all the inbox messages
     */
    ReadableListImpl getInboxMessages()
    {
        return _inboxMessages;
    }
	
    /**
     * This is an implementation of the ReadableList interface which stores the
     * list of messages using a Vector.
     */
    static class ReadableListImpl implements ReadableList, Persistable
    {
        private Vector _messages;
        private ToIntHashtable _typeToImageJSON;
        private static int _typeCounter = 1;

        /**
         * Creates a empty instance of ReadableListImpl
         */
        ReadableListImpl()
        {
            _messages = new Vector();
            _typeToImageJSON = new ToIntHashtable();
        }
        
        public void init(){

        }


        /**
         * @see net.rim.device.api.collection.ReadableList#getAt(int)
         */
        public Object getAt(int index)
        {
            return _messages.elementAt(index);
        }


        /**
         * @see net.rim.device.api.collection.ReadableList#getAt(int, int, Object, int)
         */
        public int getAt(int index, int count, Object[] elements, int destIndex)
        {
        	int returnVal = 0;
        	if(_messages.size() < index+count){
        		throw new ArrayIndexOutOfBoundsException();
        	}else{
        		for(;returnVal<count;returnVal++){
        			elements[destIndex+returnVal] = _messages.elementAt(index+returnVal);
        		}
        	}
            return returnVal;
        }


        /**
         * @see net.rim.device.api.collection.ReadableList#getIndex(Object)
         */
        public int getIndex(Object element)
        {
            return _messages.indexOf(element);
        }


        /**
         * @see net.rim.device.api.collection.ReadableList#size()
         */
        public int size()
        {
            return _messages.size();
        }


        /**
         * Add a message to this list
         * 
         * @param message The message to add to this list
         */
        void addMessage(CustomMessage message)        
        {        	
            registerIcons(message);
        	_messages.addElement(message);
        }
        

        /**
         * Removes a message from this list
         * 
         * @param message The message to remove from this list
         */
        void removeMessage(CustomMessage message)
        {
            _messages.removeElement(message);
        }
        
        ToIntHashtable getCustomTypeTable(){
        	return _typeToImageJSON;
        }
        
        private void registerIcons(CustomMessage message){
        	int type = CustomMessage.DEFAULT_IMAGE_TYPE;
        	String newImage = message.getImageNew()==null?"":message.getImageNew();
        	String readImage = message.getImageRead()==null?"":message.getImageRead();
        	if(newImage.length()!=0 || readImage.length()!=0){        		
        		String JSON = "{'imageNew':" + newImage + ",'imageRead':" + readImage +"}";
        		type = _typeToImageJSON.get(JSON);
        		if(type == -1){
        			type = CustomMessage.DEFAULT_IMAGE_TYPE+_typeCounter++;
        			_typeToImageJSON.put(JSON, type);
        			
        			CustomMessageDispatcher.registerType(type, newImage, readImage);
        		}        		
        	}
        	message.setType(type);        	
        }
    }
}
