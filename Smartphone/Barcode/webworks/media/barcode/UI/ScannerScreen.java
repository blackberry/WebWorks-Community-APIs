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

package webworks.media.barcode.UI;

import java.util.Hashtable;

import javax.microedition.media.MediaException;
import javax.microedition.media.Player;
import javax.microedition.amms.control.camera.FocusControl;
import javax.microedition.amms.control.camera.ZoomControl;

import webworks.media.barcode.ErrorObject;

import net.rim.device.api.barcodelib.BarcodeDecoder;
import net.rim.device.api.barcodelib.BarcodeDecoderListener;
import net.rim.device.api.barcodelib.BarcodeScanner;

import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.KeyListener;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.Keypad;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.container.MainScreen;


public class ScannerScreen extends MainScreen {

	private BarcodeScanner barcodeScanner;
	private Player cameraPlayer;
	private ScriptableFunction captureCallback;
	private ScriptableFunction errorCallback;
	
	
    
    /**
     * Creates a new ViewFinderScreen object
     */
    public ScannerScreen(final ScriptableFunction captureCallback,final ScriptableFunction errorCallback, final Hashtable hints )
    {
    	this.setCaptureCallback(captureCallback);
    	this.setErrorCallback(errorCallback);

    	// Initialize the BarcodeDecoder
        BarcodeDecoder decoder = new BarcodeDecoder(hints);
        
        // Create a custom instance of a BarcodeDecoderListener to pop the
        // screen and display results when a  barcode is recognized.
        BarcodeDecoderListener decoderListener = new BarcodeDecoderListener()
        {
            /**
             * @see BarcodeDecoderListener#barcodeDecoded(String)
             */
            public void barcodeDecoded(String rawText)
            {

                returnSuccess(rawText);
            }
        };
            
        try
        {
            // Initialize the BarcodeScanner object and add the associated
            // view finder.
        	if(this.getBarcodeScanner() != null) {
        		/*
        		 * There may be a player active so we need to destroy the player prior to proceeding
        		 * because only one player instance can be active at any given time.
        		 */
        		Player cpTemp = this.getCameraPlayer();
        		cpTemp.close();
        		this.setCameraPlayer(null);
        		this.setBarcodeScanner(null);
        		
        	}
            BarcodeScanner bcScanner = new BarcodeScanner(decoder, decoderListener);
            this.setBarcodeScanner(bcScanner);
            this.setCameraPlayer(bcScanner.getPlayer());
           	Field scannerField = bcScanner.getViewfinder();
            

           	bcScanner.getVideoControl().setDisplayFullScreen(true);
			// Create and add our key listener to the screen
			this.addKeyListener(new KeyPadListener());
            add(scannerField);

            
        }
        catch(Exception e)
        {
            returnError("ScannerScreen Constructor Error: " + e.getMessage());
        }
                
    }        

    
    /**
     * Informs the BarcodeScanner that it should begin scanning for QR Codes
     */
    public void startScan()
    {
        try
        {
        	BarcodeScanner bcScanner = this.getBarcodeScanner();
            bcScanner.startScan();
        }
        catch(MediaException me)
        {
        	returnError("ScannerScreen.startScan Error: " + me.getMessage());
        }
    }
    
    
    /***
	 * Keylistener class
	 * <p>
	 * This KeyListener will stop the current scan cleanly when the back
	 * button is pressed, and then pop the viewfinder off the stack.
	 */
	private class KeyPadListener implements KeyListener {

		public boolean keyDown(int keycode, int time) {

			// First convert the keycode into an actual key event
			int key = Keypad.key(keycode);

			//get the keypad code and compare it to the key that was pressed. If it matches
			//stop the scan, close the camera player, and pop the screen
			if (key == Keypad.KEY_ESCAPE) {
				try {
					if(barcodeScanner != null || cameraPlayer != null) {
						barcodeScanner.stopScan();
						cameraPlayer.close();						
					}

					UiApplication.getUiApplication().popScreen(ScannerScreen.this);
				} catch (Exception e) {
					returnError("MyKeyListener Error: " + e.getMessage());
				}

				return true;

			}
			// Otherwise, we'll return false so as not to consume the
			// keyDown event
			return false;
		}

		// We will only act on the keyDown event
		public boolean keyChar(char key, int status, int time) {
			return false;
		}

		public boolean keyRepeat(int keycode, int time) {
			return false;
		}

		public boolean keyStatus(int keycode, int time) {
			return false;
		}

		public boolean keyUp(int keycode, int time) {
			return false;
		}

	}

    
    
    /**
     * @see net.rim.device.api.ui.Screen#close()
     */
    public void close()
    {
        try 
        {
        	if(this.getBarcodeScanner() != null) {
                BarcodeScanner bcScanner = this.getBarcodeScanner();
                bcScanner.stopScan();       		
        	}
        	Player cPlayer = this.getCameraPlayer();
        	if(cPlayer != null && (cPlayer.getState() != Player.CLOSED)) {
        		cPlayer.close();
        	}
            UiApplication.getUiApplication().popScreen(ScannerScreen.this);
        }
        catch (MediaException me)
        {
        	returnError("ScannerScreen.close Error: " + me.getMessage());
        }
        
        super.close();
    }
    
    
    /**
     * Pops the ViewFinderScreen and displays text on the main screen
     * 
     * @param text Text to display on the screen
     */
    private void returnSuccess(final String text)
    {
        UiApplication.getUiApplication().invokeLater(new Runnable()
        {
            public void run()
            {   
            	
                try {
                	barcodeScanner.stopScan();
    				cameraPlayer.close();
                    UiApplication.getUiApplication().popScreen(ScannerScreen.this);
					captureCallback.invoke(null, new Object[] { text });
				}  catch (MediaException me) {
					returnError("ScannerScreen.returnSuccess Error: " + me.getMessage());
				} catch (Exception e) {
				}
            } 
        });
    }
    
    /**
     * Pops the ViewFinderScreen and displays text on the main screen
     * 
     * @param text Text to display on the screen
     */
    public void returnError(final String text)
    {
        UiApplication.getUiApplication().invokeLater(new Runnable()
        {
            public void run()
            {                
               
                try {
                	barcodeScanner.stopScan();
    				cameraPlayer.close();
                    UiApplication.getUiApplication().popScreen(ScannerScreen.this);
					errorCallback.invoke(null,new Object[] {new ErrorObject(-1, text)});
				} catch (Exception e) {
				}
            } 
        });
    }
	public BarcodeScanner getBarcodeScanner() {
		return barcodeScanner;
	}


	public void setBarcodeScanner(BarcodeScanner barcodeScanner) {
		this.barcodeScanner = barcodeScanner;
	}


	public Player getCameraPlayer() {
		return cameraPlayer;
	}


	public void setCameraPlayer(Player cameraPlayer) {
		this.cameraPlayer = cameraPlayer;
	}


	public ScriptableFunction getCaptureCallback() {
		return captureCallback;
	}


	public void setCaptureCallback(ScriptableFunction captureCallback) {
		this.captureCallback = captureCallback;
	}


	public ScriptableFunction getErrorCallback() {
		return errorCallback;
	}


	public void setErrorCallback(ScriptableFunction errorCallback) {
		this.errorCallback = errorCallback;
	}
	
	public void setAutoFocus () {
		try {
			FocusControl focusControl = (FocusControl) this.cameraPlayer
					.getControl("javax.microedition.amms.control.camera.FocusControl");
			if (focusControl.isAutoFocusSupported()) {
				focusControl.setFocus(FocusControl.AUTO);
			} else {
				returnError("ScannerScreen.setAutoFocus Error: Auto focus not supported.");
			}
		} catch (MediaException me) {
			returnError("ScannerScreen.setAutoFocus Error: " + me.getMessage());
		}		
	}

	public void setMacro() {
		try {
			FocusControl focusControl = (FocusControl) this.cameraPlayer
					.getControl("javax.microedition.amms.control.camera.FocusControl");
			if (focusControl.isMacroSupported()) {
				focusControl.setMacro(true);
			} else {
				returnError("ScannerScreen.setMacro Error: Macro focus is not supported.");
			}
		} catch (MediaException me) {
			returnError("ScannerScreen.setMacro Error: " + me.getMessage());
		}
	}

	public void setDigitalZoom(int zoom) {
		try {
			ZoomControl zoomControl = (ZoomControl) this.cameraPlayer
					.getControl("javax.microedition.amms.control.camera.ZoomControl");

			if (zoomControl.getMaxDigitalZoom() == 100) {
				returnError("ScannerScreen.setDigitalZoom Error: Digital zoom not supported.");
			} else {
				zoomControl.setDigitalZoom(zoom);
			}
		} catch (IllegalArgumentException iae) {
			returnError("ScannerScreen.setDigitalZoom Error: " + iae.getMessage());
		}
	}

	public void setOpticalZoom(int zoom) {
		try {
			ZoomControl zoomControl = (ZoomControl) this.cameraPlayer
					.getControl("javax.microedition.amms.control.camera.ZoomControl");

			if (zoomControl.getMaxOpticalZoom() == 100) {
				returnError("ScannerScreen.setOpticallZoom Error: Optical zoom not supported.");
			} else {
				zoomControl.setOpticalZoom(zoom);
			}
		} catch (IllegalArgumentException iae) {
			returnError("ScannerScreen.setOpticalZoom Error: " + iae.getMessage());
		}
	}
	
}
