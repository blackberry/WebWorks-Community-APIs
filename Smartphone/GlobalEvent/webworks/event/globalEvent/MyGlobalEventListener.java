
/*
* Copyright 2010-2012 Research In Motion Limited.
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

package webworks.event.globalEvent;

import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.GlobalEventListener;
import net.rim.device.api.util.StringUtilities;

public class MyGlobalEventListener implements GlobalEventListener{
	private String _myKey;
	private long _myGuid;
	private ScriptableFunction _callback;
	
	public MyGlobalEventListener(String myKey, ScriptableFunction callback){
		_myKey = myKey;
		_myGuid = StringUtilities.stringHashToLong(myKey);
		_callback = callback;
	}
	
	public void eventOccurred(long guid, int data0, int data1, Object object0,
			Object object1) {
		System.out.println("Event Occured.");
		System.out.println("guid: " + Long.toString(guid));
		
		if (guid == _myGuid){
			System.out.println("GUID MATCH!");
			
			// Create a final thread safe result to pass into the thread object
			final Object[] threadedResult = new Object[] {_myKey};

			// Create a new thread to make sure that the invoke of the JavaScript callback
			// does not initiate from the UI thread.  This can otherwise cause a deadlock scenario
			new Thread () {
				public void run() {
					try
					{
						// Pass the result of the picker back to the handle of the JavaScript callback
						_callback.invoke(_callback, threadedResult);
					}
					catch (Exception e) {
						throw new RuntimeException(e.getMessage());
					}
				}
			}.start();
		}
	}

}
