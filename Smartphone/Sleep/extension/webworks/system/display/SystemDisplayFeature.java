/*
* Copyright 2010-2011 Research In Motion Limited.
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

package webworks.system.display;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.system.Display;

import java.lang.Boolean;
import java.util.Timer;
import java.util.TimerTask;
import net.rim.device.api.system.Backlight;

/**
 * Controlling the Backlight in BlackBerry Device Software invovles extending the security timer.
 * This is accomplished by calling Backlight.enable(true, INTERVAL).  In order to support a persistent
 * backlight 'on' experience, a Timer is used to repeat the call made to enable() every INTERVAL seconds.
 *
 */
public final class SystemDisplayFeature extends Scriptable 
{
	private String	PROPERTY_ALLOW_SLEEP = "allowSleep";
	
	private int MAX_BACKLIGHT_TIMEOUT = 255;		//The Backlight.setTimeout class allows a max interval of 255 s

	private Timer t;
	private boolean isTimerActive;

	
	/**
	 *  Constructor
	 */	
	public SystemDisplayFeature()
	{
		this.t = new Timer();
		isTimerActive = false;
	}

	
	/**
	 *  getField - Called when WebWorks application tries to get the value of a property
	 */	
	public Object getField(String name) throws Exception 
	{
		try
		{
			if (name.equals(PROPERTY_ALLOW_SLEEP)) 
			{
				// WebWorks application has requested to read the property 'allowSleep'	
				// return true if and only if the BacklightTask is NOT currently running.
				
				return new Boolean(isTimerActive == false);		//must return an object, not a value.
			}
		}
		catch (Exception e) 
		{
			System.out.println("Exception in SystemDisplayFeature.getField: " + e.toString());
		}
		
		return super.getField(name);
	}


	/**
	 *  putField - Called when WebWorks application tries to change the value of a property
	 */	
	public boolean putField(String name, Object value) throws Exception 
	{
		try 
		{
			
			if (name.equals(PROPERTY_ALLOW_SLEEP)) 
			{
				boolean allowSleep = (value.toString() == "true");
					
				if (allowSleep)
				{
					// WebWorks application has requested to set the property 'allowSleep' to 'false'
					// ==> Stop the timer that is currently calling Backlight.enable()
					
					if (isTimerActive == true)
					{
						isTimerActive = false;
						t.cancel();
						Backlight.setTimeout(Backlight.getTimeoutDefault());  		//restore the backlight timeout default value (30 seconds)
					}
				}
				else 
				{
					// WebWorks application has requested to set the property 'allowSleep' to 'true'
					// ==> Start a timer that will call Backlight.enable() every 255 seconds. 
					
					if (isTimerActive == false)
					{
						isTimerActive = true;
						BacklightTask task = new BacklightTask();
						t.scheduleAtFixedRate(task, 0, MAX_BACKLIGHT_TIMEOUT * 1000);		//Start the timer task immediately, and have it run every 255 seconds.
					}
				}

				return true;
			}
		} 
		catch (Exception e) 
		{
			System.out.println("Exception in SystemDisplayFeature.putField: " + e.toString());
		}
		
		return false;
	}

	
	
	//The purpose of this timer task is to reset the display backlight every 255 seconds.
	protected class BacklightTask extends TimerTask {
	
		/**
		 * Execute the Timer's Task
		 */
		public void run() 
		{
			try 
			{
				//http://supportforums.blackberry.com/t5/Java-Development/Enable-the-backlight-and-prevent-the-BlackBerry-device-screen/ta-p/444706

				//The security timer cannot be directly reset programmatically.  Instead, this action is performed via the Backlight.enable() API.
				//If the application is not permitted to modify the security timer, then Backlight.enable() will silently skip this step.
				//No indication is given to the calling application as to whether a security timer reset was performed.

				//Two conditions must be satisfied for an application to be able to reset the security timer:
				 
				//	1) If an IT Policy is present, the ALLOW_RESETTING_OF_IDLE_TIMER policy in the Security group must be set to True.  
				//		(This permits Blackberry Enterprise Server administrators to ban security timer resets entirely.)  
				//		This policy defaults to False.
				 
				//	2) The calling application must hold the PERMISSION_IDLE_TIMER permission (also called the “Security Timer Reset” permission).  
				//		This permission defaults to Deny.
				
				if (( Display.getProperties() & Display.DISPLAY_PROPERTY_REQUIRES_BACKLIGHT) != 0 )
				{
					Backlight.enable( true, MAX_BACKLIGHT_TIMEOUT );
				}
				
			} 
			catch (Exception e) 
			{
				System.out.println("Exception in BacklightTask.run: " + e.toString());
//				EventLogger.logEvent(EVENT_LOG_GUID, "Exception in BacklightTask.run: " + e.toString(), EventLogger.ERROR);
			}
		}
	
	}	

	
}