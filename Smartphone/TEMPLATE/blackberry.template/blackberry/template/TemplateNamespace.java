/*
 * Copyright 2010-2011 Research In Motion Limited.
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


//------------------------------------------------------------------------------------
//
// CREATE YOUR OWN BLACKBERRY WEBWORKS EXTENSION WITH THIS TEMPLATE.
// GET STARTED BY COMPLETING ALL OF THE 'STEP' INSTRUCTIONS LISTED BELOW:
//
// 1. Namepace
// 2. Class and constructor name
// 3. Define property and function names
// 4. Handle property/function calls in getField
// 5. Handle property calls in putField
//
//
//  Consider contributing your extension to the BlackBerry WebWorks Community APIs:
//	   https://github.com/blackberry/WebWorks-Community-APIs
//
//------------------------------------------------------------------------------------


/**
*  A namespace class is where the properties and functions for an extension are defined.
*
*  All communication between the J2ME and JavaScript is done using String variables.
*  The getField or putField methods in this namespace class are called by the extension and  
*  provided with the name of a property or function call being made from JavaScript.
*
* Example (JavaScript)                   (extension)
* webworks.sample.foo.add(4, 5)    -->   TemplateNamespace.getField("add")
*
*/


//
//STEP 1: Choose a unique package name that describes your extension.
//        Use the same package name in all classes that define this extension.
//
//		  e.g. "my.extension" or "companyname.description"
//
package blackberry.template;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;

import java.util.Enumeration;
import java.util.Hashtable;
import blackberry.core.ApplicationEventHandler;


//
//STEP 2: Rename this class and constructor to describe the primary purpose of this feature.
//		  e.g. BluetoothNamespace or CryptoNamespace
//
public class TemplateNamespace extends Scriptable 
{

//
//STEP 3: Replace the following with the names of any properties or functions that 
//		will be supported by this extension.
//
	public static final String PROPERTY_BOOLEAN = "bool";
	public static final String PROPERTY_STRING  = "string";
	public static final String PROPERTY_INTEGER = "integer";		
	public static final String FUNCTION_ADD     = "add";
	public static final String FUNCTION_LOG     = "log";
	public static final String FUNCTION_CALLBACK = "onEvent";

    private Hashtable _fields;

//
//STEP 2 (Continued): Rename this constructor to match the class name.
//
	/**
	 * Constructor: Initializes this namespace class.
	 *
	 *   TIP: When the extension class (main entry point to this extension) creates this
	 *         namespace, it can provide references to the active browserfield or 
	 *         config.xml objects.
	 *        
	 *        e.g. public TemplateNamespace(BrowserField bf) { ... }
	 */
	public TemplateNamespace()
	{
        _fields = new Hashtable();
//
//STEP 4: Replace the following with any properties or functions that 
//        will be supported in your extension.
//
        _fields.put( PROPERTY_BOOLEAN, new Boolean(false) );
        _fields.put( PROPERTY_STRING, new String("hello") );
        _fields.put( PROPERTY_INTEGER, new Integer(0) );

        _fields.put( FUNCTION_ADD, new ReturnFunction() );
        _fields.put( FUNCTION_LOG, new VoidFunction() );
        _fields.put( FUNCTION_CALLBACK, new CallbackFunction() );
        
		System.out.println("TemplateNamespace.constructor");
	}
	

	/**
	 *  Called when a WebWorks application tries to GET the value of a property or function.
	 *  This occurs when when the dot “.” extender is used on your JavaScript object.
	 *
	 *   e.g. retrieving a property value from this extension using JavaScript:
	 *         var bool = webworks.sample.foo.bool;		//false
	 *
	 *   e.g. calling a function from this extension using JavaScript:
	 *         var result = webworks.sample.foo.add(5, 4);	//9
	 *
	 * @param name  a JavaScript function or property name.
	 * @return result of the given property or function.
	 */	
	public Object getField(String name) throws Exception 
	{
		System.out.println("TemplateNamespace.getField(" + name + ")");
        
		Object field = _fields.get( name );
        if( field == null ) {
            return UNDEFINED;
        }
        return field;
	}

	/**
	 *  Called when WebWorks application tries to SET the value of a property.
	 *  This occurs when when the dot “.” extender is used on your JavaScript object.
	 *
	 * @param name - the name of the property being set (by the script engine).
	 * @param value - the value of the property being set.
	 * @return returns true if property has been changed, false otherwise.
	 */	
	public boolean putField(String name, Object value) throws Exception 
	{
		System.out.println("TemplateNamespace.putField(" + name + "), value=" + value);

		Object field = _fields.get( name );        
		if( field == null ) {
            return super.putField(name, value);
        }
		_fields.put(name, value);
		return true;	
	}

	
    /**
     * When dealing with event listeners and callbacks, the responsible thing to do
	 * is call any waiting events when this namespace is unloaded.
     * 
     * @throws Exception
     */
    public void unload() throws Exception {
        for( Enumeration e = _fields.elements(); e.hasMoreElements(); ) {

    //        OnAppEventFunction event = (OnAppEventFunction) e.nextElement();
    //        event.execute( null, new Object[] { null } );
	
        }
    }


//
//STEP 6: Implement any functions used by this extension.
//

	//
	//TIP: The following ScriptableFunction classes can be their own external files,
	//		as long as they share the same package name as this NameSpace class.
	//
	//		e.g. ReturnFunction.java, VoidFunction.java
	//
	
	//Used to demonstrate how to implement a function that returns results.
	public class ReturnFunction extends ScriptableFunction {

		/**
		* Invoked by the WidgetExtension when a JavaScript function call is made.
		*
		* @param obj   Context where this function was called.
		* @param args  0 or more parameters provided to the JavaScript function.
		*
		* @return value to be returned for the JavaScript function.
		*/
		public Object invoke(Object obj, Object[] args) throws Exception 
		{
			System.out.println("ReturnFunction.invoke");
			Object result = Boolean.FALSE;

			//
			//    i. Validate parameters supported by this function:
			//
			if (args.length == 2)
			{
				//
				//   ii. Read parameter values and cast to appropriate data types:
				// 		e.g.  boolean state = ((Boolean) args[0]).booleanValue();
				//		e.g.  String message = args[1].toString();
				//
				int first  = Integer.parseInt(args[0].toString());
				int second = Integer.parseInt(args[1].toString());

				//
				//  iii. Return an object that represents the result of the function:
				//	     Value must be an object, not a primative data type. 
				//		  e.g.  result = Boolean.TRUE;
				//		  e.g.  result = new String("success"); 
				//
				//  TIP: Return UNDEFINED if your method doesn't need to return any values.
				//		 e.g. result = UNDEFINED;
				//
				result = new Integer(first + second);
			} 


			return result;		// Returns false by default if invalid parameters received.
		}
		
	}	
	
	//used to demonstrate how to implement a function with no results.
	public class VoidFunction extends ScriptableFunction {

		public Object invoke(Object obj, Object[] args) throws Exception {
			if (args.length == 1)
			{
				String msg  = args[0].toString();
				System.out.println("VoidFunction - log : " + msg);
			}				
			return UNDEFINED;
		}
	}

	//used to demonstrate how to implement callback event functions that return parameters.
	public class CallbackFunction extends ScriptableFunction {
	
		/**
		* Invoked by the WidgetExtension when a JavaScript function call is made.
		*
		* @param obj   Context where this function was called.
		* @param args  0 or more parameters provided to the JavaScript function.
		*
		* @return value to be returned for the JavaScript function.
		*/
		public Object invoke(Object obj, Object[] args) throws Exception 
		{
			System.out.println("CallbackFunction.invoke");

			if (args.length == 1)
			{
				int eventId  = Integer.parseInt(args[0].toString());
				
				//TODO - DO THE ACTUAL CALLBACK?
				return new Integer(5);	//return 5 for now - so we can see this in the unit test.
			}
			
			return UNDEFINED;
		}
		
	}	
}
