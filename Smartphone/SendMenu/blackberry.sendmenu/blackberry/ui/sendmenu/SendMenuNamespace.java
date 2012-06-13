/*
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

package blackberry.ui.sendmenu;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;

import net.rim.blackberry.api.sendmenu.*;
import net.rim.device.api.ui.menu.SubMenu;

import org.json.me.JSONException;
import org.json.me.JSONObject;
import java.util.Hashtable;
import java.util.Enumeration;
import net.rim.device.api.ui.container.MainScreen;

public class SendMenuNamespace extends Scriptable 
{

	// SendCommand Types
	public static final String PROPERTY_TYPE_PATH  	= "TYPE_PATH";
	public static final String PROPERTY_TYPE_TEXT 	= "TYPE_TEXT";
	
	// SendCommand JSON Keys
	public static final String PROPERTY_PATH 		= "PATH";
	public static final String PROPERTY_SUBJECT		= "SUBJECT";
	public static final String PROPERTY_TEXT 		= "TEXT";
	
	// JS API Methods
	public static final String FUNCTION_ADD     	= "add";
	public static final String FUNCTION_REMOVE     	= "remove";
	public static final String FUNCTION_LABEL		= "setLabel";

    private Hashtable _fields;
	
	private SubMenu menu = null;
	private String menuLabel = "Share";

	public SendMenuNamespace()
	{
        _fields = new Hashtable();
        _fields.put( PROPERTY_TYPE_PATH, new Integer(SendCommand.TYPE_PATH) );
        _fields.put( PROPERTY_TYPE_TEXT, new Integer(SendCommand.TYPE_TEXT) );
		_fields.put( PROPERTY_PATH, SendCommandContextKeys.PATH );
		_fields.put( PROPERTY_SUBJECT, SendCommandContextKeys.SUBJECT );
		_fields.put( PROPERTY_TEXT, SendCommandContextKeys.TEXT );

        _fields.put( FUNCTION_ADD, new SendMenuAddFunction() );
		_fields.put( FUNCTION_REMOVE, new SendMenuRemoveFunction() );
		_fields.put( FUNCTION_LABEL, new SetLabelFunction() );
	}
	
	/**
	 * Adds the SendMenu
	 *
	 * Arguments Expected (by array index):
	 *
	 * 0: SendCommand Type (from exposed constant: TYPE_PATH or TYPE_TEXT) [REQUIRED]
	 * 1: JSON Context Object with valid keys (from exposed constants: PATH, SUBJECT, and/or TEXT) [REQUIRED]
	 * 2: Boolean for whether to get all send commands, regardless of availability (Default FALSE)
	 * 3: Integer for Send Menu ordering (Default 0)
	 * 4: Integer for Send Menu priority (Default 0)
	 * 5: JSON Filter Object for context filter, or null for none
	 */
	public class SendMenuAddFunction extends ScriptableFunction {

		public Object invoke(Object obj, Object[] args) throws Exception 
		{
			Object result = Boolean.FALSE;

			if (args.length >= 2 && menu == null)
			{
				MainScreen screen = (MainScreen) SendMenuExtension.getBrowserField().getScreen();
		
				int sendType = Integer.parseInt(args[0].toString());
				boolean getAll = (args.length>=3) ? ((Boolean) args[2]).booleanValue() : false;
				int ordering = (args.length>=4) ? Integer.parseInt(args[3].toString()) : 0;
				int priority = (args.length>=5) ? Integer.parseInt(args[4].toString()) : 0;
				
				try
				{
					JSONObject context = new JSONObject(args[1].toString());
					
					SendCommand[] sendCommands = SendCommandRepository.getInstance().get(sendType,context,getAll);
					
					if(sendCommands != null && sendCommands.length > 0)
					{
						if(args.length >= 6 && args[5] != null)
						{
							try
							{
								JSONObject filterArgs = new JSONObject(args[5].toString());
								SendContextFilter filter = new SendContextFilter(filterArgs);
								for (int i = 0; i < sendCommands.length; i++)
								{
									sendCommands[i].setSendCommandContextFilter(filter);
								}
							} catch (JSONException e) {
								System.out.println(e.toString());
							}
						}
						SendCommandMenu sendCommandmenu = new SendCommandMenu(sendCommands, 0, 0);
						SendCommandMenuItem[] menuItems = sendCommandmenu.getSendCommandMenuItems();
						menu = new SubMenu(menuItems,menuLabel,ordering,priority);
						screen.addMenuItem(menu.getMenuItem());
						result = Boolean.TRUE;
					}
				}
				catch (JSONException e)
				{
					System.out.println(e.toString());
				}
			} 

			return result;
		}
		
	}

	/*
	 * Used in SendMenuAdd if a context filter is defined
	 */
	public class SendContextFilter implements SendCommandContextFilter
	{
		private JSONObject filter = null;
		
		public SendContextFilter(JSONObject filterArgs)
		{
			filter = filterArgs;
		}

		public JSONObject filterContext(SendCommand sendCommand)
		{
			JSONObject newContext = sendCommand.getContext();
			try
			{
				Enumeration keys = filter.keys();
				while( keys.hasMoreElements() ){
					String key = (String)keys.nextElement();
					if( filter.get(key) instanceof String ){
						newContext.put(key, filter.get(key));
					}
				}
			}
			catch (JSONException e)
			{
				System.out.println(e.toString());
			}

			return newContext;
		}
	}

	/**
	 * Removes the SendMenu, if it exists
	 *
	 * No arguments required
	 */
	public class SendMenuRemoveFunction extends ScriptableFunction {

		public Object invoke(Object obj, Object[] args) throws Exception 
		{
			if (menu != null)
			{
				MainScreen screen = (MainScreen) SendMenuExtension.getBrowserField().getScreen();
				screen.removeMenuItem(menu.getMenuItem());
				menu = null;
				return Boolean.TRUE;
			}
			
			return Boolean.FALSE;
		}
	}

	/**
	 * Sets the label for the Send Menu in the parent menu
	 * (opens to sub menu with send commands)
	 *
	 * Arguments:
	 * 0: String for label
	 */
	public class SetLabelFunction extends ScriptableFunction {

		public Object invoke(Object obj, Object[] args) throws Exception
		{
			if (args.length == 1)
			{
				menuLabel = args[0].toString();
			}				
			return UNDEFINED;
		}

	}
	
    /**
     * @see net.rim.device.api.script.Scriptable#getField(String)
     */
	public Object getField(String name) throws Exception 
	{
		if(_fields.containsKey( name ))
			return _fields.get( name );
			
		return UNDEFINED;
	}

}