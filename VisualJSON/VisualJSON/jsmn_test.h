// jsmn_test.h

#ifndef __jsmn_test_h__
#define __jsmn_test_h__

#include "jsmn.h"

/* Terminate current test with error */
#define fail()  return __LINE__

/* Successfull end of the test case */
#define done() return 0

/* Check single condition */
#define check(cond) do { if (!(cond)) fail(); } while (0)

/* Test runner */

#define TOKEN_EQ(t, tok_start, tok_end, tok_type) \
        ((t).start == tok_start \
         && (t).end == tok_end  \
         && (t).type == (tok_type))

#define TOKEN_STRING(js, t, s) \
        (strncmp(js+(t).start, s, (t).end - (t).start) == 0 \
         && strlen(s) == (t).end - (t).start)

//#define TOKEN_PRINT(t) \
        //printf( "start: %d, end: %d, type: %d, size: %d\n", \
         //               (t).start, (t).end, (t).type, (t).size)
void TOKEN_PRINT(jsmntok_t &t);

class JsmnTest
{
public:
	static int test_passed;
	static int test_failed;
	static void test(int(*func)(void), const char *name);
	static int test_empty();
	static int test_simple();
	static int miltest_objstring();
	static int test_primitive();
	static int test_string();
	static int test_partial_string();
	static int test_unquoted_keys();
	static int test_partial_array();
	static int test_array_nomem();
	static int test_objects_arrays();
	static int TestAll();
};

#endif