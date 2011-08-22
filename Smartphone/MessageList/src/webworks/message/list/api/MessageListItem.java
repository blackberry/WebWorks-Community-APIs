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

package webworks.message.list.api;

import webworks.message.list.model.CustomMessage;
import net.rim.device.api.script.Scriptable;

public class MessageListItem extends Scriptable
{
	public static final String FIELD_ID = "id";
	public static final String FIELD_TITLE = "title";
	public static final String FIELD_DESCRIPTION = "description";
	public static final String FIELD_IMAGENEW = "imageNew";
	public static final String FIELD_IMAGEREAD = "imageRead";
	
	private String _id = null;
	private String _title = null;
	private String _description = null;
	private String _imageNew = null;
	private String _imageRead = null;
	
    public MessageListItem() {
		this(null,null,null,null,null);
	}
	
    public MessageListItem(CustomMessage message) {
    	this(message.getId(), message.getContact(), message.getSubject(), message.getImageNew(), message.getImageRead());
    }

	public MessageListItem(String _id, String _title, String _description,
			String _imageNew, String _imageRead) {
		super();
		this._id = _id;
		this._title = _title;
		this._description = _description;
		this._imageNew = _imageNew;
		this._imageRead = _imageRead;
	}


	public Object getField(String name) throws Exception {
        if (FIELD_ID.equals(name)) {
            return new String(_id);	  
        } else if (FIELD_TITLE.equals(name)) {
            return new String(_title);	  
        } else if (FIELD_DESCRIPTION.equals(name)) {
            return new String(_description);	  
        } else if (FIELD_IMAGENEW.equals(name)) {
            return new String(_imageNew);	  
        } else if (FIELD_IMAGEREAD.equals(name)) {
            return new String(_imageRead);	  
        }

        return super.getField(name);
    }



	public boolean putField(String field, Object value) throws Exception {
	
		if (FIELD_ID.equals(field)) {
           _id = (String)value;	  
        } else if (FIELD_TITLE.equals(field)) {
            _title = (String)value;	  
        } else if (FIELD_DESCRIPTION.equals(field)) {
            _description = (String)value;	  
        } else if (FIELD_IMAGENEW.equals(field)) {
            _imageNew = (String)value;	
        } else if (FIELD_IMAGEREAD.equals(field)) {
           _imageRead = (String)value;	  
        }
	
		return super.putField(field, value);
    }
	
}
