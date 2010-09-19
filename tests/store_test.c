#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cut.h"
#include <claro/base.h>

void __CUT_BRINGUP__StoreTest( void ) 
{
    
}


void __CUT__Store_base()
{
    long test = 10;
    long *test_val = NULL;
    size_t dlen;
    int type;
    
    store_put("test1", &test, sizeof(test), CL_TYPE_INT);
    ASSERT_EQUALS(store_has("test1"), 1, "Not found in store");
    
    test_val = store_get("test1", &dlen, &type);
    ASSERT_EQUALS(test, *test_val, "results not equal what was put in");
    ASSERT_EQUALS(type, CL_TYPE_INT, "result is not the correct type code");
    ASSERT_EQUALS(dlen, sizeof(test), "result is not the correct length");
    
    test_val = store_take("test1", &dlen, &type);
    ASSERT_EQUALS(test, *test_val, "results not equal what was put in");
    ASSERT_EQUALS(type, CL_TYPE_INT, "result is not the correct type code");
    ASSERT_EQUALS(dlen, sizeof(test), "result is not the correct length");
    ASSERT_EQUALS(store_has("test1"), 0, "store value taken but still in store");    
}


#define GEN_TEST_TYPE(tType, val) store_put_int("test" # tType, val);\
ASSERT_EQUALS(val, store_get_int("test" # tType), "results for put/get " #tType " failed");\
ASSERT_EQUALS(val, store_take_int("test" # tType), "results for take " #tType " failed");\
ASSERT_EQUALS(store_has("test" # tType), 0, "taken but still in store");


void __CUT__Store_types()
{
    
    GEN_TEST_TYPE(int, 25);
    GEN_TEST_TYPE(float, 25.0f);
    GEN_TEST_TYPE(long, 25L);
    GEN_TEST_TYPE(double, 25.0);

    size_t length;
    store_put_string("teststring", "the string", strlen("the string"));
    ASSERT(store_get_string("teststring", &length) != NULL, "didn't get the right string");
    ASSERT(store_take_string("teststring", &length) != NULL, "results for take string failed");
    ASSERT_EQUALS(store_has("teststring"), 0, "taken but still in store");
}


void __CUT_TAKEDOWN__StoreTest( void ) 
{
    
}
