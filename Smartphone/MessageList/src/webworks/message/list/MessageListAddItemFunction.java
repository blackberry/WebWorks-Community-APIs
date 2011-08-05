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

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import java.util.Date;
import net.rim.device.api.ui.component.Dialog;
	
public final class MessageListAddItemFunction extends ScriptableFunction
{
	public static final String NAME = "addItem";
	private MessageListNamespace _namespace = null;
	
	public MessageListAddItemFunction(MessageListNamespace namespace) {
		_namespace = namespace;
	}
	
	public Object invoke(Object obj, Object[] args) throws Exception
	 {
		if (args != null)
		{
		   try
		   {
				// Set defaults
				String imageNew = _namespace.getDefaultNewImage();
				String imageRead = _namespace.getDefaultReadImage();
		   
				// Grab the options object and its details
				Scriptable item = (Scriptable)args[0];
				String id = (String)item.getField(MessageListItem.FIELD_ID);
				String title = (String)item.getField(MessageListItem.FIELD_TITLE);
				String description = (String)item.getField(MessageListItem.FIELD_DESCRIPTION);
				/*String imageNew = (String)item.getField(MessageListItem.FIELD_IMAGENEW);
				String imageRead = (String)item.getField(MessageListItem.FIELD_IMAGEREAD);
				*/
								
				if (id == null)
					throw new Exception("Invalid parameter. 'id' cannot be null");
				
				if (title == null)
					throw new Exception("Invalid parameter. 'title' cannot be null");
					
				if (description == null)
					throw new Exception("Invalid parameter. 'description' cannot be null");
				
				// Actually add it to a list
				CustomMessage.createMessage(id,title,description, imageNew, imageRead);
		   } 
		   catch (Exception e) {
				throw new RuntimeException(e.getMessage());
		   }
		}
		return UNDEFINED;
	 }
}