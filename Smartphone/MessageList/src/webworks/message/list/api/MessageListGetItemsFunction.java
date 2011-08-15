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

package webworks.message.list.api;

import webworks.message.list.impl.CustomMessage;
import net.rim.device.api.script.ScriptableFunction;
	
public final class MessageListGetItemsFunction extends ScriptableFunction
{
	public static final String NAME = "getItem";
	
	public Object invoke(Object obj, Object[] args) throws Exception
	{
		if (args != null && args.length == 1)
		{
		   try
		   {
				CustomMessage[] messages = CustomMessage.getMessages();

				MessageListItem[] items = null;
				if (messages != null){
					items = new MessageListItem[messages.length];
					for(int i =0;i<messages.length;i++){
						items[i] =  new MessageListItem();
						items[i].putField(MessageListItem.FIELD_ID, new String(messages[i].getId()));
						items[i].putField(MessageListItem.FIELD_TITLE, new String(messages[i].getContact()));
						items[i].putField(MessageListItem.FIELD_DESCRIPTION, new String(messages[i].getSubject()));
					}
				}
				
				return items;		
		   } 
		   catch (Exception e) {
				throw new RuntimeException(e.getMessage());
		   }
		}
		else {
			throw new RuntimeException("Invalid number of parameters");
		}
		
	 }
}