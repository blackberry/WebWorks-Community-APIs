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

import java.util.Vector;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.web.WidgetConfig;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

public class FileDownloaderNamespace extends Scriptable {
	
	public static final String NAME = "webworks.io.FileDownloader";
	
	private static final String FIELD_DOWNLOAD = FunctionDownload.NAME;
	
	private FunctionDownload _downloadFunction;
	
	private static String transports[] =  new String[] {
		null,
		"TCP_CELLULAR",
		"WAP",
		"WAP2",
		"MDS",
		"BIS-B",
		"TCP_WIFI"
	};
	
	public FileDownloaderNamespace(WidgetConfig wc) {
		_downloadFunction = new FunctionDownload(getTransports(wc.getConfigXML()));
	}
	
	private int[] getTransports(Document d)
	{
		int[] ret = null;
		
		String connection = "rim:connection";
		NodeList connections = d.getElementsByTagName(connection);
		NodeList transports = d.getElementsByTagName("id");
		
		int connectionCount = connections.getLength();
		int transportCount = transports.getLength();
		
		if (connectionCount > 0) {
			Vector v = new Vector();
			
			if (transportCount > 0) {
				
				for (int i=0; i< transportCount; i++) {
					if (transports.item(i).getParentNode().getNodeName().equals(connection)) {
						v.addElement(transports.item(i).getFirstChild().getNodeValue());
					}
				}
				
				ret = new int[v.size()];
				for (int j=0; j<v.size(); j++) {
					ret[j] = stringToTransport((String) v.elementAt(j));
				}
			}
		}
		
		return ret;
	}

    private int stringToTransport(String transport) {
    	
    	int transportCount = transports.length;
    	
    	for (int i=transportCount-1; i >= 0; i--) {
    		if (transports[i].equals(transport)) {
    			return i;
    		}
    	}
    	
		return -1;
	}

	public Object getField(String name) throws Exception {
        if (name.equals(FIELD_DOWNLOAD)) {
            return _downloadFunction;
        }
		return super.getField(name);
    }
	
}
