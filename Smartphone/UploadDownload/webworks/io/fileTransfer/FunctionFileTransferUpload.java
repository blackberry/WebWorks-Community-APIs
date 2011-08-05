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

package io.fileTransfer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.io.file.FileConnection;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Application;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.JPEGEncodedImage;

/**
 * @author kwallis
 *
 */
public class FunctionFileTransferUpload extends ScriptableFunction {

	public static final String NAME = "upload";
	private ScriptableFunction _onProgress;
	private ScriptableFunction _onError;
	private Scriptable _options;
	private final Application _application = Application.getApplication();
	

	public Object invoke(Object thiz, Object[] args) {

		final String remotePath = args[0].toString();
		final String localPath = args[1].toString();
		_onProgress = (ScriptableFunction) args[2];
		_onError = (ScriptableFunction) args[3];
		_options = (Scriptable)args[4];
		
		if (_onProgress != null) {
			_application.invokeLater(new Runnable() {
				public void run() {
					try {
						new Thread(new Runnable() {
							public void run() {
								
								Object objToPassWithInvoke = null;

								try {
									byte[] b = readFile(localPath);
									objToPassWithInvoke = postViaHttpConnection(remotePath, b);

								} catch (IOException e) {
									objToPassWithInvoke = e.getMessage();
									// e.printStackTrace();
								}
								try {
									if (objToPassWithInvoke != null) {
										invokeCallback(objToPassWithInvoke);
									}
								} catch (final Exception e) {

								} finally {

								}
							}
						}).start();
					} catch (final Exception e) {
					}
				}
			});
		}

		return Boolean.TRUE;
	}

	private String postViaHttpConnection(String url, byte[] b)
			throws IOException {
		HttpConnection c = null;
		InputStream is = null;
		OutputStream os = null;
		int rc;

		try {
			c = (HttpConnection) Connector.open(url);

			// Set the request method and headers
			c.setRequestMethod(HttpConnection.POST);

			c.setRequestProperty("If-Modified-Since",
					"29 Oct 1999 19:43:31 GMT");
			c.setRequestProperty("User-Agent",
					"Profile/MIDP-2.0 Configuration/CLDC-1.0");
			
			c.setRequestProperty("Content-Language", "en-US");

			// Getting the output stream may flush the headers
			os = c.openOutputStream();
			os.write(b);
			os.flush(); // Optional, getResponseCode will flush

			// Getting the response code will open the connection,
			// send the request, and read the HTTP response headers.
			// The headers are stored until requested.
			rc = c.getResponseCode();

			if (rc != HttpConnection.HTTP_OK) {
				throw new IOException("HTTP response code: " + rc);
			}

			is = c.openInputStream();

			// Get the ContentType
			String type = c.getType();
			// processType(type);

			// Get the length and process the data
			int len = (int) c.getLength();
			if (len > 0) {
				int actual = 0;
				int bytesread = 0;
				byte[] data = new byte[len];
				while ((bytesread != len) && (actual != -1)) {
					actual = is.read(data, bytesread, len - bytesread);
					bytesread += actual;
				}
				String s = "";
				for (int i = 0; i < data.length; i++) {
					s = s + (char) data[i];
				}
				return s;
			}
			// else {
			// int ch;
			// while ((ch = is.read()) != -1) {
			// process((byte)ch);
			// }
			// }

		} catch (ClassCastException e) {
			throw new IllegalArgumentException("Not an HTTP URL");
		} finally {
			if (is != null)
				is.close();
			if (os != null)
				os.close();
			if (c != null)
				c.close();
		}

		return "Resp";//
	}
	
	private byte[] readFile(String path) {
		//scale file
		try {
			FileConnection fconn = (FileConnection) Connector.open(path);
			if (!fconn.exists()) {
				System.out.println("The file do not exist!");
			}
			InputStream input = fconn.openInputStream();
			int length = (int) fconn.fileSize();// totalSize();

			// allocating a temporary Buffer.
			byte[] b = new byte[length];

			// reading data from a InputStream
			input.read(b, 0, length);

			fconn.close();
			return b;
		} catch (Exception ioe) {
			System.out.println(ioe.getMessage());
		}
		
		return new byte[0];
	}

	private void invokeCallback(Object objToPassWithInvoke) throws Exception {

		_onProgress.invoke(null, new Object[] { objToPassWithInvoke });
	}
}
