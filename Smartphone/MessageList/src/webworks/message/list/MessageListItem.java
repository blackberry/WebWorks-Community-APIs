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

package webworks.message.list;

import net.rim.device.api.script.Scriptable;

public class MessageListItem extends Scriptable
{
	public static final String FIELD_ID = "id";
	public static final String FIELD_TITLE = "title";
	public static final String FIELD_DESCRIPTION = "description";
	public static final String FIELD_IMAGENEW = "imageNew";
	public static final String FIELD_IMAGEREAD = "imageRead";
	public static final String FIELD_IMAGEDELETED = "imageDeleted";
	
	private String _id = null;
	private String _title = null;
	private String _description = null;
	private String _imageNew = null;
	private String _imageRead = null;
	private String _imageDeleted = null;
//	private CustomMessage _message = null;
	
    public Object getField(String name) throws Exception {
        if (name.equals(FIELD_ID)) {
            return new String(_id);	  
        }
		else if (name.equals(FIELD_TITLE)) {
            return new String(_title);	  
        }
		else if (name.equals(FIELD_DESCRIPTION)) {
            return new String(_description);	  
        }
		else if (name.equals(FIELD_IMAGENEW)) {
            return new String(_imageNew);	  
        }
		else if (name.equals(FIELD_IMAGEREAD)) {
            return new String(_imageRead);	  
        }
		else if (name.equals(FIELD_IMAGEDELETED)) {
            return new String(_imageDeleted);	  
        }

        return super.getField(name);
    }

    public boolean putField(String field, Object value) throws Exception {
	
		if (field.equals(FIELD_ID)) {
           _id = (String)value;	  
        }
		else if (field.equals(FIELD_TITLE)) {
            _title = (String)value;	  
        }
		else if (field.equals(FIELD_DESCRIPTION)) {
            _description = (String)value;	  
        }
		else if (field.equals(FIELD_IMAGENEW)) {
            _imageNew = (String)value;	
        }
		else if (field.equals(FIELD_IMAGEREAD)) {
           _imageRead = (String)value;	  
        }
		else if (field.equals(FIELD_IMAGEDELETED)) {
            _imageDeleted = (String)value;	
        }
	
		return super.putField(field, value);
    }
	
}
