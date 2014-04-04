#include <json/reader.h>
#include <json/writer.h>

// About --------------------------------------------------
// state2json is a helper and header only library created
// for exposing state to unit tests.
//
// Compilation --------------------------------------------
// When compiling the macro S2J_ENABLED must be set true.
// Doing this with GCC means passing it during compilation 
// through the -D flag.
// Example:
//     gcc helloworld.c -o hello -D S2J_ENABLED
//
// Usage --------------------------------------------------
// You must call s2jInit() with the json value into which
// s2j should save the state.
// Example:
//     json::Value ReturnJson;
//     s2jInit(ReturnJson);
//
// API ----------------------------------------------------
// The following helper functions are provided through
// s2j:
//     s2jSet(FIELD, VAL) // Set field to exact val
//     s2jIncre(FIELD) // Increment field
//     s2jDecre(FIELD) // Decrement field
//     s2jInsert(FIELD, VAL) // Insert val into list field
//
// Requirements -------------------------------------------
// This library depends on the jsoncpp library. The bb10
// native sdk provides this dependency.


// Internal implementation details ------------------------
// This abuses the compiler into warning of our
// api requirements. The user must call s2jInit()
// else we will not know where the state should be
// saved into.
#define s2jVALUE _s2jPlease_call_s2jInit_in_scope
#ifndef S2J_ENABLED
#define S2J_ENABLED 0
#endif
#define s2jBEGIN do { if (S2J_ENABLED) {
#define s2jEND } } while(0)
#define s2jF(FIELDTOACCESS) (*s2jVALUE)[FIELDTOACCESS]

// APIs ---------------------------------------------------

// Must call this before saving any state
#define s2jInit(REALVAL) \
	Json::Value *s2jVALUE = &REALVAL;

// Sets field to given value
#define s2jSet(FIELD, VAL) \
	s2jBEGIN \
		s2jF(FIELD) = VAL; \
	s2jEND

// Increments a numberic state field
#define s2jIncre(FIELD) \
	s2jBEGIN \
		int x = (*s2jVALUE).get(FIELD, (int) 0).asInt(); \
		x++; \
		s2jF(FIELD) = x; \
	s2jEND

// Decrements a numberic state field
#define s2jDecre(FIELD) \
	s2jBEGIN \
		int x = (*s2jVALUE).get(FIELD, (int) 0).asInt(); \
		x--; \
		s2jF(FIELD) = x; \
	s2jEND

// Insert value into list field
#define s2jInsert(FIELD, VAL) \
	s2jBEGIN \
		s2jF(FIELD).append(Json::Value(VAL)); \
	s2jEND
