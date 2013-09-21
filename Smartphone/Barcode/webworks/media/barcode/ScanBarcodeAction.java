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
	public boolean autoFocus = false;
	public boolean macroFocus = false;
	public int digitalZoom = -1;
	public int opticalZoom = -1;
	/* @Override */
	public Object invoke(final Object thiz, final Object[] args)
			throws Exception {

		ScriptableFunction onCaptureArg = null;
		ScriptableFunction onErrorArg = null;
		Scriptable optionsArg = null;
		Scriptable focusArgs = null;
		Scriptable zoomArgs = null;


		if (args.length < 1) {
			throw new IllegalArgumentException("Missing Captured Callback");
		} else {
			for(int i=0; i<args.length; i++) {
				switch(i) {
				case 0:
					onCaptureArg = (ScriptableFunction) args[i];
					break;
				case 1:
					onErrorArg = (ScriptableFunction) args[i];
					break;
				case 2:
					optionsArg = (Scriptable) args[i];
					break;
				case 3:
					focusArgs = (Scriptable) args[i];
					break;
				case 4:
					zoomArgs = (Scriptable) args[i];
					break;
				}
					
			}
		}

		final ScriptableFunction capturedCallback = onCaptureArg;
		final ScriptableFunction errorCallback = onErrorArg;

		final Hashtable hints = new Hashtable(5);
		if (optionsArg != UNDEFINED) {
			try {
				Object tryHarder = optionsArg.getField("tryHarder");
				if (tryHarder != UNDEFINED) {
					hints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
				}

				Object formatObj = optionsArg.getField("formats");
				Vector formatVector = new Vector();
				if (formatObj != UNDEFINED) {
					Scriptable formats = (Scriptable) formatObj;
					for (int i = 0; i < formats.getElementCount(); i++) {
						String format = (String) formats.getElement(i);
						BarcodeFormat bcFormat;
						if (format.equalsIgnoreCase("CODE_128")) {
							formatVector.addElement(BarcodeFormat.CODE_128);
							continue;
						} else if (format.equalsIgnoreCase("CODE_39")) {
							formatVector.addElement(BarcodeFormat.CODE_39);
							continue;
						} else if (format.equalsIgnoreCase("EAN_13")) {
							formatVector.addElement(BarcodeFormat.EAN_13);
							continue;
						} else if (format.equalsIgnoreCase("EAN_8")) {
							formatVector.addElement(BarcodeFormat.EAN_8);
							continue;
						} else if (format.equalsIgnoreCase("ITF")) {
							formatVector.addElement(BarcodeFormat.ITF);
							continue;
						} else if (format.equalsIgnoreCase("UPC_A")) {
							formatVector.addElement(BarcodeFormat.UPC_A);
							continue;
						} else if (format.equalsIgnoreCase("UPC_E")) {
							formatVector.addElement(BarcodeFormat.UPC_E);
							continue;
						} else if (format.equalsIgnoreCase("QR_CODE")) {
							formatVector.addElement(BarcodeFormat.QR_CODE);
							continue;
						} else if (format.equalsIgnoreCase("DATAMATRIX")) {
							formatVector.addElement(BarcodeFormat.DATAMATRIX);
							continue;
						} else if (format.equalsIgnoreCase("PDF417")) {
							formatVector.addElement(BarcodeFormat.PDF417);
							continue;
						} else {
							try {
								bcFormat = BarcodeFormat.valueOf(format);
							} catch (IllegalArgumentException iae) {
								bcFormat = null;
							}
							if (bcFormat != null) {
								formatVector.addElement(bcFormat);
							}
						}
					}

				}

				if (formatVector.isEmpty()) {
					// if formats hint not given use all
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
				
				// set focus options
				if (focusArgs != UNDEFINED) {
					Object autoFocusOption = focusArgs
							.getField("autoFocus");
					Object macroFocusOption = focusArgs
							.getField("macroFocus");

					if(autoFocusOption != UNDEFINED) {
						if (autoFocusOption.equals(Boolean.TRUE)) {
							this.autoFocus = true;
						}
					}
					if(macroFocusOption != UNDEFINED) {
						if (macroFocusOption.equals(Boolean.TRUE)) {
							this.macroFocus = true;
						}
					}
				}
				// set zoom options
				if(zoomArgs != UNDEFINED) {
					Object digitalZoomFocusOption = zoomArgs.getField("digitalZoom");
					Object opticalZoomFocusOption = zoomArgs.getField("opticalZoom");
					if(digitalZoomFocusOption != UNDEFINED) {
						this.digitalZoom = Integer.parseInt((String)digitalZoomFocusOption);
					}
					if(opticalZoomFocusOption != UNDEFINED) {
						this.opticalZoom = Integer.parseInt((String)opticalZoomFocusOption);
					}
				}
			} catch (Exception e) {
				returnError(errorCallback, "ScanBarcodeAction.invoke Error! " + e.getMessage());
			}
		}

		Application.getApplication().invokeLater(new Runnable() {

			public void run() {
				// create new local copy of the scanner screen which can be
				// recreated on each event.
				ScannerScreen _scannerScreen = new ScannerScreen(
						capturedCallback, errorCallback, hints);
				if(autoFocus) {
					_scannerScreen.setAutoFocus();
				}
				if(macroFocus) {
					_scannerScreen.setMacro();
				}
				if(digitalZoom != -1) {
					_scannerScreen.setDigitalZoom(digitalZoom);
				}
				if(opticalZoom != -1) {
					_scannerScreen.setOpticalZoom(opticalZoom);
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
	 * @param text
	 *            Text to display on the screen
	 */
	private void returnError(final ScriptableFunction errorCallback,
			final String text) {
		UiApplication.getUiApplication().invokeLater(new Runnable() {
			public void run() {
				try {
					errorCallback.invoke(null, new Object[] { new ErrorObject(
							-1, text) });
				} catch (Exception e) {
				}
			}
		});
	}

}
