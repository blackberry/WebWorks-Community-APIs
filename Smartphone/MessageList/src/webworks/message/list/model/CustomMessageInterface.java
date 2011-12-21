package webworks.message.list.model;

import net.rim.blackberry.api.messagelist.ApplicationMessage;

public interface CustomMessageInterface extends ApplicationMessage {
	
	public static final int DEFAULT_IMAGE_TYPE = 0x01;
	
	public static final long INBOX_FOLDER_ID = 0x2fb5115c0e4a6c33L;
	
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

}
