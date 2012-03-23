/*
 * Copyright 2011 Research In Motion Limited.
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

package webworks.media.barcode;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Date;
import java.util.Hashtable;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.system.JPEGEncodedImage;
import net.rim.device.api.system.PNGEncodedImage;
import net.rim.device.api.ui.UiApplication;

import com.google.zxing.BarcodeFormat;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.common.BitArray;
import com.google.zxing.common.BitMatrix;

public class GenerateBarcodeAction extends ScriptableFunction {
	public static final String NAME = "generate";

	/* @Override */
	public Object invoke(final Object thiz, final Object[] args) throws Exception {

		String contentArg = null;
		ScriptableFunction onGenerateArg = null;
		ScriptableFunction onErrorArg = null;
		Scriptable optionsArg = null;

		if(args.length < 2){
			throw new IllegalArgumentException("Missing Captured Callback");
		}else{
			contentArg = (String) args[0];
			onGenerateArg = (ScriptableFunction) args[1];
			if(args.length > 2){
				onErrorArg =  (ScriptableFunction) args[2];
				if(args.length > 3){
					optionsArg = (Scriptable) args[3];
				}
			}
		}            	

		final String contents = contentArg;
		final ScriptableFunction generateFunction = onGenerateArg;
		final ScriptableFunction errorFunction = onErrorArg;
		final Scriptable options = optionsArg;

		GenerateBarcodeThread thread = new GenerateBarcodeThread(contents, generateFunction, errorFunction, options, thiz);
		UiApplication.getUiApplication().invokeLater(thread);
		
		return UNDEFINED;

	}

	private class GenerateBarcodeThread extends Thread{

		private final String[] ALLOWED_EXTENSIONS = {"jpq","jpeg","png"};

		private String _contents;
		private ScriptableFunction _generateFunction;
		private ScriptableFunction _errorFunction;
		private Scriptable _options;
		private Object _thiz;

		public GenerateBarcodeThread(final String contents, final ScriptableFunction generateFunction, final ScriptableFunction errorFunction, final Scriptable options, final Object thiz) {
			super();

			this._contents = contents;
			this._generateFunction = generateFunction;
			this._errorFunction = errorFunction;
			this._options = options;
			this._thiz = thiz;
		}


		public void run() {			
			try{
				int width = 50;
				int height = 50;
				BarcodeFormat format = BarcodeFormat.QR_CODE;
				Hashtable hints = new Hashtable(2);
				String filePath = "file:///SDCard/";
				String fileName = ""+(new Date()).getTime();
				String fileExtension = "png";

				if(this._options != null){
					Object  widthObj =this._options.getField("width");
					if(widthObj != UNDEFINED){
						width =((Integer)widthObj).intValue();
					}

					Object heightObj = this._options.getField("height");
					if(heightObj != UNDEFINED){
						height = ((Integer)heightObj).intValue();
					}

					Object formatObj = this._options.getField("format");
					if(formatObj != UNDEFINED){
						format = BarcodeFormat.valueOf(((String)formatObj).toUpperCase());
					}	                	

					//Hints
					Object charSetObj = this._options.getField("characterSet");
					if(charSetObj != UNDEFINED){
						hints.put(EncodeHintType.CHARACTER_SET, (String)charSetObj);
					}
					Object errorCorrectionObj = this._options.getField("errorCorrection");
					if(errorCorrectionObj != UNDEFINED){
						hints.put(EncodeHintType.ERROR_CORRECTION, (Integer)errorCorrectionObj);
					}

					Object filePathObj = this._options.getField("filePath");
					if(filePathObj != UNDEFINED){
						String filePathOrig = (String)filePathObj;
						fileExtension = filePathOrig.substring(filePathOrig.lastIndexOf('.')+1);
						fileName = filePathOrig.substring(filePathOrig.lastIndexOf('/')+1,filePathOrig.lastIndexOf('.'));
						filePath = filePathOrig.substring(0, filePathOrig.lastIndexOf('/'));

						boolean validExtension = false;
						for( int i =0; i< ALLOWED_EXTENSIONS.length;i++){
							validExtension |= fileExtension.equals(ALLOWED_EXTENSIONS[i]);
						}
						if(!validExtension){
							fileExtension = "png";
						}
					}	                		                	                	

				}

				Bitmap b = createBarcodeBitmap(this._contents, format, width, height, hints);
				String filePathSaved = saveImage(fileName, filePath, fileExtension, b);
				this._generateFunction.invoke(this._thiz, new Object[]{filePathSaved});

			}catch(Exception e){            		
				throwError(e);
			}
		}
		
		private Bitmap createBarcodeBitmap(String contents, BarcodeFormat format, int width, int height, Hashtable hints) throws Exception{
			MultiFormatWriter writer = new MultiFormatWriter();
			BitMatrix bm;                	
			try{
				bm = writer.encode(contents, format, width, height,hints);
			}catch(ClassCastException cce){
				throw new Exception("ZXing Barcode Writer FAILURE - try different paramteres and pray");
			}
			//Bitmap b = BarcodeBitmap.createBitmap(bm,0);
			Bitmap b = new Bitmap(width,height);

			//iterate through the matrix and draw the pixels to the image
			//a Jeff and BT super hack to make generating a barcode work
			for (int y = 0; y < height; y++) { 
				int[] argbArray = new int[width];
				BitArray ba = bm.getRow(y, null);
				for (int x = 0; x < width; x++) {
					boolean isSet = ba.get(x);
					int pixel = (isSet ? 0xFF000000 : 0xFFFFFFFF);
					argbArray[x] = pixel;
				}
				b.setARGB(argbArray,0, width, 0,y, width, 1);
			}
			
			return b;
		}
		
		private String saveImage(String fileName,final String filePath,final String fileExtension, final Bitmap b) throws IOException{
			FileConnection fconn = null;
			try {
				fconn = (FileConnection)Connector.open(filePath+"/"+fileName+"."+fileExtension, Connector.READ_WRITE);
				
				if(fconn.exists()){
					fileName += (new Date()).getTime();                    	
					fconn.close();
					fconn = (FileConnection)Connector.open(filePath+"//"+fileName+"."+fileExtension, Connector.READ_WRITE);
				}

				fconn.create();

				OutputStream outputStream = fconn.openOutputStream();
				EncodedImage encodedImage;
				if(fileExtension.equals("jpg") || fileExtension.equals("jpeg")){
					encodedImage = JPEGEncodedImage.encode(b, 100);
				}else{
					encodedImage = PNGEncodedImage.encode(b);
				}
				byte[] imageBytes = encodedImage.getData();
				outputStream.write(imageBytes);
				outputStream.close();

				fconn.close();
			} catch (IOException e) {
				throw e;
			} finally{
				try {
					fconn.close();
				} catch (IOException e) {
					//Ignore
				}
			}
						
			return  filePath+"//"+fileName+"."+fileExtension;
		}
		
		private void throwError(Exception e){
			if(this._errorFunction != null){

				try {
					this._errorFunction.invoke(this._thiz, new Object[] {new ErrorObject(-1, e.getMessage())} );
				} catch (Exception e1) {
				}
			}
		}

	}
	

}
