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

package webworks.message.list;


import java.util.Vector;

import net.rim.blackberry.api.messagelist.*;
import net.rim.device.api.collection.ReadableList;
import net.rim.device.api.system.RuntimeStore;
import net.rim.blackberry.api.messagelist.ApplicationIndicatorRegistry;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolder;

/**
 * This class is used to facilitate the storage of messages. For the sake of
 * simplicitly, we are saving messages in the device runtime store. In a real
 * world situation, messages would be saved in device persistent store and/or
 * on a mail server.
 */
public final class CustomMessageStore
{
	//TODO: change: unique msg name converted to long
    private static final long MSG_KEY = 0x8083229363c3b900L;

    private static CustomMessageStore _instance;
    private ReadableListImpl _inboxMessages;
    private ApplicationMessageFolder _mainFolder;


    /**
     * Creates a new CustomMessageStore object
     */
    private CustomMessageStore()
    {
        _inboxMessages = new ReadableListImpl();
    }


    /**
     * Gets the singleton instance of the CustomMessageStore
     * 
     * @return The singleton instance of the MessagelistDemoStore
     */
    public static synchronized CustomMessageStore getInstance()
    {
        // Keep messages as singleton in the RuntimeStore
        if(_instance == null)
        {
            RuntimeStore rs = RuntimeStore.getRuntimeStore();

            synchronized(rs)
            {
                _instance = (CustomMessageStore) rs.get(MSG_KEY);

                if(_instance == null)
                {
                    _instance = new CustomMessageStore();
                    rs.put(MSG_KEY, _instance);
                }
            }
        }
        return _instance;
    }


    /**
     * Sets the main and folder
     * 
     * @param mainFolder The main folder to use
     * @param deletedFolder The deleted folder to use
     */
    void setFolder(ApplicationMessageFolder mainFolder)
    {
        _mainFolder = mainFolder;
    }


    /**
     * Retrieves the inbox folder
     * 
     * @return The inbox folder
     */
    ApplicationMessageFolder getInboxFolder()
    {
        return _mainFolder;
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
    }


    /**
     * Commits a message to the persistent store
     * 
     * @param message The message to commit
     */
    void commitMessage(CustomMessage message)
    {
        // This empty method exists to reinforce the idea that in a real world
        // situation messages would be saved in device persistent store and/or
        // on a mail server.
    }


    /**
     * Adds a message to the inbox
     * 
     * @param message The message to add to the inbox
     */
    void addInboxMessage(CustomMessage message)
    {
        _inboxMessages.addMessage(message);
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
    static class ReadableListImpl implements ReadableList
    {
        private Vector messages;

        /**
         * Creates a empty instance of ReadableListImpl
         */
        ReadableListImpl()
        {
            messages = new Vector();
        }


        /**
         * @see net.rim.device.api.collection.ReadableList#getAt(int)
         */
        public Object getAt(int index)
        {
            return messages.elementAt(index);
        }


        /**
         * @see net.rim.device.api.collection.ReadableList#getAt(int, int, Object, int)
         */
        public int getAt(int index, int count, Object[] elements, int destIndex)
        {
            return 0;
        }


        /**
         * @see net.rim.device.api.collection.ReadableList#getIndex(Object)
         */
        public int getIndex(Object element)
        {
            return messages.indexOf(element);
        }


        /**
         * @see net.rim.device.api.collection.ReadableList#size()
         */
        public int size()
        {
            return messages.size();
        }


        /**
         * Add a message to this list
         * 
         * @param message The message to add to this list
         */
        void addMessage(CustomMessage message)
        {
            messages.addElement(message);
        }
        

        /**
         * Removes a message from this list
         * 
         * @param message The message to remove from this list
         */
        void removeMessage(CustomMessage message)
        {
            messages.removeElement(message);
        }
    }
}
