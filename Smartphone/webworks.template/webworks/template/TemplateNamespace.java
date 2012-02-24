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
package webworks.template;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;


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



	private boolean _boolean = false;
	private String  _string  = "hello";
	private int     _integer = 0;
	

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
        super();
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

//
//STEP 4: Replace the following section with any properties or functions that 
//        will be supported in your extension.
//
		//
		// Properties:
		//
		// Must always return an Object, never a primitive value.
		//    e.g. "new Boolean(Boolean.FALSE)" instead of "false"
		//
		if (name.equals(PROPERTY_BOOLEAN)) 
		{
			return new Boolean(_boolean);
		}
		else if (name.equals(PROPERTY_STRING))
		{
			return new String(_string);
		}
		else if (name.equals(PROPERTY_INTEGER))
		{
			return new Integer(_integer);
		}
		//
		// Functions:
		//
		// The following demonstrates how to support method or function calls within an  
		// extension. Create and return a ScriptableFunction object.  The implementation 
		// for this object will handle the logic of the function, and will receive 
		// parameters passed in by the script engine.  The ScriptableFunction object 
		// will return an Object representing the result of your function (can return 
		// UNDEFINED if there are no results).
		//
		if (name.equals(FUNCTION_ADD)) 
		{
			return new TemplateFunction();
		} 
		else if (name.equals(FUNCTION_LOG)) 
		{
			return new VoidFunction();
		}

		//
		//If the name is not recognized, send it to the super class to be handled.
		//
		return super.getField(name);
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
		
//
//STEP 5: Replace the following to set any properties you will be supporting in your extension.
//
		//
		// Properties:
		//		
		if (name.equals(PROPERTY_BOOLEAN)) 
		{
			_boolean = value.toString().toLowerCase().equals("true");
			return true;
		}
		else if (name.equals(PROPERTY_STRING))
		{
			_string = value.toString();
			return true;
		}
		else if (name.equals(PROPERTY_INTEGER))
		{
			_integer = Integer.parseInt(value.toString());
			return true;
		}
		
		//If the name is not recognized, send it to the super class to be handled.
		//
		return super.putField(name, value);
	}


	//used to demonstrate how to implement a function with no results.
	public class VoidFunction extends ScriptableFunction {

		public Object invoke(Object obj, Object[] args) throws Exception {
			System.out.println("VoidFunction - log function called.");
			return UNDEFINED;
		}
	}
	
}
