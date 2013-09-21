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

package webworks.message.email.namespace;

import webworks.message.email.functions.FunctionStartListening;
import webworks.message.email.functions.FunctionStopListening;
import webworks.message.email.impl.EmailListenerManager;
import net.rim.device.api.script.Scriptable;

public class EmailListenerNamespace extends Scriptable {

	public static final String NAME = "webworks.message.email";
	
	private FunctionStartListening _funcStartListening;
	private FunctionStopListening _funcStopListening;
	private EmailListenerManager _manager;
	
	public EmailListenerNamespace(EmailListenerManager emailListenerManager) {
		
		_manager = emailListenerManager;
		
		_funcStartListening = new FunctionStartListening(emailListenerManager);
		_funcStopListening = new FunctionStopListening(emailListenerManager);
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
	
//	public boolean putField(String name, Object value) throws Exception {
//		boolean handled = true;
//		
//		if(FIELD_DELETE_EMAIL.equals(name)) {
//			setShouldDelete(value);
//		} else if(FIELD_FORUM_ADDRESS.equals(name)) {
//			setForumAddress(value);
//		} else {
//			handled = false;
//		}
//		
//		return handled;
//	}

//	private void setForumAddress(Object value) {
//		if(value instanceof String) {
//			_manager.setForumAddress((String)value);
//		}
//	}
//
//	private void setShouldDelete(Object value) {
//		if(value instanceof Boolean) {
//			_manager.setShouldDelete((Boolean)value);
//		}
//	}

}
