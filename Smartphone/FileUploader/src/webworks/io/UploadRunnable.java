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

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.io.HttpsConnection;
import javax.microedition.io.file.FileConnection;

import net.rim.device.api.io.IOUtilities;
import net.rim.device.api.io.MIMETypeAssociations;
import net.rim.device.api.io.http.HttpProtocolConstants;
import net.rim.device.api.io.transport.ConnectionDescriptor;
import net.rim.device.api.io.transport.ConnectionFactory;
import net.rim.device.api.io.transport.TransportInfo;
import net.rim.device.api.io.transport.options.BisBOptions;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.DeviceInfo;

public class UploadRunnable implements Runnable
{
	private static final String BOUNDARY = "----0x6d6356418a05040fL";
	private static final String TD = "--";
	private static final String DCRLF = HttpProtocolConstants.HTTP_HEADER_DOUBLE_CRLF;
	private static final String CRLF = HttpProtocolConstants.HTTP_HEADER_SINGLE_CRLF;
	
	private String _url;
	private Hashtable _params;
	private String _fileKey;
	private String _filePath;
	private String _mimeType;
	private Hashtable _headers;
	
	private ScriptableFunction _success;
	private ScriptableFunction _error;
	
	private ConnectionFactory _factory;
	
	private int[] _transports = new int[] {
		TransportInfo.TRANSPORT_TCP_WIFI,
		TransportInfo.TRANSPORT_BIS_B,
		TransportInfo.TRANSPORT_MDS,
		TransportInfo.TRANSPORT_TCP_CELLULAR,
		TransportInfo.TRANSPORT_WAP2,
		TransportInfo.TRANSPORT_WAP
	};
	
	public UploadRunnable(String url, String fileKey, Hashtable params, String filePath, String mimeType, long timeout, Hashtable headers, int[] transports, ScriptableFunction success, ScriptableFunction error)
	{
		_url = url;
		_fileKey = fileKey;
		_filePath = filePath;
		_params = params;
		_headers = headers;
		
		_mimeType = mimeType;
		
		if (transports != null) {
			_transports = transports;
		}
		_success = success;
		_error = error;
		
		_factory = new ConnectionFactory();
		
		if (DeviceInfo.isSimulator()) {
			_factory.setPreferredTransportTypes(new int[] { TransportInfo.TRANSPORT_TCP_WIFI, TransportInfo.TRANSPORT_TCP_CELLULAR });
		}
		
		_factory.setPreferredTransportTypes(_transports);
		_factory.setAttemptsLimit(100);
		_factory.setTimeoutSupported(true);
		_factory.setConnectionTimeout(timeout);
		
		new Thread(this).start();
	}

	public void run() {
		FileConnection fc = null;
		HttpConnection hc = null;
		
		InputStream is = null;
		OutputStream os = null;
		
		boolean isHTTPS = false;
		
		try {
			
			try {
                fc = (FileConnection)Connector.open(_filePath, Connector.READ);
            } catch (Exception e) {
            	Logger.error("Invalid file path");
                callErrorCallback(new String[] {"Invalid file path"});
                return;
            }
            
            Logger.log("Setting mime type...");
            
			if (_mimeType == null) {
                _mimeType = MIMETypeAssociations.getMIMEType(_filePath);
                if (_mimeType == null) {
                    _mimeType = HttpProtocolConstants.CONTENT_TYPE_IMAGE_JPEG;
                }          
            }
            
            if (!fc.exists()) {
            	Logger.error("File not found");
            	callErrorCallback(new String[] { _filePath + " not found" });
            } 
            
			if (_url.indexOf("https") == 0) {
				isHTTPS = true;
				Logger.error("Setting End to End");
				_factory.setEndToEndDesired(isHTTPS);
			}
            
			ConnectionDescriptor connDesc = _factory.getConnection(_url);
			
			if (connDesc != null) {
				Logger.info("URL: " + connDesc.getUrl());
				try {
					if (isHTTPS) {
						hc = (HttpsConnection) connDesc.getConnection();
					} else {
						hc = (HttpConnection) connDesc.getConnection();
					}
					
					String startBoundary = getStartBoundary(_fileKey, fc.getName(), _mimeType);
					String endBoundary = getEndBoundary();
					
					String params = (_params != null) ? getParameters(_params) : "";
					
					long fileSize = fc.fileSize();
					long contentLength = fileSize + 
	                (long)startBoundary.length() + 
	                (long)endBoundary.length() + 
	                (long)params.length();
				
				
					hc.setRequestMethod(HttpConnection.POST);
					
					if (_headers != null) {
						String hKey;
						String hVal;
						for (Enumeration e = _headers.keys(); e.hasMoreElements();) {
							hKey = e.nextElement().toString();
				            hVal = (String) _headers.get(hKey);
				            
				            Logger.error(hKey +": " + hVal);

							hc.setRequestProperty(hKey, hVal);
						}
					}
					
					hc.setRequestProperty(
		                    HttpProtocolConstants.HEADER_USER_AGENT, 
		                    System.getProperty("browser.useragent"));
					hc.setRequestProperty(
		                    HttpProtocolConstants.HEADER_KEEP_ALIVE, "300");
					hc.setRequestProperty(
		                    HttpProtocolConstants.HEADER_CONNECTION, "keep-alive");
					hc.setRequestProperty(
		                    HttpProtocolConstants.HEADER_CONTENT_TYPE, 
		                    HttpProtocolConstants.CONTENT_TYPE_MULTIPART_FORM_DATA + "; boundary=" + BOUNDARY);
					hc.setRequestProperty(
		                    HttpProtocolConstants.HEADER_CONTENT_LENGTH, 
		                    Long.toString(contentLength));
					
					
					os = hc.openDataOutputStream();
					
					os.write(params.getBytes());
					os.write(startBoundary.getBytes());
					
					is = fc.openInputStream();
		            byte[] data = IOUtilities.streamToBytes(is);
		            os.write(data);
		            is.close();
		            
		            os.write(endBoundary.getBytes());
		            os.flush();
		            os.close();
		            
		            is = hc.openDataInputStream(); 
		            int responseCode = hc.getResponseCode();
		            
					if (responseCode != HttpConnection.HTTP_OK) {
						Logger.error("Response code: " +responseCode);
						callErrorCallback(new Object[] { "Server Error", new Integer(responseCode) });
					} else {
						callSuccessCallback(new Object[]{ new String(IOUtilities.streamToBytes(is)) });
					}
				} catch (Throwable e) {
					callErrorCallback(new String[] { e.getMessage() });
					e.printStackTrace();
				}
			} else {
				Logger.error("Error creating HTTP connection");
				callErrorCallback(new String[] { "Error creating HTTP connection." });
			}
		        
		} finally {
			 try {
				if (fc != null) fc.close();
				if (os != null) os.close();
				if (is != null) is.close();
				if (hc != null) hc.close();
			 } catch (Exception e) {
			 }
		}
	}
	
	protected String getStartBoundary(String fileKey, String fileName, String mimeType) {
        return new StringBuffer()
            .append(CRLF)
            .append(TD).append(BOUNDARY)
            .append(CRLF)
            .append("Content-Disposition: form-data; name=\"").append(fileKey)
            .append("\"; filename=\"").append(fileName).append("\"")
            .append(CRLF)
            .append(HttpProtocolConstants.HEADER_CONTENT_TYPE).append(mimeType)
            .append(DCRLF)
            .toString();
    }
	
	protected String getEndBoundary() {
        return CRLF + TD + BOUNDARY + TD + CRLF;        
    }
	
	protected String getParameters(Hashtable params) {
        StringBuffer buf = new StringBuffer();
        
        Enumeration e = params.keys();
        String key;
        String value;
        
        while (e.hasMoreElements()) {
            key = e.nextElement().toString();
            value = (String) params.get(key);
            buf.append(TD).append(BOUNDARY).append(CRLF)
                .append("Content-Disposition: form-data; name=\"").append(key).append("\"")
                .append(CRLF).append(CRLF)
                .append(value).append(CRLF);
        }
        return buf.toString();
    }
	
	
	private ScriptableFunction callSuccessCallback(Object[] args)
	{
		if (_success != null) {
			try {
				_success.invoke(null, args);
			} catch(Exception e) {
			}
		}
		return null;
	}
	
	private ScriptableFunction callErrorCallback(Object[] args)
	{
		if (_error != null) {
			try {
				_error.invoke(null, args);
			} catch(Exception e) {
			}
		}
		return null;
	}
	
}