package webworks.message.list.model;

import webworks.message.list.impl.CustomMessageStore;
import net.rim.blackberry.api.messagelist.ApplicationMessageFolder;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.Dialog;

public class CustomMessageFactory {
	
	public static void createMessage(final String id, final String title, final String description, final String imageNew, 
			final String imageRead) {
			
			// Create a new message
			UiApplication.getUiApplication().invokeLater(new Runnable() {
				public void run() {
					try {
						CustomMessageImpl message = new CustomMessageImpl();
						message.setId(id);
						message.setSender(title);
						message.setSubject(description);
						message.setReceivedTime(System.currentTimeMillis());
						message.setPreviewPicture(imageNew);
						message.setImageNew(imageNew);
						message.setImageRead(imageRead);
						
						// Store message
						CustomMessageStore messageStore = CustomMessageStore.getInstance(_GUID);
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

}
