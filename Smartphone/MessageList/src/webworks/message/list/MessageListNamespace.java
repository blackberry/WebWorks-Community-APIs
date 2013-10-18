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

import java.lang.ref.WeakReference;

import webworks.message.list.api.MessageListAddItemFunction;
import webworks.message.list.api.MessageListMarkAllReadFunction;
import webworks.message.list.api.MessageListGetItemFunction;
import webworks.message.list.api.MessageListInitFunction;
import webworks.message.list.api.MessageListItem;
import webworks.message.list.api.MessageListMarkReadFunction;
import webworks.message.list.api.MessageListRemoveItemFunction;
import webworks.message.list.api.MessageListUpdateItemFunction;
import webworks.message.list.model.CustomMessage;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Application;

public class MessageListNamespace extends Scriptable
{
	public static final String FIELD_ONITEMOPENED = "onItemOpened";
	public static final String FIELD_ONITEMMARKEDREAD = "onItemMarkedRead";
	public static final String FIELD_ONITEMMARKEDUNREAD = "onItemMarkedUnread";
	public static final String FIELD_ONITEMDELETED = "onItemDeleted";
	
	private WeakReference _onItemOpened = new WeakReference(null);
	private WeakReference _onItemMarkedRead = new WeakReference(null);
	private WeakReference _onItemMarkedUnread = new WeakReference(null);
	private WeakReference _onItemDeleted = new WeakReference(null);
	
	private String _defaultNewImage = null;
	private String _defaultReadImage = null;
	
	private static long _GUID;


	private static class MessageListNamespaceHolder{
		public static final MessageListNamespace INSTANCE = new MessageListNamespace();
	}

	private MessageListNamespace() {
	}

	public static final MessageListNamespace getInstance(){
		return MessageListNamespaceHolder.INSTANCE;
	}

	public Object getField(String name) throws Exception
	 {
		  if (MessageListInitFunction.NAME.equals(name)) {
			   return new MessageListInitFunction();			
		  } else if (MessageListAddItemFunction.NAME.equals(name)) {
			   return new MessageListAddItemFunction();			
		  } else if (MessageListGetItemFunction.NAME.equals(name)) {
			   return new MessageListGetItemFunction();			
		  } else if (MessageListMarkAllReadFunction.NAME.equals(name)) {
			   return new MessageListMarkAllReadFunction();			
		  } else if (MessageListMarkReadFunction.NAME.equals(name)) {
			   return new MessageListMarkReadFunction();
		  } else if (MessageListRemoveItemFunction.NAME.equals(name)) {
			   return new MessageListRemoveItemFunction();
		  } else if (MessageListUpdateItemFunction.NAME.equals(name)) {
			   return new MessageListUpdateItemFunction();
		  } else if (FIELD_ONITEMOPENED.equals(name)) {
			   return _onItemOpened.get();			
		  } else if (FIELD_ONITEMMARKEDREAD.equals(name)) {
			   return _onItemMarkedRead.get();			
		  } else if (FIELD_ONITEMMARKEDUNREAD.equals(name)) {
			   return _onItemMarkedUnread.get();			
		  } else if (FIELD_ONITEMDELETED.equals(name)) {
			   return _onItemDeleted.get();			
		  }
		  
		  return super.getField(name);  
	 }
	 
	 public boolean putField(String field, Object value) throws Exception {
	
		if (FIELD_ONITEMOPENED.equals(field)) {
			_onItemOpened = new WeakReference(value);	  
		} else if (FIELD_ONITEMMARKEDREAD.equals(field)) {
			_onItemMarkedRead = new WeakReference(value);
		} else if (FIELD_ONITEMMARKEDUNREAD.equals(field)) {
			_onItemMarkedUnread = new WeakReference(value);
		} else if (FIELD_ONITEMDELETED.equals(field)) {
			_onItemDeleted = new WeakReference(value);
		}
		
		return super.putField(field, value);
	}

	public ScriptableFunction getOnItemOpened() {
		Object nullCheck = _onItemOpened.get();
		if(nullCheck != null){
			return (ScriptableFunction)nullCheck;
		}else{
			return null;
		}
	}

	public ScriptableFunction getOnItemMarkedRead() {
		Object nullCheck = _onItemMarkedRead.get();
		if(nullCheck != null){
			return (ScriptableFunction)nullCheck;
		}else{
			return null;
		}
	}

	public ScriptableFunction getOnItemMarkedUnread() {
		Object nullCheck = _onItemMarkedUnread.get();
		if(nullCheck != null){
			return (ScriptableFunction)nullCheck;
		}else{
			return null;
		}
	}

	public ScriptableFunction getOnItemDeleted() {
		Object nullCheck = _onItemDeleted.get();
		if(nullCheck != null){
			return (ScriptableFunction)nullCheck;
		}else{
			return null;
		}
	}

	public String getDefaultNewImage() {
		return _defaultNewImage;
	}

	public String getDefaultReadImage() {
		return _defaultReadImage;
	}

	public void setDefaultNewImage(String value) {
		_defaultNewImage = value;
	}

	public void setDefaultReadImage(String value) {
		_defaultReadImage = value;
	}
				
	public static long getGUID() {
		return _GUID;
	}

	public static void setGUID(long GUID) {
		MessageListNamespace._GUID = GUID;
	}

	public void invokeItemOpen(final CustomMessage message) {
			
			if (getOnItemOpened() == null || message == null) {
				return;
			}
				
			Application.getApplication().invokeLater(new Runnable(){

				public void run() {
					try
					{
						MessageListItem item = new MessageListItem(message);
						
						Object[] result = new Object[] {item};
					
						// Pass the event back to the JavaScript callback
						ScriptableFunction onItemOpened = getOnItemOpened();
						onItemOpened.invoke(onItemOpened, result);
					}
					catch (Exception e) {
						throw new RuntimeException(e.getMessage());
					}
				}
			});
			
		 }
	 
	 public void invokeItemMarkedRead(final CustomMessage message) {
			
			if (getOnItemMarkedRead() == null || message == null) {
				return;
			}
				
			Application.getApplication().invokeLater(new Runnable(){

				public void run() {
					try
					{
						MessageListItem item = new MessageListItem(message);
						
						Object[] result = new Object[]{item};
					
						// Pass the event back to the JavaScript callback
						ScriptableFunction onItemMarkedRead = getOnItemMarkedRead();
						onItemMarkedRead.invoke(onItemMarkedRead, result);
					}
					catch (Exception e) {
						throw new RuntimeException(e.getMessage());
					}
				}
			});
		 }
	 
	 public void invokeItemMarkedUnread(final CustomMessage message) {
			
			if (getOnItemMarkedUnread() == null || message == null) {
				return;
			}
				
			Application.getApplication().invokeLater(new Runnable(){

				public void run() {
					try
					{
						MessageListItem item = new MessageListItem(message);
						
						Object[] result = new Object[]{item};
					
						// Pass the event back to the JavaScript callback
						ScriptableFunction onItemMarkedUnread = getOnItemMarkedUnread();
						onItemMarkedUnread.invoke(onItemMarkedUnread, result);
					}
					catch (Exception e) {
						throw new RuntimeException(e.getMessage());
					}
				}
			});
		 }
		 
		 public void invokeItemDeleted(final CustomMessage message) {
			
			
			if (getOnItemDeleted() == null || message == null) {
				return;
			}
				
			Application.getApplication().invokeLater(new Runnable(){

				public void run() {
					try
					{
						MessageListItem item = new MessageListItem(message);
						
						Object[] result = new Object[]{item};
					
						// Pass the event back to the JavaScript callback
						ScriptableFunction onItemDeleted = getOnItemDeleted();
						onItemDeleted.invoke(onItemDeleted, result);
					}
					catch (Exception e) {
						throw new RuntimeException(e.getMessage());
					}
				}
			});
		 }
	
}
