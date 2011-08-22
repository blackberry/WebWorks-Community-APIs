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

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import webworks.message.list.model.CustomMessage;
import webworks.message.list.model.CustomMessageServices;
	
public final class MessageListUpdateItemFunction extends ScriptableFunction
{
	public static final String NAME = "updateItem";
	
	public Object invoke(Object obj, Object[] args) throws Exception
	{
		if (args != null && args.length == 1)
		{
		   try
		   {
			   
			   // Set defaults
				String imageNew = null;
				String imageRead = null;
		   
				// Grab the options object and its details
				Scriptable item = (Scriptable)args[0];
				String id = (String)item.getField(MessageListItem.FIELD_ID);
				String title = (String)item.getField(MessageListItem.FIELD_TITLE);
				String description = (String)item.getField(MessageListItem.FIELD_DESCRIPTION);
				//Optional Parameter
				Object imageNewArg = item.getField(MessageListItem.FIELD_IMAGENEW);
				if(imageNewArg != UNDEFINED){
					imageNew = (String)imageNewArg;
				}
				Object imageReadArg = item.getField(MessageListItem.FIELD_IMAGEREAD);
				if(imageReadArg != UNDEFINED){
					imageRead = (String)imageReadArg;
				}
				
								
				if (id == null)
					throw new Exception("Invalid parameter. 'id' cannot be null");
				
				if (title == null)
					throw new Exception("Invalid parameter. 'title' cannot be null");
					
				if (description == null)
					throw new Exception("Invalid parameter. 'description' cannot be null");
			   
				CustomMessage message = CustomMessageServices.getMessage(id);

				if (message == null)
					throw new Exception("No item exists to update.");
					
				CustomMessageServices.updateMessage(message,id,title,description, imageNew, imageRead);	
		   } 
		   catch (Exception e) {
				throw new RuntimeException(e.getMessage());
		   }
		}
		else {
			throw new RuntimeException("Invalid number of parameters");
		}
		
		return UNDEFINED;
		
	 }
}