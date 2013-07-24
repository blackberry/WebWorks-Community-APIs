/*
* Copyright 2010 Research In Motion Limited.
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
package blackberry.ui.spinner;

import net.rim.device.api.system.DeviceInfo;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Keypad;
import net.rim.device.api.ui.Screen;
import net.rim.device.api.ui.TouchEvent;
import net.rim.device.api.ui.TouchGesture;
import net.rim.device.api.ui.TransitionContext;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.UiEngineInstance;
import net.rim.device.api.ui.VirtualKeyboard;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.component.SeparatorField;
import net.rim.device.api.ui.component.TextSpinBoxField;
import net.rim.device.api.ui.container.HorizontalFieldManager;
import net.rim.device.api.ui.container.PopupScreen;
import net.rim.device.api.ui.container.VerticalFieldManager;

public final class SpinnerScreen extends PopupScreen     
{
	private TextSpinBoxField _spinBoxField;
	private boolean _isSet;        
	
	/**
	 * Creates a new CustomSpinnerPopup object
	 */
	public SpinnerScreen(String title, String[] choices, int visibleRows, int rowHeight, int selectedIndex)
	{           
		super(new VerticalFieldManager(), Screen.DEFAULT_CLOSE);   
	   
		_spinBoxField = new CustomTextSpinBoxField(choices);
		_spinBoxField.setVisibleRows(visibleRows);
		_spinBoxField.setRowHeight(rowHeight);
		if (selectedIndex < choices.length && selectedIndex > 0)
			_spinBoxField.setSelectedIndex(selectedIndex);
		else
			_spinBoxField.setSelectedIndex(0);
		add(new LabelField(title));
		add(new SeparatorField());
		HorizontalFieldManager hfm = new HorizontalFieldManager(Field.FIELD_HCENTER);
		hfm.add(_spinBoxField);
		add(hfm);     
		// Turn off the virtual keyboard
		final VirtualKeyboard virtualKeyboard = this.getVirtualKeyboard();
		if (virtualKeyboard != null) {
			virtualKeyboard.setVisibility(VirtualKeyboard.HIDE);
		}
	}
	
	
	/**
	 * Retrieves the currently selected choice
	 * @return The currently selected choice
	 */
	public int getChoice()
	{
		return _spinBoxField.getSelectedIndex();
	}
	
	
	/**
	 * Indicates whether the TextSpinBoxField has changed from
	 * its initial state.
	 * @return True if the selected choice has been modified, otherwise false
	 */
	public boolean isSet()
	{
		return _isSet;
	}        
	
	
	/**
	 * @see Screen#touchEvent(TouchEvent)
	 */
	protected boolean touchEvent(TouchEvent message)
	{
		if(message.getEvent() == TouchEvent.GESTURE)
		{
			TouchGesture touchGesture = message.getGesture(); 
			if(touchGesture.getEvent() == TouchGesture.TAP)
			{    
				String osVersion = DeviceInfo.getSoftwareVersion();
				if (osVersion.startsWith("5.0")) {
					return super.touchEvent(message);
				}
				else {              		
					_isSet = true;                                    
					close();
					return true;
				}
			}
		}
		return super.touchEvent(message);
	}     
	
	/**
	 * @see Screen#invokeAction(int)
	 */
	protected boolean invokeAction(int action)
	{
		if(action == ACTION_INVOKE)
		{                                   
			_isSet = true;                                
			close();
			return true;
		}          
		return super.invokeAction(action); 
	}    
	
	
	/**
	 * @see Screen#close()
	 */
	public void close()
	{
		if(!_isSet)
		{                
			_spinBoxField.setSelectedIndex(0);
		}
		super.close();
	}  
	
	/**
	 * @see Screen#keyChar(char, int, int)  
	 */
	protected boolean keyChar(char key, int status, int time)
	{      
		if(key == Keypad.KEY_ENTER)
		{
			_isSet = true;                                
			close();
			return true;
		}            
		return super.keyChar(key, status, time);            
	} 
}




