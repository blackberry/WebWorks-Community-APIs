/*
* Copyright 2010-2011 Research In Motion Limited.
*
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

package webworks.io.filetransfer
{
	import flash.events.Event;
	import flash.events.HTTPStatusEvent;
	import flash.events.IEventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.events.SecurityErrorEvent;
	import flash.filesystem.File;
	import flash.filesystem.FileMode;
	import flash.filesystem.FileStream;
	import flash.net.URLRequest;
	import flash.net.URLRequestHeader;
	import flash.net.URLStream;
	import flash.utils.ByteArray;
	
	import json.JSON;
	
	import org.httpclient.HttpClient;
	import org.httpclient.events.HttpResponseEvent;
	import org.httpclient.http.multipart.Multipart;
	import org.httpclient.http.multipart.Part;
	
	import webworks.extension.DefaultExtension;
	import webworks.extension.WebWorksReturnValue;
	import webworks.uri.URI;
	
	public class FileTransferExtension extends DefaultExtension
	{
		private var _ustream : URLStream;
		private var _jsOnProgress : String = "";
		private var _jsOnError : String = "";
		private var _localPath : String = "";
		private var _remotePath : String = "";
		private var _startTime : Date;
		private var _redirectURL : String = "";
		private var _mimeType : String = "";
		private var _progressInterval : Number;
		private var _successEvent : ProgressEvent;
		
		private var CONNECTION_TIMEOUT : Number = 30000;
		private var PROGRESS_INTERVAL : Number = -1; // non-zero means report progress whenever it's available, zero means report when file is completely done transfer
		private var MIME_TYPE_HEADER : String = "Content-Type";
		private var HTTP_OK : Number = 200;
		
		public function FileTransferExtension() {
			
		}
		
		public override function loadFeature(feature:String, version:String) : void {
			
		}
		
		public override function unloadFeature() : void {
			
		}
		
		public override function getFeatureList():Array {			
			return new Array("webworks.io.fileTransfer");
		}
		
		public function upload(remotePath : String, localPath : String, onProgressID : String, onErrorID : String, options : String) : Object {
			var result:WebWorksReturnValue;
			_remotePath = remotePath;
			_localPath = localPath;
			_jsOnProgress = onProgressID;
			_jsOnError = onErrorID;
			
			var data:Object = {
				"remotePath" : remotePath,
				"localPath" : localPath,
				"onProgressId" : onProgressID,
				"onErrorId" : onErrorID
			};
			
			result = new WebWorksReturnValue(data);
			
			var optionsObj:Object = ( options != "" && options != null ) ? (JSON.decode(options) as Object) : null;
			
			_progressInterval = PROGRESS_INTERVAL;
			var connectionTimeout:Number = CONNECTION_TIMEOUT;
			
			if (optionsObj != null) {
				if (optionsObj["connectionTimeout"] != null) {
					connectionTimeout = optionsObj["connectionTimeout"];
				}
				
				_progressInterval = optionsObj["progressInterval"];
			}
			
			var client:HttpClient = new HttpClient();
			client.timeout = connectionTimeout;
			
			var uri:URI = new URI(remotePath);
			var fileBytes:ByteArray = new ByteArray();
			var file:File = new File(localPath);
			
			if (file.exists) {
				var fstream:FileStream = new FileStream();
				
				fstream.open(file, FileMode.READ);
				fstream.readBytes(fileBytes);
				
				var multipart:Multipart = new Multipart([
					new Part("file", fileBytes, null, [ { name:"filename", value:file.name } ]),
					new Part("submit", "Upload")
				]);
				
				_startTime = new Date();
				
				client.listener.onComplete = function(event:HttpResponseEvent):void {
					// Notified when complete (after status and data)
					evalJavaScriptEvent(_jsOnProgress, [{
						"percent" : 100,
						"totalFileSize" : fileBytes.length,
						"numBytes" : fileBytes.length,
						"localPath" : _localPath,
						"remotePath" : _remotePath,
						"startTime" : _startTime
					}]);
				};
				
				client.postMultipart(uri, multipart);
			} else {
				this.evalJavaScriptEvent(_jsOnError, [{
					"code" : -1,
					"description" : localPath + " does not exist"				
				}]);
			}
			
			return result.jsObject;
		}
		
		public function download(remotePath : String, localPath : String, onProgressID : String, onErrorID : String, options : String) : Object {
			var result:WebWorksReturnValue;
			
			_successEvent = null;
			_mimeType = "";
			_redirectURL = "";
			_remotePath = remotePath;
			_localPath = localPath;
			_jsOnProgress = onProgressID;
			_jsOnError = onErrorID;
			
			var data:Object = {
				"remotePath" : remotePath,
				"localPath" : localPath,
				"onProgressId" : onProgressID,
				"onErrorId" : onErrorID
			};
			
			result = new WebWorksReturnValue(data);
			
			var optionsObj:Object = ( options != "" && options != null ) ? (JSON.decode(options) as Object) : null;
			
			_progressInterval = PROGRESS_INTERVAL;
			var connectionTimeout:Number = CONNECTION_TIMEOUT;
			
			if (optionsObj != null) {
				if (optionsObj["connectionTimeout"] != null) {
					connectionTimeout = optionsObj["connectionTimeout"];
				}
				
				_progressInterval = optionsObj["progressInterval"];
			}
			
			_ustream = new URLStream();			
			configureListeners(_ustream);
			
			var request:URLRequest = new URLRequest(remotePath);
			request.idleTimeout = connectionTimeout;
			request.followRedirects = true;
			
			try {
				_startTime = new Date();
				_ustream.load(request);		
			} catch (error:Error) {				
				result = new WebWorksReturnValue(data, -1);
			}
			
			return result.jsObject;
		}

		private function configureListeners(dispatcher : IEventDispatcher) : void {			
			dispatcher.addEventListener(Event.COMPLETE, downloadCompleteHandler);			
			dispatcher.addEventListener(Event.OPEN, openHandler);			
			dispatcher.addEventListener(ProgressEvent.PROGRESS, progressHandler);		
			dispatcher.addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler);			
			dispatcher.addEventListener(HTTPStatusEvent.HTTP_STATUS, httpStatusHandler);
			dispatcher.addEventListener(HTTPStatusEvent.HTTP_RESPONSE_STATUS, httpStatusHandler);
			dispatcher.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);	
		}
		
		/**	 
		 * Function to write data to user specified file system		 
		 */			
		private function writeFile():void {		
			var dataBuffer:ByteArray = new ByteArray();			
			_ustream.readBytes(dataBuffer, 0, _ustream.bytesAvailable);	
			var fStream:FileStream = new FileStream();
			var dataFile:File = new File(_localPath);
			fStream.openAsync(dataFile, FileMode.WRITE);		
			fStream.writeBytes(dataBuffer, 0, dataBuffer.length);	
			_ustream.close();
			fStream.close();
		}
		
		/**		 
		 * Upon successful completion of read, file is written		 
		 * to file system. 
		 */		
		private function downloadCompleteHandler(event : Event) : void {		
			writeFile();
			
			this.evalJavaScriptEvent(_jsOnProgress, [{
				"percent" : 100,
				"totalFileSize" : _successEvent.bytesTotal,
				"numBytes" : _successEvent.bytesLoaded,
				"localPath" : _localPath,
				"remotePath" : _remotePath,
				"redirectPath" : _redirectURL.length == 0 ? _remotePath : _redirectURL,
				"startTime" : _startTime,
				"mimeType" : _mimeType
			}]);
		}
		
		private function openHandler(event : Event) : void {	
			trace("openHandler: " + event);
		}
				
		private function progressHandler(event : ProgressEvent) : void {
			trace("progressHandler loaded:" + event.bytesLoaded + " total: " + event.bytesTotal);
			
			if (event.bytesLoaded == event.bytesTotal) {
				_successEvent = event;
				return;
			}
			
			if (_progressInterval != 0) {
				this.evalJavaScriptEvent(_jsOnProgress, [{
					"percent" : 100 * ( event.bytesLoaded / event.bytesTotal ),
					"totalFileSize" : event.bytesTotal,
					"numBytes" : event.bytesLoaded,
					"localPath" : _localPath,
					"remotePath" : _remotePath,
					"redirectPath" : _redirectURL.length == 0 ? _remotePath : _redirectURL,
					"startTime" : _startTime,
					"mimeType" : _mimeType
				}]);
			}
		}
				
		private function securityErrorHandler(event:SecurityErrorEvent) : void {			

		}
				
		private function httpStatusHandler(event:HTTPStatusEvent) : void {		
			trace("httpStatusHandler: " + event.target + " currentTarget=" + event.currentTarget + " url=" + event.responseURL + " responseHeaders=" + event.responseHeaders + " status=" + event.status);
			
			if (event.status != HTTP_OK) {
				// stop reporting progress if the request is not OK
				_ustream.removeEventListener(ProgressEvent.PROGRESS, progressHandler);
				_ustream.removeEventListener(Event.COMPLETE, downloadCompleteHandler);	
				
				this.evalJavaScriptEvent(_jsOnError, [{
					"code" : event.status,
					"description" : "Bad URL"				
				}]);
				
			}
			
			if (event.responseHeaders != null) {
				for (var i:Number = 0; i < event.responseHeaders.length; i++) {
					var header:URLRequestHeader = event.responseHeaders[i];
					if (header.name == MIME_TYPE_HEADER) {
						_mimeType = header.value;
					}
				}
			}
			
			if (event.responseURL != null && event.responseURL.length != 0) {
				_redirectURL = event.responseURL;
			}
				
		}
				
		private function ioErrorHandler(event:IOErrorEvent) : void {
			this.evalJavaScriptEvent(_jsOnError, [{
				"code" : event.errorID,
				"description" : event.text				
			}]);
		}
	}
}
