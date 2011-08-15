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

import java.util.Enumeration;

import net.rim.blackberry.api.menuitem.ApplicationMenuItem;
import net.rim.blackberry.api.messagelist.ApplicationIcon;
import net.rim.blackberry.api.messagelist.ApplicationMessage;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolder;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolderListener;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolderRegistry;
import net.rim.device.api.system.ApplicationDescriptor;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.util.ToIntHashtable;
import webworks.message.list.MessageListNamespace;
import webworks.message.list.impl.CustomMessageStore.ReadableListImpl;

/**
 * Daemon process that runs in the background. It's tasks include non-gui
 * message operations such as message deletion, marking messages as read/unread
 */
public final class CustomMessageDispatcher implements ApplicationMessageFolderListener
{
    private static String MENU_NAME = "Open";
    private static String MENU_NAME_MARK_READ = "Mark Read";
	private static String MENU_NAME_MARK_UNREAD = "Mark Unread";

	private static String _folderName;
	
	private static long _GUID;
	
	private static ApplicationMenuItem[] _newGuiMenuItems;
	private static ApplicationMenuItem[] _newDaemonMenuItems;
	private static ApplicationMenuItem[] _openedGuiMenuItems;
	private static ApplicationMenuItem[] _markedUnreadMenuItems;

    /**
     * Called during device startup. Registers application descriptors, message
     * folder listeners, message icons and menu items.
     */
    void init(String folderName, String appName, long GUID)
    {
		_folderName = folderName;
		_GUID = GUID;
		
		// 1. Register folders and application descriptors ----------------------
		ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
	  
		// Some context menu items don't need a GUI (e.g. an item for deleting a
		// message) and will be run in the current daemon application.
		ApplicationDescriptor appDescr = new ApplicationDescriptor(ApplicationDescriptor.currentApplicationDescriptor(),new String[]{});
		
		CustomMessageStore messages = CustomMessageStore.getInstance(GUID);
		ReadableListImpl inboxMessages = messages.getInboxMessages();
		
		ApplicationMessageFolder inbox = reg.registerFolder(CustomMessage.INBOX_FOLDER_ID, _folderName, inboxMessages);

		// Register as a listener for callback notifications
		inbox.addListener(this, ApplicationMessageFolderListener.MESSAGE_DELETED | ApplicationMessageFolderListener.MESSAGE_MARKED_OPENED
			| ApplicationMessageFolderListener.MESSAGE_MARKED_UNOPENED, appDescr);			

		// We've registered two folders, specify root folder name for the [View Folder] screen.
		reg.setRootFolderName(_folderName);			

		// Set up menu items --------------------------------------
		ApplicationMenuItem openMenuItem = new OpenContextMenu(0x230010);
		ApplicationMenuItem markOpenedMenuItem = new MarkOpenedContextMenu(0x230030);
		ApplicationMenuItem markUnreadMenuItem = new MarkUnreadContextMenu(0x230031);
		
		_newGuiMenuItems = new ApplicationMenuItem[] {openMenuItem};
		_newDaemonMenuItems = new ApplicationMenuItem[] {markOpenedMenuItem};
		_openedGuiMenuItems = new ApplicationMenuItem[] {openMenuItem};
		_markedUnreadMenuItems = new ApplicationMenuItem[] {markUnreadMenuItem};
		
		//Register Default type				
		registerType(CustomMessage.DEFAULT_IMAGE_TYPE, "", "");
		
		//Register persisted types
		ToIntHashtable customTypeTable = inboxMessages.getCustomTypeTable();
		if(!customTypeTable.isEmpty()){
			for(Enumeration e =customTypeTable.keys();e.hasMoreElements();){
				String JSON = (String) e.nextElement();
				int commaIndex = JSON.indexOf(',');
				String newImage = JSON.substring(12, commaIndex);
				String readImage = JSON.substring(commaIndex+13,JSON.length()-1);
				int type = customTypeTable.get(JSON);
				registerType(type, newImage, readImage);
			}
		}
    }
    
    public static void registerType(int type, String newImage, String readImage){
    	ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
    	ApplicationDescriptor appDescr = new ApplicationDescriptor(ApplicationDescriptor.currentApplicationDescriptor(),new String[]{});
    	
    	
		// 2. Register message icons -------------------------------------------
    	newImage = newImage.length()==0?MessageListNamespace.getInstance().getDefaultNewImage():newImage;
    	readImage = readImage.length()==0?MessageListNamespace.getInstance().getDefaultReadImage():readImage;
		ApplicationIcon newIcon = new ApplicationIcon(EncodedImage.getEncodedImageResource(newImage), true);
		ApplicationIcon readIcon = new ApplicationIcon(EncodedImage.getEncodedImageResource(readImage), true);
		reg.registerMessageIcon(type, CustomMessage.STATUS_NEW, newIcon);
		reg.registerMessageIcon(type, CustomMessage.STATUS_OPENED, readIcon);

		//3. Register message menu items --------------------------------------
		reg.registerMessageMenuItems(type, CustomMessage.STATUS_NEW, _newGuiMenuItems, appDescr);
		reg.registerMessageMenuItems(type, CustomMessage.STATUS_NEW, _newDaemonMenuItems, appDescr);
		reg.registerMessageMenuItems(type, CustomMessage.STATUS_OPENED, _openedGuiMenuItems, appDescr);
		reg.registerMessageMenuItems(type, CustomMessage.STATUS_OPENED, _markedUnreadMenuItems, appDescr);
		reg.setBulkMarkOperationsSupport(type, CustomMessage.STATUS_NEW, true, false);
		reg.setBulkMarkOperationsSupport(type, CustomMessage.STATUS_OPENED, false, true);
    }
	

    public static void markAllAsRead()
    {
    	try {
			CustomMessageStore messageStore = CustomMessageStore.getInstance(_GUID);
			int length = messageStore.getInboxMessages().size();
			for (int i = 0; i < length; i++) {
				CustomMessage msg = (CustomMessage) messageStore.getInboxMessages().getAt(i);
				msg.markRead();
			}
			ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
			ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessage.INBOX_FOLDER_ID);
			folder.fireReset();
    	} catch (Exception e) 
    	{
    		Dialog.alert("problem marking all messages as read " + e.getMessage());
    	}
    }

    /**
     * Mark Opened menu item. After the method marks the message read, it fires
     * an update event.
     */
    private static class MarkOpenedContextMenu extends ApplicationMenuItem
    {		
	
        /**
         * Creates a new ApplicationMenuItem instance with provided menu position
         * 
         * @param order Display order of this item, lower numbers correspond to  higher placement in the menu
         */
        MarkOpenedContextMenu(int order)
        {
            super(order);
        }


        /**
         * Marks the context message opened
         * 
         * @see ApplicationMenuItem#run(Object)
         */
        public Object run(Object context)
        {
            if(context instanceof CustomMessage)
            {
                CustomMessage message = (CustomMessage) context;
                message.markRead();
                ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
                ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessage.INBOX_FOLDER_ID);
                folder.fireElementUpdated(message, message);
				
				// Invoke our callback
                MessageListNamespace.getInstance().invokeItemMarkedRead(message);
            }
            
            return context;
        }


        /**
         * @see java.lang.Object#toString()
         */
        public String toString()
        {
            return MENU_NAME_MARK_READ;
        }
    }
	
	/**
     * Mark Opened menu item. After the method marks the message read, it fires
     * an update event.
     */
    private static class MarkUnreadContextMenu extends ApplicationMenuItem
    {
	
        /**
         * Creates a new ApplicationMenuItem instance with provided menu position
         * 
         * @param order Display order of this item, lower numbers correspond to  higher placement in the menu
         */
        MarkUnreadContextMenu(int order)
        {
            super(order);
        }


        /**
         * Marks the context message opened
         * 
         * @see ApplicationMenuItem#run(Object)
         */
        public Object run(Object context)
        {
            if(context instanceof CustomMessage)
            {
                CustomMessage message = (CustomMessage) context;
                message.markUnRead();
                ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
                ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessage.INBOX_FOLDER_ID);
                folder.fireElementUpdated(message, message);
				
				// Invoke our callback
                MessageListNamespace.getInstance().invokeItemMarkedUnread(message);
            }
            
            return context;
        }


        /**
         * @see java.lang.Object#toString()
         */
        public String toString()
        {
            return MENU_NAME_MARK_UNREAD;
        }
    }
    
	

    /**
     * Open Context menu item. Marks read and opens the selected message for
     * viewing.
     */
    static class OpenContextMenu extends ApplicationMenuItem
    {
		
        /**
         * Creates a new ApplicationMenuItem instance with provided menu position
         * 
         * @param order Display order of this item, lower numbers correspond to higher placement in the menu
         */
        public OpenContextMenu(int order)
        {
            super(order);
        }

        /**
         * @see ApplicationMenuItem#run(Object)
         */
        public Object run(Object context)
        {
            if(context instanceof CustomMessage)
            {
                CustomMessage message = (CustomMessage) context;

                // Update status if message is new
                if(message.isNew())
                {
                    message.markRead();
                    ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
                    ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessage.INBOX_FOLDER_ID);
                    folder.fireElementUpdated(message, message);
                }
				
				// Invoke our callback
                MessageListNamespace.getInstance().invokeItemOpen(message);
            }
            return context;
        }


        /**
         * @see java.lang.Object#toString()
         */
        public String toString()
        {
            return MENU_NAME;
        }
    }


    /**
     * @see ApplicationMessageFolderListener#actionPerformed(int, ApplicationMessage[], ApplicationMessageFolder)
     */
    public void actionPerformed(int action, ApplicationMessage[] messages, ApplicationMessageFolder folder)
    {
        CustomMessageStore messageStore = CustomMessageStore.getInstance(_GUID);

        synchronized(messageStore)
        {
           
			for(int i = 0; i < messages.length; i++)
			{
				CustomMessage message = (CustomMessage) messages[i];

				switch(action)
				{
					case ApplicationMessageFolderListener.MESSAGE_DELETED:			
						messageStore.deleteInboxMessage(message);
						
						// Invoke our callback
						MessageListNamespace.getInstance().invokeItemDeleted(message);
						
						break;
					case ApplicationMessageFolderListener.MESSAGE_MARKED_OPENED:

						// Update message
						message.markRead();

						// Update storage
						messageStore.commitMessage(message);

						// Notify GUI that message has changed
						folder.fireElementUpdated(message, message);
					
						// Invoke our callback
						MessageListNamespace.getInstance().invokeItemMarkedRead(message);
						
						break;
					case ApplicationMessageFolderListener.MESSAGE_MARKED_UNOPENED:

						// Update message
						message.markAsNew();

						// Update storage
						messageStore.commitMessage(message);

						// Notify GUI that message has changed
						folder.fireElementUpdated(message, message);
						
						// Invoke our callback
						MessageListNamespace.getInstance().invokeItemMarkedUnread(message);
						break;
				}
			}
        }
    }
}
