/*
* Copyright 2010-2012 Research In Motion Limited.
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

package webworks.pim.appointment.impl;


import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Application;

import java.util.Hashtable;
import java.util.Vector;

import javax.microedition.pim.Event;
import javax.microedition.pim.EventList;
import javax.microedition.pim.PIM;
import javax.microedition.pim.PIMException;
import javax.microedition.pim.PIMItem;
import javax.microedition.pim.PIMList;

import net.rim.blackberry.api.pdap.BlackBerryContact;
import net.rim.blackberry.api.pdap.BlackBerryEvent;
import net.rim.blackberry.api.pdap.BlackBerryPIMList;
import net.rim.blackberry.api.pdap.PIMListListener;

import blackberry.pim.appointment.AppointmentObject;


public class AptListenerManager {

	 private AptListener _listener;
     private Hashtable _listenerHash;
     private Application _appObject;
     private Vector _vec;
     
     public AptListenerManager() {
           _appObject = Application.getApplication();
           _listenerHash = new Hashtable();
     }
     
     
     public void startListening(String eventService, ScriptableFunction callback) {
           
           System.out.println("startListening called...");
          
        try
         {
       	 	//check if event name was passed in, otherwise use default
              if (!eventService.equalsIgnoreCase("")) {
                       
        	// if the eventService does not exist in the hashtable,
           // create a new vector and put them into the hashtable
                 if (_listenerHash.get(eventService)==null) {
                     
               	    _listener = new AptListener(eventService);  
               	  	_vec = new Vector();
                       _vec.addElement(callback);
                       _listenerHash.put(eventService, _vec );
                 }else{
                       _vec = (Vector)_listenerHash.get(eventService);
                       _vec.addElement(callback);
                       _listenerHash.put(eventService, _vec);
                 }// end if (_listenerHash.get(eventService)==null)

                     BlackBerryPIMList eventList = (BlackBerryPIMList)PIM.getInstance().openPIMList(PIM.EVENT_LIST, PIM.READ_WRITE);
					 eventList.addListener(new AptListener(eventService));
                      
            //          callback.invoke(null, new Object[] { "added listener" });
                             		  	
                                 
               }// end of if (!eventService.equalsIgnoreCase("")){
               else {
            //	   callback.invoke(null, new Object[] { "event service empty" });
             	  // if the eventService is NOT provided, i.e., eventService= ""
             	  if (_listenerHash.get(eventService)==null) {
                 	  _listener = new AptListener(eventService);  
                       _vec = new Vector();
                       _vec.addElement(callback);
                       _listenerHash.put(eventService, _vec );
                   }else{
                        _vec = (Vector)_listenerHash.get(eventService);
                        _vec.addElement(callback);
                        _listenerHash.put(eventService, _vec);
                   }
             	 BlackBerryPIMList eventList = (BlackBerryPIMList)PIM.getInstance().openPIMList(PIM.EVENT_LIST, PIM.READ_WRITE);
               	 eventList.addListener(_listener);
               }    
        	
         }catch (Exception ex){
        	 try{
        	   callback.invoke(null, new Object[] {ex.getMessage()});
           
        	 }catch (Exception e){}
         }
         }//end start listening
     
     

     public void stopListening(String eventService, ScriptableFunction callback) {
          // System.out.println("stopListening called...");
          
           //Check eventService exists in our service-callback vector
           if(_listenerHash.containsKey(eventService)){
           	Vector vCBack = (Vector)_listenerHash.get(eventService);
           	 for(int i=0; i < vCBack.size(); i++){
                    ScriptableFunction sf = (ScriptableFunction)vCBack.elementAt(i);
                    if(sf == callback){
                   	 vCBack.removeElementAt(i);
                   	 break; // removing the first callback only
                    }
           	 }
           	 
           	 //last callback was deleted
           	 if(vCBack.size() == 0)
           	 {
           		 _listenerHash.remove(eventService);
           	 }
           }
           else
           {
           	//do nothing
           }
           
           if(_listener == null) {
                 return;
           }
         
           try {
        	  
        	  BlackBerryPIMList eventList = (BlackBerryPIMList)PIM.getInstance().openPIMList(PIM.EVENT_LIST, PIM.READ_WRITE);
        	  eventList.removeListener(_listener);
          
          }catch (Exception ex){
          } finally  {
        	  
        	 if(!(_listener == null)){  _listener = null;} ;
        	  
          }
          
     }
     
     protected void notifyAptReceived(final String eService, final AppointmentObject msg){
         
         System.out.println("Firing callback...");
          
         // if the service does not exist in the hashtable,
         // we got a problem
         if (_listenerHash.get(eService)==null) {
              //problem
         }else{
               _vec = (Vector)_listenerHash.get(eService);
 
               for(int i=0; i < _vec.size(); i++){
                 final ScriptableFunction sf = (ScriptableFunction)_vec.elementAt(i);
                   _appObject.invokeLater(new Runnable() {
                       public void run() {
                             try {
                                 
                                 sf.invoke(null, new Object[] { eService, msg });

                             } catch (Exception e) {
                                   System.out.println("Error invoking callback: " + e.getMessage());
                                   throw new RuntimeException("Error invoking callback: " + e.getMessage());
                             }
                       }
                       
                 });
               }
               
               
         }
        
   }
    
     public class AptListener implements PIMListListener {
   
         private String _serviceName;   
         
         public AptListener( String s ){
         	_serviceName = s;
         }
     
    	 public void itemAdded(final PIMItem item) {

    		try {
    				    if (item instanceof BlackBerryContact) {
	    		
    				    } else if (item instanceof BlackBerryEvent) {
    				    	 
    				    	Event event = (Event) item;
    				    	AppointmentObject ao = new AppointmentObject(event, null);
    				    	
    				    	notifyAptReceived(_serviceName, ao);
	    			
    				    } else {
	    					
	    					return;
    				    }
    				    
    				} catch (Exception e) {
    				}
    	 }
     
		public boolean isMeeting(BlackBerryEvent event) {
		    int count = event.countValues(BlackBerryEvent.ATTENDEES);
		    if (count > 0) {
		    	return true;
		    }
		    return false;
		}
     
     
     
     
     public void itemUpdated(final PIMItem oldItem, final PIMItem newItem) {

    			try {
				    if (newItem instanceof BlackBerryContact) {
    		
				    } else if (newItem instanceof BlackBerryEvent) {
				    	 
				    	Event event = (Event) newItem;
				    	AppointmentObject ao = new AppointmentObject(event, null);
				
    						notifyAptReceived(_serviceName, ao);
    				
				    } else {
    					
    					return;
				    }

    			   
    		} catch (Exception e) {
    			//System.out.println("Exception: ");
    	}
     }

    		
     
     
     public void itemRemoved(final PIMItem item) {
    		
    	 try {
    			    if (item instanceof BlackBerryContact) {
    			
    			    } else if (item instanceof BlackBerryEvent) {
    				
    			    Event event = (Event) item;
			    	AppointmentObject ao = new AppointmentObject(event, null);
    				
					notifyAptReceived(_serviceName, ao);
    			
    			    } else {
    				
    			    	return;
    			    }

    	} catch (Exception e) {
    	
    	}//end trycatch
     }//end itemRemoved
  }//end AptListener
}//end AptListenerManager

