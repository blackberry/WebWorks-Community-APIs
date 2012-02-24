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
// GET STARTED BY COMPLETING ALL OF THE 'STEP' INSTRUCTIONS LISTED BELOW.
//
// 1. Namespace
// 2. Class name
// 3. Code
//
//  Consider contributing your extension to the BlackBerry WebWorks Community APIs:
//	   https://github.com/blackberry/WebWorks-Community-APIs
//
//------------------------------------------------------------------------------------


/**
*  A ScriptableFunction class is where the code for your logic is defined.
*
*	The invoke method is called by the extension through the namespace class.
*
*  All communication between the J2ME and JavaScript is done using String variables.
*
*/


//
//STEP 1: Choose a unique package name that describes your extension.
//        Use the same package name in all classes that define this extension.
//
//		  e.g. "my.extension" or "companyname.description"
//
package webworks.template;

import net.rim.device.api.script.ScriptableFunction;

//
//STEP 2: Rename the following class to describe your function.
//
//		e.g AddFunction or BlinkLEDfunction
//
public final class TemplateFunction extends ScriptableFunction {

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
		System.out.println("TemplateFunction.invoke");
		Object result = Boolean.FALSE;

//
//STEP 3: Implement your function.
//

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
