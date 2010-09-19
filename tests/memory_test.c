#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cut.h"
#include <claro/base.h>

void __CUT_BRINGUP__MemoryTest( void ) {
    claro_base_init();
}


void __CUT__Memory_usage_test()
{
    void *mem1 = NULL;
    void *should_be_zeroes = NULL;
    int is_zeroes[10] = {0};
    char *astring = NULL;
    char *dupme = "I'm duped";
    
    mem1 = smalloc(300);
    ASSERT(mem1 != NULL, "failed to smalloc");
    
    should_be_zeroes = scalloc(10, sizeof(int));
    ASSERT_EQUALS(memcmp(should_be_zeroes, is_zeroes, 10 * sizeof(int)), 0, "calloc didn't return 0 mem");
    
    free(mem1);
    
    mem1 = srealloc(should_be_zeroes, 200);
    ASSERT(mem1 != NULL, "failed to realloc");
    
    astring = sstrdup (dupme);
    ASSERT(astring != NULL, "failed to strdup");
    ASSERT_EQUALS(strlen(astring), strlen(dupme), "duped strings different lengths");
    ASSERT_EQUALS(strcmp(astring, dupme), 0, "duped string not equal");
    
    free(astring);
    free(mem1);
}



void __CUT_TAKEDOWN__MemoryTest( void ) {
}
