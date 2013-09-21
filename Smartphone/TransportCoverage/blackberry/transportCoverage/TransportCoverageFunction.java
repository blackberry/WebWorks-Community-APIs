
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
package blackberry.transportCoverage;

import net.rim.device.api.io.transport.TransportInfo;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;

public final class TransportCoverageFunction extends ScriptableFunction {
	
	public static final String NAME = "getTransportDetails";
	
	
	
	public Object invoke(Object obj, Object[] args) throws Exception
	{
		String transport = args[0].toString().toUpperCase(); 	//transport we are testing
		TransportDetailsObject t = new TransportDetailsObject();
		if (transport.equals("TRANSPORT_MDS")){			
			t.transport = transport;
			t.available = new Boolean(TransportInfo.isTransportTypeAvailable(TransportInfo.TRANSPORT_MDS));
			t.coverage = new Boolean(TransportInfo.hasSufficientCoverage(TransportInfo.TRANSPORT_MDS));	
		}else if (transport.equals("TRANSPORT_BIS_B")){			
			t.transport = transport;
			t.available = new Boolean(TransportInfo.isTransportTypeAvailable(TransportInfo.TRANSPORT_BIS_B));
			t.coverage = new Boolean(TransportInfo.hasSufficientCoverage(TransportInfo.TRANSPORT_BIS_B));	
		}else if (transport.equals("TRANSPORT_TCP_CELLULAR")){			
			t.transport = transport;
			t.available = new Boolean(TransportInfo.isTransportTypeAvailable(TransportInfo.TRANSPORT_TCP_CELLULAR));
			t.coverage = new Boolean(TransportInfo.hasSufficientCoverage(TransportInfo.TRANSPORT_TCP_CELLULAR));	
		}else if (transport.equals("TRANSPORT_TCP_WIFI")){			
			t.transport = transport;
			t.available = new Boolean(TransportInfo.isTransportTypeAvailable(TransportInfo.TRANSPORT_TCP_WIFI));
			t.coverage = new Boolean(TransportInfo.hasSufficientCoverage(TransportInfo.TRANSPORT_TCP_WIFI));	
		}else if (transport.equals("TRANSPORT_WAP")){			
			t.transport = transport;
			t.available = new Boolean(TransportInfo.isTransportTypeAvailable(TransportInfo.TRANSPORT_WAP));
			t.coverage = new Boolean(TransportInfo.hasSufficientCoverage(TransportInfo.TRANSPORT_WAP));	
		}else if (transport.equals("TRANSPORT_WAP2")){			
			t.transport = transport;
			t.available = new Boolean(TransportInfo.isTransportTypeAvailable(TransportInfo.TRANSPORT_WAP2));
			t.coverage = new Boolean(TransportInfo.hasSufficientCoverage(TransportInfo.TRANSPORT_WAP2));	
		}
		
		return t;		
	}
}

