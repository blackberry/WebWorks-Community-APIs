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

package webworks.io;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.io.file.FileConnection;

import net.rim.device.api.io.IOUtilities;
import net.rim.device.api.io.MIMETypeAssociations;
import net.rim.device.api.io.http.HttpProtocolConstants;
import net.rim.device.api.io.transport.ConnectionDescriptor;
import net.rim.device.api.io.transport.ConnectionFactory;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;

public class FunctionUpload extends ScriptableFunction {
	
	public static final String NAME = "upload";
	
	private ScriptableFunction _successCallback = null;
	private ScriptableFunction _errorCallback = null;

	public Object invoke(Object thiz, Object[] args) throws Exception {
		
		if (args.length == 1) {
			Hashtable headers = null;
			Hashtable params = null;
			
			Logger.info("Getting options...");
			
			Scriptable config = (Scriptable) args[0];
			
			Logger.info("Setting core options...");
			
			String url = (String) config.getField("url");
			String filePath = (String) config.getField("file");
			String fileKey = (String) config.getField("fileKey");
			String mimeType = (String) config.getField("mimeType");
			
			Logger.info("Getting headers and params...");
			
			Object sHeaders = config.getField("headers");
			Object sParams = config.getField("params");
			
			if (mimeType.equals(UNDEFINED)) {
				mimeType = null;
			}
			
			Logger.info("Setting headers and extra params...");
			
			try {
				headers = objectToHashtable(sHeaders);
				params = objectToHashtable(sParams);
			} catch(Throwable e) {
				Logger.error("Error setting custom params and headers..." + e.getMessage());
			}
			
			_successCallback = (ScriptableFunction) config.getField("success");
			_errorCallback = (ScriptableFunction) config.getField("error");
			
			Logger.info("Beginning upload...");
			
			new UploadRunnable(url, fileKey, params, filePath, mimeType, headers, _successCallback, _errorCallback);
		}
		return UNDEFINED;
	}
	
	private Hashtable objectToHashtable(Object obj) throws Exception
	{
		Hashtable ht = null;
		
		if (!obj.equals(UNDEFINED) && obj != null) {
			Scriptable s = (Scriptable) obj;

			Vector v = new Vector();
			s.enumerateFields(v);
			
			int count = v.size();
			Logger.log("Object Keys: " + String.valueOf(v.size()));
			
			if (count > 0) {
				ht = new Hashtable();
				String fieldName;
				
				for (int i=0; i<count; i++) {
					fieldName = (String) v.elementAt(i);
					ht.put(fieldName, s.getField(fieldName));
				}
			}
		}
		return ht;
	}
	
}
