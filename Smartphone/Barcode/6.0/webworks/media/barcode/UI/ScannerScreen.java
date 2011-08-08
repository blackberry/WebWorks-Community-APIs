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

import webworks.media.barcode.ErrorObject;

import net.rim.device.api.barcodelib.BarcodeDecoder;
import net.rim.device.api.barcodelib.BarcodeDecoderListener;
import net.rim.device.api.barcodelib.BarcodeScanner;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Characters;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.container.MainScreen;

public class ScannerScreen extends MainScreen {

	private BarcodeScanner _scanner;
	
	private ScriptableFunction _captureCallback;
	private ScriptableFunction _errorCallback;
	
	
    
    /**
     * Creates a new ViewFinderScreen object
     */
    public ScannerScreen(final ScriptableFunction captureCallback,final ScriptableFunction errorCallback, final Hashtable hints )
    {
    	_captureCallback = captureCallback;
    	_errorCallback = errorCallback;
    	
    	
    
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
            _scanner = new BarcodeScanner(decoder, decoderListener);
            _scanner.getVideoControl().setDisplayFullScreen(true);
            add(_scanner.getViewfinder());
            
        }
        catch(Exception e)
        {
            returnError("Error: " + e.getMessage());
        }
                
    }        

    
    /**
     * Informs the BarcodeScanner that it should begin scanning for QR Codes
     */
    public void startScan()
    {
        try
        {
            _scanner.startScan();
        }
        catch(MediaException me)
        {
        	returnError("Error: " + me.getMessage());
        }
    }
    
    
    /**
     * @see net.rim.device.api.ui.Screen#keyChar()
     */
    protected boolean keyChar(char key, int status, int time)
    {
        if (key == Characters.ESCAPE) 
        {
            // Manually stop the scanning process and pop the screen
            try
            {
                _scanner.stopScan();
                UiApplication.getUiApplication().popScreen(ScannerScreen.this);
            }
            catch (MediaException me)
            {
            	returnError("Error: " + me.getMessage());
            }
        }
        
        return super.keyChar(key, status, time);
    }
    
    
    /**
     * @see net.rim.device.api.ui.Screen#close()
     */
    public void close()
    {
        try 
        {
            _scanner.stopScan();
            UiApplication.getUiApplication().popScreen(ScannerScreen.this);
        }
        catch (MediaException me)
        {
        	returnError("Error: " + me.getMessage());
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
                UiApplication.getUiApplication().popScreen(ScannerScreen.this);
                try {
					_captureCallback.invoke(null, new Object[] { text });
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
                UiApplication.getUiApplication().popScreen(ScannerScreen.this);
                try {
					_errorCallback.invoke(null,new Object[] {new ErrorObject(-1, text)});
				} catch (Exception e) {
				}
            } 
        });
    }
}
