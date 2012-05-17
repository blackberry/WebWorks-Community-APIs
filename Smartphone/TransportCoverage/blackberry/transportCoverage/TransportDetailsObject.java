package blackberry.transportCoverage;

import net.rim.device.api.script.Scriptable;

public class TransportDetailsObject extends Scriptable {
		
		public static final String FIELD_TRANSPORT = "transport";
	    public static final String FIELD_AVAILABLE = "available";
	    public static final String FIELD_COVERAGE = "coverage";
	    
		public String transport = "";
		public Boolean available = new Boolean(false);
		public Boolean coverage = new Boolean(false);
		public String rStr = "";
		
		/**
	     * @see net.rim.device.api.script.Scriptable#getField(java.lang.String)
	     */
	    public Object getField( final String name ) throws Exception {
	        if( name.equals( FIELD_TRANSPORT ) ) {
	            return transport;
	        } else if( name.equals( FIELD_AVAILABLE ) ) {
	            return available;
	        }else if( name.equals( FIELD_COVERAGE ) ) {
	            return coverage;
	        }

	        return super.getField( name );
	    }
	
}
