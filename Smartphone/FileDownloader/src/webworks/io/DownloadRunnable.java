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

import net.rim.blackberry.api.browser.URLEncodedPostData;
import net.rim.device.api.io.IOUtilities;
import net.rim.device.api.io.http.HttpProtocolConstants;
import net.rim.device.api.io.transport.ConnectionDescriptor;
import net.rim.device.api.io.transport.ConnectionFactory;
import net.rim.device.api.io.transport.TransportInfo;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.DeviceInfo;

public class DownloadRunnable implements Runnable
{
	private static final String BOUNDARY = "----0x6d6356418a05040fL";
	private static final String TD = "--";
	private static final String DCRLF = HttpProtocolConstants.HTTP_HEADER_DOUBLE_CRLF;
	private static final String CRLF = HttpProtocolConstants.HTTP_HEADER_SINGLE_CRLF;
	
	private String _url;
	private Hashtable _params;
	private String _filePath;
	private String _method = HttpConnection.GET;
	private Hashtable _headers;
	
	private ScriptableFunction _success;
	private ScriptableFunction _error;
	
	private ConnectionFactory _factory;
	
	private static String FILE_SEPARATOR = System.getProperty("file.separator");
	private static String PARENT_DIRECTORY = "..";
	private static String FILE_PROTOCOL = "file://";
	
	private int[] _transports = new int[] {
		TransportInfo.TRANSPORT_TCP_WIFI,
		TransportInfo.TRANSPORT_BIS_B,
		TransportInfo.TRANSPORT_MDS,
		TransportInfo.TRANSPORT_TCP_CELLULAR,
		TransportInfo.TRANSPORT_WAP2,
		TransportInfo.TRANSPORT_WAP
	};
	
	public DownloadRunnable(String url, String method, Hashtable params, String filePath, long timeout, Hashtable headers, int[] transports, ScriptableFunction success, ScriptableFunction error)
	{
		_url = url;
		_filePath = filePath;
		_params = params;
		_headers = headers;
		
		if (transports != null) {
			_transports = transports;
		}
		
		if (method != null) {
			_method = method;
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
	
	public static void createFolders(String path) throws Exception
	{
		String delimiter = FILE_SEPARATOR;
		String prefix = FILE_PROTOCOL + delimiter;
		int prefixIdx = path.indexOf(prefix);
		
		if (prefixIdx == 0) {
			String newPath = path.substring(prefix.length(), path.lastIndexOf('/')) + delimiter;
			Logger.log(newPath);
			int pathIdx = newPath.lastIndexOf('/');
			
			if (pathIdx == -1) {
				return; // Folder creation not necessary
			}
			
			String[] folders = StringUtils.split(newPath, delimiter);
			
			int folderCount = folders.length;
			
			Logger.log(String.valueOf(folderCount));
			
			if (folderCount > 0) {
				StringBuffer folderBuffer = new StringBuffer(prefix + folders[0] + delimiter);
				
				Logger.log(folderBuffer.toString());				

				for (int i=1; i < folderCount; i++) {
					folderBuffer.append(folders[i]);
					folderBuffer.append(delimiter);
					
					createFolder(folderBuffer.toString());
				}
			}
		} else {
			throw new Exception("file:/// prefix is missing");
		}
	}
	
	private static void createFolder(String path) throws Exception
	{
		FileConnection fc = (FileConnection) Connector.open(path, Connector.READ_WRITE);
		
		if (!fc.exists()) {
			fc.mkdir();
			fc.setWritable(true);
		}
		
		fc.close();
	}

	public void run() {
		FileConnection fc = null;
		HttpConnection hc = null;
		
		InputStream is = null;
		OutputStream os = null;
		
		boolean isHTTPS = false;
		boolean hasQuery = false;
		
		StringBuffer errorBuffer;
		
		try {
			
			try {
				createFolders(_filePath);
				
                fc = (FileConnection)Connector.open(_filePath, Connector.READ_WRITE);
                
                if (!fc.exists()) {
                	fc.create();
                }
            } catch (Exception e) {
            	errorBuffer = new StringBuffer("Unable to create file: ");
            	errorBuffer.append(e.getMessage());
            	
            	Logger.error(errorBuffer.toString());
                callErrorCallback(new String[] { errorBuffer.toString() });
                return;
            }
            
			if (_url.indexOf("https") == 0) {
				isHTTPS = true;
				Logger.error("Setting End to End");
				_factory.setEndToEndDesired(isHTTPS);
			}
			
			hasQuery = (_url.indexOf("?") > -1);
			
			String params = (_params != null) ? getParameters(_params) : "";	
			
			if (HttpConnection.GET == _method && params.length() > 0) { 
				if (hasQuery) {
					_url += "&" + params;
				} else {
					_url += "?" + params;
				}
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
				
					hc.setRequestMethod(_method);
					
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
					
					if (HttpConnection.POST == _method) {
						os = hc.openDataOutputStream();
						os.write(params.getBytes());
					}
		            
		            is = hc.openDataInputStream(); 
		            int responseCode = hc.getResponseCode();
		            
					if (responseCode != HttpConnection.HTTP_OK) {
						Logger.error("Response code: " +responseCode);
						callErrorCallback(new Object[] { "Server Error", new Integer(responseCode) });
					} else {
						byte[] file = IOUtilities.streamToBytes(is);
			            os = fc.openDataOutputStream();
			            os.write(file);
			            os.close();
						callSuccessCallback(new Object[]{ _filePath });
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
	
	protected String getEndBoundary() {
        return CRLF + TD + BOUNDARY + TD + CRLF;        
    }
	
	protected String getParameters(Hashtable params) {
        
        Enumeration e = params.keys();
        String key;
        String value;
        
        URLEncodedPostData data = new URLEncodedPostData(URLEncodedPostData.DEFAULT_CHARSET, false);
        
        while (e.hasMoreElements()) {
            key = e.nextElement().toString();
            value = (String) params.get(key);
           
            data.append(key, value);
        }
        return data.toString();
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