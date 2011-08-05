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

import net.rim.blackberry.api.menuitem.ApplicationMenuItem;
//import net.rim.blackberry.api.messagelist.ApplicationFolderIntegrationConfig;
import net.rim.blackberry.api.messagelist.ApplicationIcon;
import net.rim.blackberry.api.messagelist.ApplicationIndicator;
import net.rim.blackberry.api.messagelist.ApplicationIndicatorRegistry;
import net.rim.blackberry.api.messagelist.ApplicationMessage;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolder;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolderListener;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolderRegistry;
import net.rim.device.api.system.ApplicationDescriptor;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.ui.image.Image;
import net.rim.device.api.ui.image.ImageFactory;
import net.rim.device.api.system.Application;

/**
 * Daemon process that runs in the background. It's tasks include non-gui
 * message operations such as message deletion, marking messages as read/unread
 */
public final class CustomMessageDispatcher implements ApplicationMessageFolderListener
{
    private static String MENU_NAME = "Open";
    private static String MENU_NAME_MARK_READ = "Mark Read";
	private static String MENU_NAME_MARK_UNREAD = "Mark Unread";
	
	private MessageListInitFunction _initScriptable;
	private static String _folderName;

    /**
     * Called during device startup. Registers application descriptors, message
     * folder listeners, message icons and menu items.
     */
    void init(MessageListInitFunction initScriptable, String folderName, String appName)
    {
		_initScriptable = initScriptable;
		_folderName = folderName;
		
		// 1. Register folders and application descriptors ----------------------
		ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
	  
		// Some context menu items don't need a GUI (e.g. an item for deleting a
		// message) and will be run in the current daemon application.
		ApplicationDescriptor daemonDescr = ApplicationDescriptor.currentApplicationDescriptor();

		// Main application descriptor - causes application to be launched with
		// default welcome screen if a user clicks on the "Message List Demo"
		// header in the home screen notifications view.
		ApplicationDescriptor mainDescr = ApplicationDescriptor.currentApplicationDescriptor();
		
		// This application descriptor launches this application with a GUI to
		// execute listener callbacks, e.g. to display a message.    
		ApplicationDescriptor uiCallbackDescr = ApplicationDescriptor.currentApplicationDescriptor();
		
		CustomMessageStore messages = CustomMessageStore.getInstance();
		ApplicationMessageFolder inbox = reg.registerFolder(CustomMessage.INBOX_FOLDER_ID, _folderName, messages.getInboxMessages());

		// Register as a listener for callback notifications
		inbox.addListener(this, ApplicationMessageFolderListener.MESSAGE_DELETED | ApplicationMessageFolderListener.MESSAGE_MARKED_OPENED
			| ApplicationMessageFolderListener.MESSAGE_MARKED_UNOPENED, daemonDescr);
			
		messages.setFolder(inbox);

		// We've registered two folders, specify root folder name for the [View Folder] screen.
		reg.setRootFolderName(_folderName);
			
		// 2. Register message icons -------------------------------------------
		ApplicationIcon newIcon = new ApplicationIcon(EncodedImage.getEncodedImageResource(initScriptable.getNamespace().getDefaultNewImage()), true);
		ApplicationIcon readIcon = new ApplicationIcon(EncodedImage.getEncodedImageResource(initScriptable.getNamespace().getDefaultReadImage()), true);

		reg.registerMessageIcon(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_NEW, newIcon);
		reg.registerMessageIcon(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_OPENED, readIcon);

		// 3. Register message menu items --------------------------------------
		ApplicationMenuItem openMenuItem = new OpenContextMenu(0x230010, initScriptable);
		ApplicationMenuItem markOpenedMenuItem = new MarkOpenedContextMenu(0x230030, initScriptable);
		ApplicationMenuItem markUnreadMenuItem = new MarkUnreadContextMenu(0x230031, initScriptable);
		
		ApplicationMenuItem[] newGuiMenuItems = new ApplicationMenuItem[] {openMenuItem};
		ApplicationMenuItem[] newDaemonMenuItems = new ApplicationMenuItem[] {markOpenedMenuItem};
		ApplicationMenuItem[] openedGuiMenuItems = new ApplicationMenuItem[] {openMenuItem};
		ApplicationMenuItem[] markedUnreadMenuItems = new ApplicationMenuItem[] {markUnreadMenuItem};
		
		reg.registerMessageMenuItems(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_NEW, newGuiMenuItems, uiCallbackDescr);
		reg.registerMessageMenuItems(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_NEW, newDaemonMenuItems, daemonDescr);
		reg.registerMessageMenuItems(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_OPENED, openedGuiMenuItems, uiCallbackDescr);
		reg.registerMessageMenuItems(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_OPENED, markedUnreadMenuItems, daemonDescr);
		reg.setBulkMarkOperationsSupport(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_NEW, true, false);
		reg.setBulkMarkOperationsSupport(CustomMessage.DEMO_MESSAGE_TYPE, CustomMessage.STATUS_OPENED, false, true);	
    }
	

    public static void markAllAsRead()
    {
    	try {
			CustomMessageStore messageStore = CustomMessageStore.getInstance();
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
		private MessageListInitFunction _initScriptable;
	
        /**
         * Creates a new ApplicationMenuItem instance with provided menu position
         * 
         * @param order Display order of this item, lower numbers correspond to  higher placement in the menu
         */
        MarkOpenedContextMenu(int order, MessageListInitFunction initScriptable)
        {
            super(order);
			_initScriptable = initScriptable;
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
				_initScriptable.invokeItemMarkedRead(message);
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
		private MessageListInitFunction _initScriptable;
	
        /**
         * Creates a new ApplicationMenuItem instance with provided menu position
         * 
         * @param order Display order of this item, lower numbers correspond to  higher placement in the menu
         */
        MarkUnreadContextMenu(int order, MessageListInitFunction initScriptable)
        {
            super(order);
			_initScriptable = initScriptable;
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
				_initScriptable.invokeItemMarkedUnread(message);
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
		private MessageListInitFunction _initScriptable;
		
        /**
         * Creates a new ApplicationMenuItem instance with provided menu position
         * 
         * @param order Display order of this item, lower numbers correspond to higher placement in the menu
         */
        public OpenContextMenu(int order, MessageListInitFunction initScriptable)
        {
            super(order);
			_initScriptable = initScriptable;
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
				_initScriptable.invokeItemOpen(message);
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
        CustomMessageStore messageStore = CustomMessageStore.getInstance();

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
						_initScriptable.invokeItemDeleted(message);
						
						break;
					case ApplicationMessageFolderListener.MESSAGE_MARKED_OPENED:

						// Update message
						message.markRead();

						// Update storage
						messageStore.commitMessage(message);

						// Notify GUI that message has changed
						folder.fireElementUpdated(message, message);
					
						// Invoke our callback
						_initScriptable.invokeItemMarkedRead(message);
						
						break;
					case ApplicationMessageFolderListener.MESSAGE_MARKED_UNOPENED:

						// Update message
						message.markAsNew();

						// Update storage
						messageStore.commitMessage(message);

						// Notify GUI that message has changed
						folder.fireElementUpdated(message, message);
						
						// Invoke our callback
						_initScriptable.invokeItemMarkedUnread(message);
						break;
				}
			}
        }
    }
}
