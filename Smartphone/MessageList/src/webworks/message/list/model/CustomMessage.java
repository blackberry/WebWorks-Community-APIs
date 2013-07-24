package webworks.message.list.model;

import java.util.Date;

import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Application;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.util.Persistable;
import webworks.message.list.MessageListNamespace;
import webworks.message.list.api.MessageListItem;

public class CustomMessage implements CustomMessageInterface, Persistable {

	private String _id;
	private String _sender;
	private String _subject;
	private String _message;
	private long _receivedTime;
	private boolean _isNew;
	private boolean _deleted;
	private String _replyMessage;
	private long _replyTime;
	private int _type;
	
	// Images
	private String _previewPicture;
	private String _imageNew = null;
	private String _imageRead = null;
	
	/**
	 * Creates a new DemoMesage object
	 */
	public CustomMessage() {
		this(null,null,null,null,System.currentTimeMillis());
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
	public void messageDeleted() {
		_isNew = false;
		_deleted = true;
	}

	/**
	 * Marks this message as new
	 */
	public void markAsNew() {
		_isNew = true;
		_replyMessage = null;
	}

	/**
	 * Marks this message as read
	 */
	public void markRead() {
		_isNew = false;
	}
	
	/**
	 * Marks this message as unread
	 */
	public void markUnRead() {
		_isNew = true;
	}

	/**
	 * Indicates whether this message is new or not
	 * 
	 * @return True if the message is new, false otherwise
	 */
	public boolean isNew() {
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
	void setPreviewPicture(String image) {
		_previewPicture = image;
	}
	
	/**
	 * Sets the "New" image
	 * 
	 * @param image
	 *            The desired new picture of this message
	 */
	void setImageNew(String image) {
		_imageNew = image;
	}
	
	public String getImageNew() {
		return _imageNew;
	}
	
	/**
	 * Sets the "Read" image
	 * 
	 * @param image
	 *            The desired read picture of this message
	 */
	void setImageRead(String image) {
		_imageRead = image;
	}
	
	public String getImageRead() {
		return _imageRead;
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
		return _type;
	}
	
	public void setType(int newType) {
		_type = newType;
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
		return EncodedImage.getEncodedImageResource(_previewPicture==null?MessageListNamespace.getInstance().getDefaultNewImage():_previewPicture);
	}


	public void invokeItemOpen(final MessageListNamespace messageListNamespace) {
			
			if (messageListNamespace.getOnItemOpened() == null || this == null) {
				return;
			}
				
			Application.getApplication().invokeLater(new Runnable(){
	
				public void run() {
					try
					{
						MessageListItem item = new MessageListItem();
						item.putField(MessageListItem.FIELD_ID, new String(getId()));
						item.putField(MessageListItem.FIELD_TITLE, new String(getContact()));
						item.putField(MessageListItem.FIELD_DESCRIPTION, new String(getSubject()));
						
						Object[] result = new Object[] {item};
					
						// Pass the event back to the JavaScript callback
						ScriptableFunction onItemOpened = messageListNamespace.getOnItemOpened();
						onItemOpened.invoke(onItemOpened, result);
					}
					catch (Exception e) {
						throw new RuntimeException(e.getMessage());
					}
				}
			});
			
		 }

}
