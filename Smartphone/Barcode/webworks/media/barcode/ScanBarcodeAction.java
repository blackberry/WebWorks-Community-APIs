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

import java.util.Hashtable;
import java.util.Vector;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Application;
import net.rim.device.api.ui.UiApplication;
import webworks.media.barcode.UI.ScannerScreen;

import com.google.zxing.BarcodeFormat;
import com.google.zxing.DecodeHintType;

public class ScanBarcodeAction extends ScriptableFunction {
	public static final String NAME = "scan";

	private ScannerScreen _scannerScreen;	

	/* @Override */
	public Object invoke(final Object thiz, final Object[] args) throws Exception {

		ScriptableFunction onCaptureArg = null;
		ScriptableFunction onErrorArg = null;
		Scriptable optionsArg = null;

		if(args.length < 1){
			throw new IllegalArgumentException("Missing Captured Callback");
		}else{
			onCaptureArg = (ScriptableFunction) args[0];
			if(args.length > 1){
				onErrorArg =  (ScriptableFunction) args[1];
				if(args.length > 2){
					optionsArg = (Scriptable) args[2];
				}
			}
		}

		final ScriptableFunction capturedCallback = onCaptureArg;
		final ScriptableFunction errorCallback = onErrorArg;

		final Hashtable hints = new Hashtable(5);
		if(optionsArg != UNDEFINED){
			try{
				Object tryHarder = optionsArg.getField("tryHarder");
				if(tryHarder != UNDEFINED){
					hints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
				}

				Object formatObj = optionsArg.getField("formats");
				Vector formatVector = new Vector();
				if(formatObj != UNDEFINED){
					Scriptable formats = (Scriptable) formatObj;
					for(int i=0;i<formats.getElementCount();i++){
						String format = ((String) formats.getElement(i)).toUpperCase();
						BarcodeFormat bcFormat;
						if (format.equals("1D")){
							formatVector.addElement(BarcodeFormat.CODE_128);	
							formatVector.addElement(BarcodeFormat.CODE_39);
							formatVector.addElement(BarcodeFormat.EAN_13);
							formatVector.addElement(BarcodeFormat.EAN_8);
							formatVector.addElement(BarcodeFormat.ITF);	
							formatVector.addElement(BarcodeFormat.UPC_A);
							formatVector.addElement(BarcodeFormat.UPC_E);
						}else if (format.equals("2D")){
							formatVector.addElement(BarcodeFormat.QR_CODE);
							formatVector.addElement(BarcodeFormat.DATAMATRIX);
							formatVector.addElement(BarcodeFormat.PDF417);
						}else{
							try{
								bcFormat = BarcodeFormat.valueOf(format);
							}catch(IllegalArgumentException iae){							
								bcFormat = null;
							}
							if(bcFormat != null){
								formatVector.addElement(bcFormat);
							}
						}
					}
					
				}
				
				if(formatVector.isEmpty()){
					//if formats hint not given use all
					formatVector.addElement(BarcodeFormat.QR_CODE);
					formatVector.addElement(BarcodeFormat.CODE_128);	
					formatVector.addElement(BarcodeFormat.CODE_39);
					formatVector.addElement(BarcodeFormat.DATAMATRIX);
					formatVector.addElement(BarcodeFormat.EAN_13);
					formatVector.addElement(BarcodeFormat.EAN_8);
					formatVector.addElement(BarcodeFormat.ITF);
					formatVector.addElement(BarcodeFormat.PDF417);
					formatVector.addElement(BarcodeFormat.UPC_A);
					formatVector.addElement(BarcodeFormat.UPC_E);
				}
				
				hints.put(DecodeHintType.POSSIBLE_FORMATS, formatVector);
			}catch(Exception e){
				returnError(errorCallback, "Error! " + e.getMessage());
			}
		}
		
		


		Application.getApplication().invokeLater(new Runnable(){

			public void run() {
				// If no screen exists, create one before displaying
				if(_scannerScreen == null)
				{
					_scannerScreen = new ScannerScreen(capturedCallback,errorCallback,hints);
				}

				// Push view finder screen onto the display stack
				UiApplication.getUiApplication().pushScreen(_scannerScreen);

				// Begin the scanning process
				_scannerScreen.startScan();

			}

		});

		return UNDEFINED;

	}  
	
    /**
     * Pops the ViewFinderScreen and displays text on the main screen
     * 
     * @param text Text to display on the screen
     */
    private void returnError(final ScriptableFunction errorCallback, final String text)
    {
        UiApplication.getUiApplication().invokeLater(new Runnable()
        {
            public void run()
            {                
                try {
					errorCallback.invoke(null,new Object[] {new ErrorObject(-1, text)});
				} catch (Exception e) {
				}
            } 
        });
    }

}
