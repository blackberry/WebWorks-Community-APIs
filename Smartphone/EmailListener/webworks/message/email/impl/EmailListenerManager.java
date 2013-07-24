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


package webworks.message.email.impl;

import net.rim.blackberry.api.mail.BodyPart;
import net.rim.blackberry.api.mail.Folder;
import net.rim.blackberry.api.mail.Message;
import net.rim.blackberry.api.mail.MessagingException;
import net.rim.blackberry.api.mail.Multipart;
import net.rim.blackberry.api.mail.ServiceConfiguration;
import net.rim.blackberry.api.mail.Session;
import net.rim.blackberry.api.mail.Store;
import net.rim.blackberry.api.mail.Transport;
import net.rim.blackberry.api.mail.BodyPart.ContentType;
import net.rim.blackberry.api.mail.event.FolderEvent;
import net.rim.blackberry.api.mail.event.FolderListener;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.servicebook.ServiceBook;
import net.rim.device.api.servicebook.ServiceRecord;
import net.rim.device.api.system.Application;

import java.util.Hashtable;
import java.util.Vector;

import blackberry.messaging.message.MessageObject;

public class EmailListenerManager {
      
      private EmailListener _listener;
      private Hashtable _listenerHash;
      private Application _appObject;
      private Vector _vec;
      
      public EmailListenerManager() {
            _appObject = Application.getApplication();
            _listenerHash = new Hashtable();
      }
      
      
      public void startListening(String emailService, ScriptableFunction callback) {
            
            System.out.println("startListening called...");
           
         try
          {
        	 	//check if email service name was passed in, otherwise use default
               if (!emailService.equalsIgnoreCase("")) {
                        
                  ServiceBook sb = ServiceBook.getSB();
                  ServiceRecord[] records = sb.findRecordsByCid("CMIME");
                                    
                  if (records != null)
                  {
                      for (int i = 0; i < records.length; ++i)
                      {
                          ServiceRecord record = records[i];
                                      
                          if (record.isValid())
                            { 
                              String name = record.getName();
                           
                              if (name.equalsIgnoreCase(emailService)) 
                              { 
                                    ServiceConfiguration sc = new ServiceConfiguration(record);
                              
                                    if (sc != null)
                                    {
                                        Session session = Session.getInstance(sc);
                                        
                                        if (session != null)
                                        {
                                          // if the service does not exist in the hashtable,
                                          // create a new vector and put them into the hashtable
                                          if (_listenerHash.get(emailService)==null) {
                                              
                                        	    _listener = new EmailListener(emailService);  
                                        	  	_vec = new Vector();
                                                _vec.addElement(callback);
                                                _listenerHash.put(emailService, _vec );
                                          }else{
                                                _vec = (Vector)_listenerHash.get(emailService);
                                                _vec.addElement(callback);
                                                _listenerHash.put(emailService, _vec);
                                          }// end if (_listenerHash.get(emailService)==null)

	                                          Store store = session.getStore();
	                                          Folder[] folders = store.list(Folder.INBOX);
	                                          Folder inbox = folders[0];
	                                          inbox.addFolderListener(_listener);
	                                          
	                                         // callback.invoke(null, new Object[] { "added listener" });
                              		  	}// end if (session != null)
                                    }//end if (sc != null)

                                }//end of service names compare
                            }//end of if valid records were found on device
                        }// end of for loop
                    }// end of if (records != null)
                }// end of if (!emailService.equalsIgnoreCase("")){
                else {
              	  // if the emailService is NOT provided, i.e., emailService= ""
              	  if (_listenerHash.get(emailService)==null) {
                  	  _listener = new EmailListener(emailService);  
                        _vec = new Vector();
                        _vec.addElement(callback);
                        _listenerHash.put(emailService, _vec );
                    }else{
                         _vec = (Vector)_listenerHash.get(emailService);
                         _vec.addElement(callback);
                         _listenerHash.put(emailService, _vec);
                    }
              	  Session.getDefaultInstance().getStore().addFolderListener(_listener);
                }    
            }catch (Exception ex){
            }
      }//end start listening
      
      

      public void stopListening(String emailService, ScriptableFunction callback) {
            System.out.println("stopListening called...");
            
            //Check emailService exists in our service-callback vector
            if(_listenerHash.containsKey(emailService)){
            	Vector vCBack = (Vector)_listenerHash.get(emailService);
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
            		 _listenerHash.remove(emailService);
            	 }
            }
            else
            {
            	//do nothing
            }
            
            if(_listener == null) {
                  return;
            }
            
            Session.getDefaultInstance().getStore().removeFolderListener(_listener);
            //Dump the old listener
            _listener = null;
      }
      
      protected void notifyEmailReceived(final String eService, final MessageObject msg){
          
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

            
      private class EmailListener implements FolderListener {

       //     private ServiceRecord _serviceRecord;
         private String _serviceName;   
            
            public EmailListener( String s ){
            	_serviceName = s;
            }
            
            public void messagesAdded(FolderEvent e) {
                System.out.println("### Message added! ###");

                if(e.getType() == FolderEvent.MESSAGE_ADDED)
                {
                  Message msg = e.getMessage();
                      try {
                                  String htmlContent = "";
                                  MessageObject msgObject = new MessageObject(msg, null );
                                  //This can be Multipart if it has an attachment or MimeBodyPart if it doesn't.
                                  Object content = msg.getContent();
                                  if(content instanceof BodyPart) {
                                        htmlContent = getHtmlContent((BodyPart)content);
                                        msgObject.putField(MessageObject.FIELD_BODY, htmlContent);
                                  }
                                  notifyEmailReceived(_serviceName, msgObject);
                            } catch (Exception e1) {
                                  System.out.println(e1.getMessage());
                            }
                }
                
          }

            

            private String getHtmlContent(BodyPart content) {
                  String html = "";
                  
                  //Have to check the content type string because the API doesn't properly parse the mime type for the isMimeType call.
                  //If type is text/html we found an html part. Convert and return it.
                  String contentType = content.getContentType();
                  if(contentType instanceof String) {
                        if(contentType.startsWith(ContentType.TYPE_TEXT_HTML_STRING)) {
                              downloadAllData(content);
                              
                              html = processRawBodyPart(content);
                        } 
                        //If it's a multipart container, we need to iterate and recurse over its parts to extract nested html content
                        else if(contentType.startsWith(ContentType.TYPE_MULTIPART_ALTERNATIVE_STRING)) {
                              Multipart mp = (Multipart) content.getContent();
                              //Iterate over parts and call this function recursively
                              for(int i = 0; i < mp.getCount(); i++) {
                                    html += getHtmlContent(mp.getBodyPart(i));
                              }
                        }
                  }
                  
                  return html;
            }
            
            private String processRawBodyPart(BodyPart bp) {
                  Object rawContent = bp.getContent();
                  String contentString = "";
                  
                  if(rawContent instanceof String) {
                        contentString += (String)rawContent;
                  } else if(rawContent instanceof byte[]) {
                        contentString += new String((byte[])bp.getContent());
                  }
                  
                  return contentString;
            }

            private void downloadAllData(BodyPart bp) {
                  if(bp.hasMore() && !bp.moreRequestSent()) {
                        try {
                              Transport.more(bp, true);
                        } catch (MessagingException e) {
                              System.out.println(e.getMessage());
                        }
                  }
            }
            
            
            public void messagesRemoved(FolderEvent e) {
                  return; //nothing to implement here
            }
            
            
      }
}
