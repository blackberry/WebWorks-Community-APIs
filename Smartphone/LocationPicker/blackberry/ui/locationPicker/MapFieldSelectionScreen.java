/*
 * MapFieldSelectionScreen.java
 *
 * Research In Motion Limited proprietary and confidential
 * Copyright Research In Motion Limited, 2012-2012
 */

package blackberry.ui.locationPicker;

import javax.microedition.location.LocationException;
import javax.microedition.location.LocationProvider;
import javax.microedition.location.QualifiedCoordinates;

import org.json.me.JSONException;
import org.json.me.JSONObject;

import net.rim.blackberry.api.menuitem.ApplicationMenuItem;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.*;
import net.rim.device.api.ui.container.*;
import net.rim.device.api.ui.component.*;
import net.rim.device.api.ui.input.*;
import net.rim.device.api.util.MathUtilities;

import net.rim.device.api.gps.BlackBerryCriteria;
import net.rim.device.api.gps.BlackBerryLocation;
import net.rim.device.api.gps.BlackBerryLocationProvider;
import net.rim.device.api.gps.LocationInfo;
import net.rim.device.api.lbs.maps.Boxable;
import net.rim.device.api.lbs.maps.LatLonRectangle;
import net.rim.device.api.lbs.maps.MapConstants;
import net.rim.device.api.lbs.maps.MapDimensions;
import net.rim.device.api.lbs.maps.model.MapMarker;
import net.rim.device.api.lbs.maps.model.MapPoint;
import net.rim.device.api.lbs.maps.model.PeerPoint;
import net.rim.device.api.lbs.maps.ui.MapAction;
import net.rim.device.api.lbs.maps.ui.MapField;

/**
 * Screen containing a MapField in selection mode. Every time the user selects a
 * location (by tapping or clicking on the map) it will be added as a pin to the map.
 */
public class MapFieldSelectionScreen extends FullScreen implements FieldChangeListener {
    
    private MapField map;
    private ScriptableFunction _callback;
    private Scriptable _ops = null;
    private MapPoint currSelection;
    
    public MapFieldSelectionScreen(ScriptableFunction callback, Scriptable ops) {
        super( FullScreen.DEFAULT_CLOSE | FullScreen.DEFAULT_MENU | FullScreen.VERTICAL_SCROLL | FullScreen.VERTICAL_SCROLLBAR );
        
        _callback = callback;
        if (ops != null) _ops = ops;
        
     // Register for pinch events so we can get pinch-to-zoom.
        final InputSettings is = TouchscreenSettings.createEmptySet();
        is.set( TouchscreenSettings.DETECT_PINCH, 1 );
        addInputSettings( is );
        
        map = new MapField();
        
        // Add a change listener to listen to changes to the MapField.
        map.addChangeListener( this );
        add( map );
        
        new Thread () {
			public void run() {
				try
				{
					//get current location - 4 tries or bust - in case of invalid fix
			        BlackBerryLocation myLocation = getGeolocation(4);
					
			        // Show location.
			        if (myLocation != null){
			        	currSelection = new MapPoint(myLocation.getQualifiedCoordinates());
			        	PeerPoint point = new PeerPoint(myLocation.getQualifiedCoordinates().getLatitude(), myLocation.getQualifiedCoordinates().getLongitude());
			        	point.setAccuracy(MathUtilities.round(myLocation.getQualifiedCoordinates().getHorizontalAccuracy()));
			        	
			        	//auto zoom to point
			        	map.getAction().setCenterAndZoom(point);
			        	
			        	//add the point with accuracy circle
			        	map.getModel().add(point);
			        	
			        	
			        }else{
			        	if (_ops != null){
			        		double lat = Double.parseDouble((String) _ops.getField("lat"));
			        		double lon = Double.parseDouble((String) _ops.getField("lon"));
			        		int zoom = Integer.parseInt((String) _ops.getField("zoom"));
			        		//get default location from options
			        		map.getAction().setCenterAndZoom( new MapPoint( lat, lon ), zoom );
			        	}else{
			        		//default location (ottawa with high zoom level)
			        		map.getAction().setCenterAndZoom( new MapPoint( 45.417, -75.7 ), 10 );
			        	}
			        }
				}
				catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		}.start();
        
        
        //enable selection mode
        map.getAction().enableOperationMode( MapConstants.MODE_SELECTION );
    }
    
    //Recursive function to attempt a geolocation fix.
    //numTries is the number of tries to ge tthe location. once it reaches 0 we have to fail
    private BlackBerryLocation getGeolocation(int numTries){
    	if (numTries == 0) return null;
    	
    	//get current location
        BlackBerryCriteria myCriteria = new BlackBerryCriteria(LocationInfo.GEOLOCATION_MODE);
        BlackBerryLocation myLocation = null;
        try {
			BlackBerryLocationProvider myProvider = (BlackBerryLocationProvider)
			LocationProvider.getInstance(myCriteria);
			myLocation = (BlackBerryLocation)myProvider.getLocation(-1);
			if (myLocation.isValid()) return myLocation;
			else return getGeolocation(numTries-1);
		} catch (LocationException e) {
			//possible no towers found
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;	//unable to get location if we get here.
    }
    
    // FieldChangeListener implementation.
    public void fieldChanged( final Field field, final int context ) {
        if ( field == map ) {
            switch ( context ) {
            case MapAction.ACTION_SELECTED_MAPPABLE_CHANGE:
                handleSelection( map.getSelectionPoint() );
                break;
            default:
                // Unhandled event. See MapAction.ACTION_* for other events.
            }
        }
    }
    
    // Just add the selection point to the map data model as a MapMarker (represented
    // as a pin).
    private void handleSelection( final MapPoint selection ) {
    	map.getModel().removeAll();
        map.getModel().add( new MapMarker( selection.getLat(), selection.getLon() ) );
        currSelection = selection;
    }
    
    private void makeCallback(){
    
    	String result = "";
    	
    	JSONObject j = new JSONObject();
    	try {
			j.put("latitude", Double.toString(currSelection.getLat()));
			j.put("longitude", Double.toString(currSelection.getLon()));
			result = j.toString();
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}catch (Exception e){
			
			
		}
    	
		
			
    	// Create a final thread safe result to pass into the thread object
		final Object[] threadedResult = new Object[] {result};

		// Create a new thread to make sure that the invoke of the JavaScript callback
		// does not initiate from the UI thread.  This can otherwise cause a deadlock scenario
		new Thread () {
			public void run() {
				try
				{
					// Pass the result of the picker back to the handle of the JavaScript callback
					_callback.invoke(_callback, threadedResult);
				}
				catch (Exception e) {
					throw new RuntimeException(e.getMessage());
				}
			}
		}.start();
    }
    
    public boolean onClose(){
    	makeCallback();
    	this.close();
		return true;
    }

}
