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
package webworks.mail.attachment;

import java.io.DataInputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

import net.rim.blackberry.api.mail.Address;
import net.rim.blackberry.api.mail.Folder;
import net.rim.blackberry.api.mail.Message;
import net.rim.blackberry.api.mail.Multipart;
import net.rim.blackberry.api.mail.Session;
import net.rim.blackberry.api.mail.SupportedAttachmentPart;
import net.rim.blackberry.api.mail.TextBodyPart;
import net.rim.blackberry.api.mail.Transport;
import net.rim.device.api.lbs.picker.LocationPicker;
import net.rim.device.api.lbs.picker.MapsLocationPicker;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.gps.*;
import net.rim.device.api.io.IOUtilities;
import net.rim.device.api.system.Application;
import net.rim.device.api.ui.*;
import net.rim.device.api.ui.component.*;
import net.rim.device.api.ui.container.*;

public final class MailAttachmentFunction extends ScriptableFunction {
	
	public static final String NAME = "send";
	//private 
	
	public Object invoke(Object obj, Object[] args) throws Exception
	{
		//input = (email address, subject, body, full attachment path)
       	
		String email = (String)args[0].toString();
	    String subject = (String)args[1].toString();
	    String body = (String)args[2].toString();
	    String attachment = (String)args[3].toString();
	    String fName = attachment;
	    byte[] data = null;
	    FileConnection fconn = null;
	    DataInputStream is = null;
	    try{
	    fconn = (FileConnection) Connector.open(fName, Connector.READ_WRITE);
	    is = fconn.openDataInputStream();             
	    data = IOUtilities.streamToBytes(is);
    	    is.close();
    	    fconn.close();
	    } catch (Exception ex) {
	    	 Dialog.inform("Error in file path: " + ex.toString());
	    	 return new Boolean(false);
	    }
      //create a multipart
        Multipart mp = new Multipart();

        //create the file
        SupportedAttachmentPart sap = new SupportedAttachmentPart(mp,"application/x-example",attachment,data);

        TextBodyPart tbp = new TextBodyPart(mp, body);

        //add the file to the multipart
        mp.addBodyPart(tbp);
        mp.addBodyPart(sap);

        //create a message in the sent items folder
        Folder folders[] = Session.getDefaultInstance().getStore().list(Folder.SENT);

        Message message = new Message(folders[0]);
        Address toAdd = new Address(email,"my email");
        Address toAdds[] = new Address[1];
        toAdds[0] = toAdd;
        message.addRecipients(Message.RecipientType.TO, toAdds);
        message.setSubject(subject);
        message.setContent(mp);
        Session session = Session.getDefaultInstance();
        Transport trans = session.getTransport();
        //add recipients to the message and send
        boolean sent = false;
        try {
             trans.send(message);
             sent = true;
        } catch (Exception e) {
             Dialog.inform("Error while sending: " + e.toString());
        }
    

    return new Boolean(sent);
	}

	

}

