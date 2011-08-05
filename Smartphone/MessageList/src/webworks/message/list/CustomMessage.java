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

import java.util.Date;

import net.rim.blackberry.api.mail.ServiceConfiguration;
import net.rim.blackberry.api.mail.Session;
import net.rim.blackberry.api.mail.Store;
import net.rim.blackberry.api.messagelist.ApplicationMessage;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolder;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolderRegistry;
import net.rim.device.api.system.*;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.collection.ReadableList;

/**
 * This class provides a sample implementation of the ApplicationMessage
 * interface. It demonstrates how an application can define its own message
 * formats for use with the message store.
 */
public final class CustomMessage implements ApplicationMessage {

	static final int DEMO_MESSAGE_TYPE = 0x01;

	/* com.rimdevrel.cdp.messagelistinterceptor */
	//static final long KEY = 0x39d90c5bc6899541L; // Base folder key

	/* com.rimdevrel.cdp.messagelistinterceptor.INBOX_FOLDER_ID */
	static final long INBOX_FOLDER_ID = 0x2fb5115c0e4a6c33L;
	
	static MessageListInitFunction _initScriptable;


	/**
	 * Flag for deleted messages. The lower 16 bits are RIM-reserved, so we have
	 * to use higher 16 bits.
	 */
	static final int FLAG_DELETED = 1 << 17;
	// All our messages are received, we don't show sent messages
	static final int BASE_STATUS = ApplicationMessage.Status.INCOMING;
	static final int STATUS_NEW = BASE_STATUS | ApplicationMessage.Status.UNOPENED;
	static final int STATUS_OPENED = BASE_STATUS | ApplicationMessage.Status.OPENED;
	static final int STATUS_DELETED = BASE_STATUS | FLAG_DELETED;

	private String _id;
	private String _sender;
	private String _subject;
	private String _message;
	private long _receivedTime;
	private boolean _isNew;
	private boolean _deleted;
	private String _replyMessage;
	private long _replyTime;
	
	// Images
	private EncodedImage _previewPicture;
	private EncodedImage _imageNew = null;
	private EncodedImage _imageRead = null;
	private EncodedImage _imageDeleted = null;

	public static void initializeOnStartup(final MessageListInitFunction initScriptable,final String folderName, final String appName) {
	
		_initScriptable = initScriptable;
	
		UiApplication.getUiApplication().invokeLater(new Runnable() {
			public void run() {
				try {
					CustomMessageDispatcher daemon = new CustomMessageDispatcher();

					// Check if this application registered folders already
					ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
					if (reg.getApplicationFolder(INBOX_FOLDER_ID) == null) {
						// Register folders & message types and initialize
						// folders
						daemon.init(initScriptable, folderName, appName);
					}

				} catch (Exception e) {
					throw new RuntimeException("CustomMessage.initializeOnStartup() error: "
							+ e.getMessage());
				}
			}
		});
	}

	public static void createMessage(final String id, final String title, final String description, final String imageNew, 
		final String imageRead) {
		
		// Create a new message
		UiApplication.getUiApplication().invokeLater(new Runnable() {
			public void run() {
				try {
					CustomMessage message = new CustomMessage();
					message.setId(id);
					message.setSender(title);
					message.setSubject(description);
					message.setReceivedTime(System.currentTimeMillis());
					message.setPreviewPicture(EncodedImage.getEncodedImageResource(imageNew));
					message.setImageNew(EncodedImage.getEncodedImageResource(imageNew));
					message.setImageRead(EncodedImage.getEncodedImageResource(imageRead));
					
					// Store message
					CustomMessageStore messageStore = CustomMessageStore.getInstance();
					synchronized (messageStore) {
						messageStore.addInboxMessage(message);
					}
					// Notify folder
					ApplicationMessageFolder inboxFolder = messageStore.getInboxFolder();
					inboxFolder.fireElementAdded(message);

				} catch (Exception e) {
					Dialog.alert("create message failed:  " + e.getMessage());
				}
			}
		});
	}
	
	public static CustomMessage getMessage( String id) {
		try {
			CustomMessage message = null;
			
			// Loop through our stored messages
			CustomMessageStore messageStore = CustomMessageStore.getInstance();
			synchronized (messageStore) {
				ReadableList list = messageStore.getInboxMessages();
				int size = list.size();
				for (int i = 0; i < size; i++ ) {
					CustomMessage temp = (CustomMessage)list.getAt(i);
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
							CustomMessage message = (CustomMessage)list.getAt(i);
							if (message.getId().equals(id)) {	
								messageStore.deleteInboxMessage(message);
								
								// Update the inbox
								ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
								ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessage.INBOX_FOLDER_ID);
								folder.fireElementRemoved(message);
								
								_initScriptable.invokeItemDeleted(message);	
								
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
							CustomMessage message = (CustomMessage)list.getAt(i);
							if (message.getId().equals(id)) {	
								// Update the inbox
								ApplicationMessageFolderRegistry reg = ApplicationMessageFolderRegistry.getInstance();
								ApplicationMessageFolder folder = reg.getApplicationFolder(CustomMessage.INBOX_FOLDER_ID);
								
								// Update message
								message.markRead();

								// Update storage
								messageStore.commitMessage(message);

								// Notify GUI that message has changed
								folder.fireElementUpdated(message, message);
							
								// Invoke our callback
								_initScriptable.invokeItemMarkedRead(message);
								
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
	
	

	public static void clearMessages() {
		CustomMessageDispatcher.markAllAsRead();
	}

	/**
	 * Creates a new DemoMesage object
	 */
	public CustomMessage() {
		_isNew = true;
	}

	/**
	 * Constructs a CustomMessage object with specified properties
	 * 
	 * @param sender
	 *            The name of the sender
	 * @param subject
	 *            The subject of the message
	 * @param message
	 *            The body of the message
	 * @param receivedTime
	 *            The time stamp for when the message was received
	 */
	CustomMessage(String id, String sender, String subject, String message,
			long receivedTime) {
		_id = id;
		_sender = sender;
		_subject = subject;
		_message = message;
		_receivedTime = receivedTime;
		_isNew = true;
	}

	/**
	 * Stores the reply message and sets the reply time
	 * 
	 * @param message
	 *            The reply message
	 */
	void reply(String message) {
	}

	/**
	 * Marks this message as deleted
	 */
	void messageDeleted() {
		_isNew = false;
		_deleted = true;
	}

	/**
	 * Marks this message as new
	 */
	void markAsNew() {
		_isNew = true;
		_replyMessage = null;
	}

	/**
	 * Marks this message as read
	 */
	void markRead() {
		_isNew = false;
	}
	
	/**
	 * Marks this message as unread
	 */
	void markUnRead() {
		_isNew = true;
	}

	/**
	 * Indicates whether this message is new or not
	 * 
	 * @return True if the message is new, false otherwise
	 */
	boolean isNew() {
		return _isNew;
	}

	/**
	 * Indicates whether this message has been replied to or not
	 * 
	 * @return True if the message has been replied to, false otherwise
	 */
	boolean hasReplied() {
		return _replyMessage != null;
	}

	/**
	 * Sets the name of the sender who sent this message
	 * 
	 * @param sender
	 *            The name of the sender
	 */
	void setSender(String sender) {
		_sender = sender;
	}
	
	/**
	 * Sets the id of this message
	 * 
	 * @param id
	 *            The id of the message
	 */
	void setId(String id) {
		_id = id;
	}
	
	public String getId() {
		return _id;
	}

	/**
	 * Sets the subject of this message
	 * 
	 * @param subject
	 *            The subject of this message
	 */
	void setSubject(String subject) {
		_subject = subject;
	}

	/**
	 * Sets the time at which this message was received
	 * 
	 * @param receivedTime
	 *            The time at which this message was received
	 */
	void setReceivedTime(long receivedTime) {
		_receivedTime = receivedTime;
	}

	/**
	 * Sets the message body
	 * 
	 * @param message
	 *            The message body
	 */
	void setMessage(String message) {
		_message = message;
	}

	/**
	 * Retrieves the message body
	 * 
	 * @return The message body
	 */
	String getMessage() {
		return _message;
	}

	/**
	 * Sets the preview picture for this message
	 * 
	 * @param image
	 *            The desired preview picture of this message
	 */
	void setPreviewPicture(EncodedImage image) {
		_previewPicture = image;
	}
	
	/**
	 * Sets the "New" image
	 * 
	 * @param image
	 *            The desired new picture of this message
	 */
	void setImageNew(EncodedImage image) {
		_imageNew = image;
	}
	
	/**
	 * Sets the "Read" image
	 * 
	 * @param image
	 *            The desired read picture of this message
	 */
	void setImageRead(EncodedImage image) {
		_imageRead = image;
	}
	
	
	// Implementation of ApplicationMessage ------------------------------------
	/**
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getContact()
	 */
	public String getContact() {
		return _sender;
	}
	

	/**
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getStatus()
	 */
	public int getStatus() {
		// Form message list status based on current message state
		if (_isNew) {
			return CustomMessage.STATUS_NEW;
		}
		if (_deleted) {
			return CustomMessage.STATUS_DELETED;
		}
		if (_replyMessage != null) {
			// return CustomMessage.STATUS_REPLIED;
		}
		return CustomMessage.STATUS_OPENED;
	}

	/**
	 * 
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getSubject()
	 */
	public String getSubject() {
		if (_replyMessage != null) {
			return "Re: " + _subject;
		} else {
			return _subject;
		}
	}

	/**
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getTimestamp()
	 */
	public long getTimestamp() {
		return _receivedTime;
	}

	/**
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getType()
	 */
	public int getType() {
		// All messages have the same type
		return DEMO_MESSAGE_TYPE;
	}

	/**
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getPreviewText()
	 */
	public String getPreviewText() {
		if (_message == null) {
			return null;
		}

		StringBuffer buffer = new StringBuffer(_message);

		if (_replyMessage != null) {
			buffer.append(". You replied on ").append(new Date(_replyTime))
					.append(": ").append(_replyMessage);
		}

		return buffer.length() > 100 ? buffer.toString().substring(0, 100)
				+ " ..." : buffer.toString();
	}

	/**
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getCookie()
	 */
	public Object getCookie(int cookieId) {
		return null;
	}

	/**
	 * 
	 * @see net.rim.blackberry.api.messagelist.ApplicationMessage#getPreviewPicture()
	 */
	public Object getPreviewPicture() {
		return _previewPicture;
	}
}
