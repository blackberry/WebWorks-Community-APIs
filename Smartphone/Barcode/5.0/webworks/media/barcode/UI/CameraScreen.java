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

import javax.microedition.media.Manager;
import javax.microedition.media.MediaException;
import javax.microedition.media.Player;
import javax.microedition.media.control.VideoControl;

import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.Characters;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.container.MainScreen;
import webworks.media.barcode.ErrorObject;

import com.google.zxing.Binarizer;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.NotFoundException;
import com.google.zxing.Result;
import com.google.zxing.client.rim.BitmapLuminanceSource;
import com.google.zxing.common.GlobalHistogramBinarizer;

/**
 * A UI screen to display the camera display and buttons
 */
public final class CameraScreen extends MainScreen
{
    /** The camera's video controller */
    private VideoControl _videoControl;
//    private SnapshotControl _snapshotControl;

    private Player _player;
    /** The field containing the feed from the camera */
    private Field _videoField;    
    
    private int _runnableID = -1;
    
    private boolean _isCameraInitialized = false;
    
	private ScriptableFunction _captureCallback;
	private ScriptableFunction _errorCallback;
	private Hashtable _hints;

    /**
     * Constructor. Initializes the camera and creates the UI.
     */
    public CameraScreen(final ScriptableFunction captureCallback,final ScriptableFunction errorCallback, final Hashtable hints)
    {
    	
    	_captureCallback = captureCallback;
    	_errorCallback = errorCallback;
    	_hints = hints;
    	    	    	

        // Initialize the camera object and video field
        initializeCamera();
        
        // If the field was constructed successfully, create the UI
        if(_videoField != null)
        {
        	add(_videoField);
        }
        // If not, display an error message to the user
        else
        {
            returnError("Error connecting to camera." );
        }
    }       
    
    public VideoControl getVideoControl(){
    	return _videoControl;
    }    

    /**
     * Initializes the Player, VideoControl and VideoField
     */
    private void initializeCamera()
    {
        try
        {
            // Create a player for the Blackberry's camera
            _player = Manager.createPlayer( "capture://video?encoding=image/jpeg" );

            // Set the player to the REALIZED state (see Player javadoc)
            _player.realize();

            
            // Grab the video control and set it to the current display
            _videoControl = (VideoControl)_player.getControl( "VideoControl" );

            if (_videoControl != null)
            {
                // Create the video field as a GUI primitive (as opposed to a
                // direct video, which can only be used on platforms with
                // LCDUI support.)
                _videoField = (Field) _videoControl.initDisplayMode (VideoControl.USE_GUI_PRIMITIVE, "net.rim.device.api.ui.Field");
                _videoControl.setDisplayFullScreen(true);
                _videoControl.setVisible(true);                
            }

            // Set the player to the STARTED state (see Player javadoc)
            _player.start();
            
            _isCameraInitialized = true;
					
        } catch(Exception e) {
        	returnError("ERROR " + e.getClass() + ":  " + e.getMessage());
        }
    }

	public void startScanner(){		
		if(_isCameraInitialized && _runnableID == -1){
			_runnableID = UiApplication.getUiApplication().invokeLater(new DecodeRunnable(CameraScreen.this), 3000, true);
		}
    }
    
    private void stopScanner(){
    	if(_runnableID != -1){
    		UiApplication.getUiApplication().cancelInvokeLater(_runnableID);
    		_runnableID = -1;
    	}
    	if(_player.getState() != Player.CLOSED){
    		try {
				_player.stop();
			} catch (MediaException e) {
				returnError("ERROR " + e.getClass() + ":  " + e.getMessage());
			}
    	}
    }
    
    
    public void onSuccessfulDecode(Result r, byte[] rawData){
    	stopScanner();
    	returnSuccess(r.getText());
    }
    
    /**
     * @see net.rim.device.api.ui.Screen#keyChar()
     */
    protected boolean keyChar(char key, int status, int time)
    {
        if (key == Characters.ESCAPE) 
        {
            stopScanner();
    		try{
    			UiApplication.getUiApplication().popScreen(CameraScreen.this);
    		}catch(IllegalArgumentException iae){
    			//eat the IAE if the screen has been popped
    		}
        }
        
        return super.keyChar(key, status, time);
    }
    
    
    /**
     * @see net.rim.device.api.ui.Screen#close()
     */
    public void close()
    {
    	stopScanner();
		try{
			UiApplication.getUiApplication().popScreen(CameraScreen.this);
		}catch(IllegalArgumentException iae){
			//eat the IAE if the screen has been popped
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
            		try{
            			UiApplication.getUiApplication().popScreen(CameraScreen.this);
            		}catch(IllegalArgumentException iae){
            			//eat the IAE if the screen has been popped
            		}
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
                try {
            		try{
            			UiApplication.getUiApplication().popScreen(CameraScreen.this);
            		}catch(IllegalArgumentException iae){
            			//eat the IAE if the screen has been popped
            		}
					_errorCallback.invoke(null,new Object[] {new ErrorObject(-2, text)});
				} catch (Exception e) {
				}
            } 
        });
    }
    
    
    private class DecodeRunnable implements Runnable{
    	
    	private CameraScreen _cameraScreen;

    	public DecodeRunnable(CameraScreen cameraScreen) {
    		super();
    		_cameraScreen = cameraScreen;
    	}

    	public void run() {
    		Result r = null;
    		 byte[] rawData = null;
    	       try {
    	    	    rawData = _cameraScreen.getVideoControl().getSnapshot( "encoding=jpeg&width=1024&height=768&quality=normal" );
    	            BitmapLuminanceSource bls = new BitmapLuminanceSource(Bitmap.createBitmapFromBytes( rawData, 0, -1, 1 ));
    	            Binarizer b = new GlobalHistogramBinarizer(bls);
    	            BinaryBitmap bb = new BinaryBitmap(b);
    	            MultiFormatReader reader = new MultiFormatReader();
    				r = reader.decode(bb,_hints);
    			} catch (NotFoundException e) {
    				//Don't throw an error here, just keep scanning
//    				CameraScreen.this.returnError("ERROR " + e.getClass() + ":  " + e.getMessage());
    			} catch (MediaException e) {
    				CameraScreen.this.returnError("ERROR " + e.getClass() + ":  " + e.getMessage());
    			}
    			if(r!=null){
    				final Result threadResult = r;
    				final byte[] threadData = rawData;
    				UiApplication.getUiApplication().invokeLater(new Runnable() {						
						public void run() {
							_cameraScreen.onSuccessfulDecode(threadResult,threadData);							
						}
					});
    				
    			}
    		
    	}
    }        
}
