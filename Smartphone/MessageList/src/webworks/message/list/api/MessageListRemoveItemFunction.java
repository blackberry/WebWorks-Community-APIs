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

import net.rim.device.api.script.ScriptableFunction;
import webworks.message.list.model.CustomMessageServices;


public class MessageListRemoveItemFunction extends ScriptableFunction
{
	public static final String NAME = "removeItem";
	
	 
	 public Object invoke(Object obj, Object[] args) throws Exception
	 {
		if (args != null && args.length == 1)
		{
		   try
		   {
				String id = (String)args[0];
				CustomMessageServices.removeMessage(id);
				
		   } catch (final Exception e) {
				throw new RuntimeException(e.getMessage());
		   }
		}
		
		return UNDEFINED;
	}
}
	

