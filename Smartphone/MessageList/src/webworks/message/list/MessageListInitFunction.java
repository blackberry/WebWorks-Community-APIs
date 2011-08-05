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

import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.system.Application;
import net.rim.device.api.ui.UiApplication;
	
public final class MessageListInitFunction extends ScriptableFunction
{
    public static final String NAME = "init";
   
   
	private boolean _alreadyInit = false;
	
	private MessageListNamespace _namespace;
   
	public MessageListInitFunction(MessageListNamespace namespace) {
		_namespace = namespace;
	}

   
	public Object invoke(Object obj, Object[] args) throws Exception
	{
		if (args != null && !_alreadyInit)
		{
		   try
		   {
				// Grab the options data
				Scriptable optionsSent = (Scriptable)args[0];
				String guid = (String)optionsSent.getField("guid");
				String folderName = (String)optionsSent.getField("folderName");
					
				// Set the namespace properties
				_namespace.setDefaultNewImage((String)optionsSent.getField("defaultNewImage"));
				_namespace.setDefaultReadImage((String)optionsSent.getField("defaultReadImage"));
				
				// Do all our actual registration stuff
				CustomMessage.initializeOnStartup(this, folderName, "Example App");
					
				// Set our init flag
				_alreadyInit = true;
		   }catch ( Exception e) {}
		}
		return UNDEFINED;
	 }
	 
	 public MessageListNamespace getNamespace() {
		return _namespace;
	}
	 
	 
	 public void invokeItemOpen(final CustomMessage message) {
		
		if (_namespace.getOnItemOpened() == null || message == null) {
			return;
		}
			
		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				try
				{
					MessageListItem item = new MessageListItem();
					item.putField(MessageListItem.FIELD_ID, new String(message.getId()));
					item.putField(MessageListItem.FIELD_TITLE, new String(message.getContact()));
					item.putField(MessageListItem.FIELD_DESCRIPTION, new String(message.getSubject()));
					
					Object[] result = new Object[1];
					result[0] = item;
				
					// Pass the event back to the JavaScript callback
					ScriptableFunction onItemOpened = _namespace.getOnItemOpened();
					onItemOpened.invoke(onItemOpened, result);
				}
				catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		});
		
	 }
	 
	 public void invokeItemMarkedRead(final CustomMessage message) {
		
		if (_namespace.getOnItemMarkedRead() == null || message == null) {
			return;
		}
			
		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				try
				{
					MessageListItem item = new MessageListItem();
					item.putField(MessageListItem.FIELD_ID, new String(message.getId()));
					item.putField(MessageListItem.FIELD_TITLE, new String(message.getContact()));
					item.putField(MessageListItem.FIELD_DESCRIPTION, new String(message.getSubject()));
					
					Object[] result = new Object[1];
					result[0] = item;
				
					// Pass the event back to the JavaScript callback
					ScriptableFunction onItemMarkedRead = _namespace.getOnItemMarkedRead();
					onItemMarkedRead.invoke(onItemMarkedRead, result);
				}
				catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		});
	 }
	 
	 public void invokeItemMarkedUnread(final CustomMessage message) {
		
		if (_namespace.getOnItemMarkedUnread() == null || message == null) {
			return;
		}
			
		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				try
				{
					MessageListItem item = new MessageListItem();
					item.putField(MessageListItem.FIELD_ID, new String(message.getId()));
					item.putField(MessageListItem.FIELD_TITLE, new String(message.getContact()));
					item.putField(MessageListItem.FIELD_DESCRIPTION, new String(message.getSubject()));
					
					Object[] result = new Object[1];
					result[0] = item;
				
					// Pass the event back to the JavaScript callback
					ScriptableFunction onItemMarkedUnread = _namespace.getOnItemMarkedUnread();
					onItemMarkedUnread.invoke(onItemMarkedUnread, result);
				}
				catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		});
	 }
	 
	 public void invokeItemDeleted(final CustomMessage message) {
		
		
		if (_namespace.getOnItemDeleted() == null || message == null) {
			return;
		}
			
		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				try
				{
					MessageListItem item = new MessageListItem();
					item.putField(MessageListItem.FIELD_ID, new String(message.getId()));
					item.putField(MessageListItem.FIELD_TITLE, new String(message.getContact()));
					item.putField(MessageListItem.FIELD_DESCRIPTION, new String(message.getSubject()));
					
					Object[] result = new Object[1];
					result[0] = item;
				
					// Pass the event back to the JavaScript callback
					ScriptableFunction onItemDeleted = _namespace.getOnItemDeleted();
					onItemDeleted.invoke(onItemDeleted, result);
				}
				catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		});
	 }
}