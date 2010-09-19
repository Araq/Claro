#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cut.h"
#include <claro/base.h>

static int event_func_ran;

void event_func_tester(object_t *object, event_t *event) {
    event_func_ran = 1;
}


void __CUT_BRINGUP__ObjectTest( void ) {
    claro_base_init();
    event_func_ran = 0;
}


void __CUT__Object_usage_test()
{
    /*
    object_t *parent;
    object_t *child;
    object_t *attached;
    
    parent = object_create(NULL, 30, "test object");
    ASSERT(parent != NULL, "failed to create object");
    
    child = object_create(parent, 30, "test child");
    ASSERT(child != NULL, "failed to create child");
    
    attached = object_create(NULL, 30, "test attaching child");
    ASSERT(attached != NULL, "failed to make child to attach");
    object_set_parent(attached, parent);
    
    object_addhandler(parent, "test", event_func_tester);
    event_send(parent, "test", "");
    ASSERT(event_func_ran, "event didn't fire");
    
    object_destroy(parent);
    */
}



void __CUT_TAKEDOWN__ObjectTest( void ) {
}
