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

package webworks.message.list.model;

import java.util.Date;

import net.rim.blackberry.api.messagelist.ApplicationMessageFolder;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolderRegistry;
import net.rim.device.api.collection.ReadableList;
import net.rim.device.api.system.Application;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.util.Persistable;
import webworks.message.list.MessageListNamespace;
import webworks.message.list.api.MessageListInitFunction;
import webworks.message.list.impl.CustomMessageDispatcher;
import webworks.message.list.impl.CustomMessageStore;

/**
 * This class provides a sample implementation of the ApplicationMessage
 * interface. It demonstrates how an application can define its own message
 * formats for use with the message store.
 */
public final class CustomMessageImpl extends AbstractCustomMessage implements Persistable {
	

	public static void initializeOnStartup(final MessageListInitFunction initScriptable,final String folderName, final String appName, final long GUID) {	
		
		UiApplication.getUiApplication().invokeLater(new Runnable() {
			public void run() {
				try {
					CustomMessageDispatcher daemon = new CustomMessageDispatcher();

					// Check if this application registered folders already
					ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
					if (reg.getApplicationFolder(INBOX_FOLDER_ID) == null) {
						// Register folders & message types and initialize
						// folders
						daemon.init(folderName, appName, GUID);
					}

				} catch (Exception e) {
					throw new RuntimeException("CustomMessage.initializeOnStartup() error: "
							+ e.getMessage());
				}
			}
		});
	}

	
	
	public static void updateMessage(final CustomMessageImpl message, final String id, final String title, final String description, final String imageNew, 
			final String imageRead) {
			
			// Create a new message
			UiApplication.getUiApplication().invokeLater(new Runnable() {
				public void run() {
					try {
						message.setId(id);
						message.setSender(title);
						message.setSubject(description);
						message.setReceivedTime(System.currentTimeMillis());
						message.setPreviewPicture(imageNew);
						message.setImageNew(imageNew);
						message.setImageRead(imageRead);
						
						// Store message
						CustomMessageStore messageStore = CustomMessageStore.getInstance();
						synchronized (messageStore) {
							messageStore.commitMessage(message);
						}
						// Notify folder
						ApplicationMessageFolder inboxFolder = messageStore.getInboxFolder();
						inboxFolder.fireElementUpdated(message, message);

					} catch (Exception e) {
						Dialog.alert("update message failed:  " + e.getMessage());
					}
				}
			});
		}
	
	public static CustomMessageImpl getMessage( String id) {
		try {
			CustomMessageImpl message = null;
			
			// Loop through our stored messages
			CustomMessageStore messageStore = CustomMessageStore.getInstance();
			synchronized (messageStore) {
				ReadableList list = messageStore.getInboxMessages();
				int size = list.size();
				for (int i = 0; i < size; i++ ) {
					CustomMessageImpl temp = (CustomMessageImpl)list.getAt(i);
					if (temp.getId().equals(id)) {
						message = temp;
						break;
					}						
				}
			}
		
			return message;	

		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
		}
	}
	
	public static CustomMessageImpl[] getMessages() {
		try {
			CustomMessageImpl[] messages = null;
			
			// Loop through our stored messages
			CustomMessageStore messageStore = CustomMessageStore.getInstance();
			synchronized (messageStore) {
				ReadableList list = messageStore.getInboxMessages();
				int size = list.size();
				messages = new CustomMessageImpl[size];
				list.getAt(0, size, messages, 0);
			}
		
			return messages;	

		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
		}
	}
	
	public static void removeMessage(final String id) {

		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				try {			
					// Loop through our stored messages
					CustomMessageStore messageStore = CustomMessageStore.getInstance();
					synchronized (messageStore) {
						ReadableList list = messageStore.getInboxMessages();
						int size = list.size();
						for (int i = 0; i < size; i++ ) {
							CustomMessageImpl message = (CustomMessageImpl)list.getAt(i);
							if (message.getId().equals(id)) {	
								messageStore.deleteInboxMessage(message);
								
								// Update the inbox
								ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
								ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessageImpl.INBOX_FOLDER_ID);
								folder.fireElementRemoved(message);
								
								MessageListNamespace.getInstance().invokeItemDeleted(message);	
								
								break;
							}						
						}
					}
				} catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		});	
	}
	
	public static void markMessageRead(final String id) {

		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				try {			
					// Loop through our stored messages
					CustomMessageStore messageStore = CustomMessageStore.getInstance();
					synchronized (messageStore) {
						ReadableList list = messageStore.getInboxMessages();
						int size = list.size();
						for (int i = 0; i < size; i++ ) {
							CustomMessageImpl message = (CustomMessageImpl)list.getAt(i);
							if (message.getId().equals(id)) {	
								// Update the inbox
								ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
								ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessageImpl.INBOX_FOLDER_ID);
								
								// Update message
								message.markRead();

								// Update storage
								messageStore.commitMessage(message);

								// Notify GUI that message has changed
								folder.fireElementUpdated(message, message);
							
								// Invoke our callback
								MessageListNamespace.getInstance().invokeItemMarkedRead(message);
								
								break;
							}						
						}
					}
				} catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		});	
	}
	
	public static void markAllMessageRead() {

		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				try {			
					// Loop through our stored messages
					CustomMessageStore messageStore = CustomMessageStore.getInstance();
					synchronized (messageStore) {
						ReadableList list = messageStore.getInboxMessages();
						int size = list.size();
						for (int i = 0; i < size; i++ ) {
							CustomMessageImpl message = (CustomMessageImpl)list.getAt(i);
								
							// Update the inbox
							ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
							ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessageImpl.INBOX_FOLDER_ID);
							
							// Update message
							message.markRead();													

							// Notify GUI that message has changed
							folder.fireElementUpdated(message, message);
						
							//  We called MarkAll Read, skip the callback
							//MessageListNamespace.getInstance().invokeItemMarkedRead(message);
						}
						messageStore.persist();
					}
				} catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		});	
	}
	
	

	public static void clearMessages() {
		CustomMessageDispatcher.markAllAsRead();
	}

	
}
