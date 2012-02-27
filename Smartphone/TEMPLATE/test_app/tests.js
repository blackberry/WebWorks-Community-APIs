/*
 * Copyright 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// documentation on writing tests here: http://docs.jquery.com/QUnit
// example tests: https://github.com/jquery/qunit/blob/master/test/same.js
 
//Test Setup:
//	module(name) : Separate tests into modules
//	expect(amount) : Specify how many assertions are expected to run within a test.
//	test(name, expected, test) : Add a test to run

//Assertions:
//	ok(state, message)
//	equal(actual, expected, message)
//  strictEqual(actual, expected, message)
//  raises(block, expected, message)

module("blackberry.template");	

test("object", function(){
	expect(2);

	//Setup test:
	var typeofObjA, typeofObjB;

	try {
		typeofObjA = (typeof blackberry);
		typeofObjB = (typeof blackberry.template);
	} catch(e) {
		console.log("blackberry object not found");
	}
	
	//Run the test
	equals(typeofObjA, "object", "'blackberry' JavaScript object exists in ScriptEngine.");
	equals(typeofObjB, "object", "'blackberry.template' JavaScript object exists in ScriptEngine.");
	//Test teardown:
  
})

test("GET properties", function(){

	//Setup test:
	
	//Run the test
	expect(3);
	equals(blackberry.template.bool, false, "blackberry.template.bool initial value");
	equals(blackberry.template.string, "hello", "blackberry.template.string initial value.");
	equals(blackberry.template.integer, 0, "blackberry.template.integer initial value.");
	//Test teardown:
  
})

test("SET properties", function(){
	expect(3);

	//Setup test:
	var newBool = true, newString = "world", newInt = 42;

	blackberry.template.bool = newBool;
	blackberry.template.string = newString;
	blackberry.template.integer = newInt;
	
	//Run the test
	equals(blackberry.template.bool, newBool, "blackberry.template.bool changed");
	equals(blackberry.template.string, newString, "blackberry.template.string changed.");
	equals(blackberry.template.integer, newInt, "blackberry.template.integer changed.");
	//Test teardown:
  
})

test("functions", function(){
	expect(3);

	//Setup test:
	var valueA = 5, valueB = 6;
	
	//Run the test
	equals(blackberry.template.add(valueA, valueB), 11, "blackberry.template.add correct");
	equals(blackberry.template.add(valueA+1, valueB+1), 13, "blackberry.template.add correct");
	equals(blackberry.template.log("test"), undefined, "blackberry.template.log correct");
	//Test teardown:
  
})
