
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cut.h"
#include <claro/base.h>

void __CUT_BRINGUP__ListTest( void ) {
    // this calls list_init
    claro_base_init();
}


void __CUT__List_usage_test()
{
    list_t list;
    list_t newlist;
    node_t *node = NULL;
    node_t *found = NULL;
    int data = 12;
    
    list_create(&list);
    list_create(&newlist);
    node = node_create();
    
    ASSERT(node != NULL, "failed to create node");
    node_add(&data, node, &list);
    
    // try to find it
    found = node_find(&data, &list);
    ASSERT(found != NULL, "failed to find node");
    ASSERT(found == node, "found node is not the same");
    
    // move it to another list
    node_move(found, &list, &newlist);
    found = node_find(&data, &newlist);
    ASSERT(found != NULL, "failed to find node after move");
    ASSERT(found == node, "found node is not the same after move");

    ASSERT_EQUALS(LIST_LENGTH(&newlist), 1, "list has wrong length");
    
    // delete it and then make sure it's gone
    node_del(found, &newlist);
    found = node_find(&data, &newlist);
    ASSERT(found == NULL, "deleted node still in newlist");

    ASSERT_EQUALS(LIST_LENGTH(&newlist), 0, "list has wrong length");
    ASSERT_EQUALS(LIST_LENGTH(&list), 0, "list has wrong length");
}



void __CUT_TAKEDOWN__ListTest( void ) {
}
