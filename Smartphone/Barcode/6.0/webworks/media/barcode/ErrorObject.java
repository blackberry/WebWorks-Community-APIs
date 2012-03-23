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

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import net.rim.device.api.script.Scriptable;

public class ErrorObject extends Scriptable {

	    public static final String CODE = "code";
	    public static final String MESSAGE = "message";

	    private Hashtable _fields;

	    /**
	     * Constructor
	     */
	    public ErrorObject(int code, String message) {
	        _fields = new Hashtable();
	        _fields.put( MESSAGE, new String( message ) );
	        _fields.put( CODE, new Integer( code ) );
	    }

	    /**
	     * @see net.rim.device.api.script.Scriptable#enumerateFields(Vector)
	     */
	    public void enumerateFields( Vector v ) {
	        if( !_fields.isEmpty() ) {
	            for( Enumeration e = _fields.keys(); e.hasMoreElements(); ) {
	                v.addElement( e.nextElement() );
	            }
	        }
	    }

	    /**
	     * see net.rim.device.api.script.Scriptable#getElementCount()
	     */
	    public int getElementCount() {
	        return _fields.size();
	    }

	    /**
	     * @see net.rim.device.api.script.Scriptable#getField(String)
	     */
	    public Object getField( String fieldName ) throws Exception {
	        Object field = _fields.get( fieldName );
	        if( field == null ) {
	            return super.getField( fieldName );
	        }
	        return field;
	    }

}
