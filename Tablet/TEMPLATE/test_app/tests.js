
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
	"use strict";
	
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
  
});

test("GET properties", function(){
	"use strict";
	
	expect(3);

	//Setup test:
	
	//Run the test
	equals(blackberry.template.bool, false, "blackberry.template.bool initial value");
	equals(blackberry.template.string, "hello", "blackberry.template.string initial value.");
	equals(blackberry.template.integer, 0, "blackberry.template.integer initial value.");
	//Test teardown:
  
});

test("SET properties", function(){
	"use strict";
	
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
  
});

test("functions", function(){
	"use strict";
	
	expect(5);

	//Setup test:
	var valueA = 5, valueB = 6;
	
	//Run the tests
	equals((typeof blackberry.template.add), "function", "blackberry.template.add function exists");
	equals((typeof blackberry.template.log), "function", "blackberry.template.log function exists");
	
	valueA = blackberry.template.add(5, 6);
	equals(valueA, 11, "blackberry.template.add correct");
	valueB = blackberry.template.add(valueA+1, 1);
	equals(valueB, 13, "blackberry.template.add correct");
	equals(blackberry.template.log("test"), undefined, "blackberry.template.log correct");
	//Test teardown:
  
}); 


//
//Because callbacks happen asyncronously, we must use an asyncTest in order for QUnit to lock the tread.
//	Otherwise what appears to happen is the ActionScript function never appears to be called. However, it
//	actually does and the callback occurs after this JavaScript thread has finished evaluating all unit tests.
//
asyncTest("callback", function(){
	"use strict";
	
	expect(4);

	//Setup test:
	var result;

	//Run the test:
	equals((typeof blackberry.template.onEvent), "function", "blackberry.template.onEvent function exists");
	
	result = blackberry.template.onEvent(function (eventType) {
				ok(true, "blackberry.template.onEvent complete");
				equals(eventType, "Event type = custom", "blackberry.template.onEvent expected parameter okay");
			});
	
	equals(result, undefined, "blackberry.template.onEvent returns no value");

	//Test teardown:

	
	//Begin the asyncronous test
	start();
  
});
