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
	
public final class MessageListInitFunction extends ScriptableFunction
{
    public static final String NAME = "init";
   
   
	private boolean _alreadyInit = false;	
   
	public MessageListInitFunction() {
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
				final long GUID = Long.parseLong(guid);
				String folderName = (String)optionsSent.getField("folderName");
					
				// Set the namespace properties
				MessageListNamespace namespace = MessageListNamespace.getInstance();
				namespace.setDefaultNewImage((String)optionsSent.getField("defaultNewImage"));
				namespace.setDefaultReadImage((String)optionsSent.getField("defaultReadImage"));
				
				// Do all our actual registration stuff
				CustomMessage.initializeOnStartup(this, folderName, "Example App", GUID);
					
				// Set our init flag
				_alreadyInit = true;
		   }catch ( Exception e) {
			   throw new IllegalArgumentException("Could not complete initialization - "+ e.getMessage());
		   }
		}
		return UNDEFINED;
	 }	 	 
}