package webworks.notification;


import net.rim.blackberry.api.messagelist.ApplicationIndicator;
import net.rim.blackberry.api.messagelist.ApplicationIndicatorRegistry;
import net.rim.device.api.notification.NotificationsConstants;
import net.rim.device.api.notification.NotificationsManager;
import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.util.StringUtilities;

public class NotificationNamespace  extends Scriptable
{
	 public static final String FIELD_REGISTER = "register";
	 public static final String FIELD_FIRE = "fire";
	 public static final String FIELD_CANCEL = "cancel";
	 public static final String FIELD_UNREGISTER = "unregister";
	 
	 
	
	 public NotificationNamespace()
	 {

	 }
	
   public Object getField(String name) throws Exception
	 {
		  if (name.equals(FIELD_REGISTER))
		  {
			   return new registerNotificationObjects();			
		  }
		  else if (name.equals(FIELD_FIRE))
		  {
			   return new fireEvent();			
		  }
		  else if (name.equals(FIELD_CANCEL))
		  {
			   return new cancelEvent();			
		  }
		  else if (name.equals(FIELD_UNREGISTER))
		  {
			   return new unregisterNotificationObjects();			
		  }
		  return super.getField(name);
	 }	
	
	
   public class registerNotificationObjects  extends ScriptableFunction
   {
      public static final String NAME = "register";
      public Object invoke(Object obj, Object[] args) throws Exception
		  {
			   if (args != null || args.length == 2)
			   {
		        try
		        {
               final long sourceID = StringUtilities.stringHashToLong(args[0].toString());		        	
					     final String notificationName =  args[1].toString();
		           NotificationsManager.registerSource( 
		            		sourceID, new Object()
                    {
                       public String toString()
                       {
                       	  return notificationName;
	                     }
	                  }
		                , NotificationsConstants.IMPORTANT );
		        }
		        catch (final Exception e)
		        {
		        	
		        }
		     }
			return UNDEFINED;
			}	       
   }
	
	 public class fireEvent  extends ScriptableFunction
	 {
      public static final String NAME = "fire";
      public Object invoke(Object obj, Object[] args) throws Exception
		  {
			if (args != null || args.length == 1)
			{
          try
          {
             final long sourceID = StringUtilities.stringHashToLong(args[0].toString());		        	
             NotificationsManager.triggerImmediateEvent( sourceID, 0, this, null );
          }
          catch (final Exception e)
          {
          	
          }
         }
		  return UNDEFINED;
      }
	 }	

	 public class cancelEvent  extends ScriptableFunction
	 {
      public static final String NAME = "cancel";
      public Object invoke(Object obj, Object[] args) throws Exception
		  {
			   if (args != null || args.length == 1)
			   {
            try
            {
               final long sourceID = StringUtilities.stringHashToLong(args[0].toString());		        	
        	     NotificationsManager.cancelImmediateEvent( sourceID, 0, this, null );
            }
            catch (final Exception e)
            {
            	
            }
           }
		     return UNDEFINED;
		  }
	 }
	 
	 public class unregisterNotificationObjects  extends ScriptableFunction
   {
      public static final String NAME = "unregister";
      public Object invoke(Object obj, Object[] args) throws Exception
		  {
			   if (args != null || args.length == 1)
			   {
		        try
		        {
               final long sourceID = StringUtilities.stringHashToLong(args[0].toString());		        	
		           NotificationsManager.deregisterSource(sourceID);
		        }
		        catch (final Exception e)
		        {
		        	
		        }
		     }
			return UNDEFINED;
			}	       
   }
}