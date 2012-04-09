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

package webworks.pim.appointment.namespace;

import webworks.pim.appointment.functions.FunctionStartListening;
import webworks.pim.appointment.functions.FunctionStopListening;
import webworks.pim.appointment.impl.AptListenerManager;
import net.rim.device.api.script.Scriptable;

public class AptListenerNamespace extends Scriptable  {

	
public static final String NAME = "webworks.pim.appointment";
	
	private FunctionStartListening _funcStartListening;
	private FunctionStopListening _funcStopListening;
	private AptListenerManager _manager;
	
	public AptListenerNamespace(AptListenerManager aptListenerManager) {
		
		_manager = aptListenerManager;
		
		_funcStartListening = new FunctionStartListening(aptListenerManager);
		_funcStopListening = new FunctionStopListening(aptListenerManager);
	}
	
	public Object getField(String name) throws Exception {
		
		if(FunctionStartListening.NAME.equals(name)) {
			return _funcStartListening;
		} else if(FunctionStopListening.NAME.equals(name)) {
			return _funcStopListening;
		} else {
			return super.getField(name);
		}
	}
		
}
