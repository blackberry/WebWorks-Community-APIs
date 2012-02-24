
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

module("webworks.template");	

test("object", function(){
	expect(2);

	//Setup test:
	var typeofObjA, typeofObjB;
	
	try {
		typeofObjA = (typeof webworks);
		typeofObjB = (typeof webworks.template);
	} catch(e) {
		console.log("webworks object not found");
	}
	
	//Run the test
	equals(typeofObjA, "object", "'webworks' JavaScript object exists in ScriptEngine.");
	equals(typeofObjB, "object", "'webworks.template' JavaScript object exists in ScriptEngine.");

	//Test teardown:
  
})

test("GET properties", function(){

	//Setup test:
	
	//Run the test
	expect(3);
	equals(webworks.template.bool, false, "webworks.template.bool initial value");
	equals(webworks.template.string, "hello", "webworks.template.string initial value.");
	equals(webworks.template.integer, 0, "webworks.template.integer initial value.");

	//Test teardown:
  
})

test("SET properties", function(){
	expect(3);

	//Setup test:
	var newBool = true, newString = "world", newInt = 42;
	
	webworks.template.bool = newBool;
	webworks.template.string = newString;
	webworks.template.integer = newInt;
	
	//Run the test
	equals(webworks.template.bool, newBool, "webworks.template.bool changed");
	equals(webworks.template.string, newString, "webworks.template.string changed.");
	equals(webworks.template.integer, newInt, "webworks.template.integer changed.");

	//Test teardown:
  
})

test("functions", function(){
	expect(3);

	//Setup test:
	var valueA = 5, valueB = 6;
	
	//Run the test
	equals(webworks.template.add(valueA, valueB), 11, "webworks.template.add correct");
	equals(webworks.template.add(valueA+1, valueB+1), 13, "webworks.template.add correct");
	equals(webworks.template.log(), undefined, "webworks.template.log correct");

	//Test teardown:
  
})
