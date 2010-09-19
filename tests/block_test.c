
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cut.h"
#include <claro/base.h>

void __CUT_BRINGUP__BaseBlockTest( void ) {
    claro_base_init();
}


void __CUT__BaseBlock_heap_ops()
{
    block_heap_t *heap = block_heap_create(10, 20, BH_NOW);
    ASSERT(heap != NULL, "heap allocation failed");
    block_heap_destroy(heap);
}

void __CUT__BaseBlock_alloc_free()
{
    int i = 0;
    void *temp = NULL;
    block_heap_t *heap =  NULL;
	
    heap = block_heap_create(10, 20, BH_NOW);
    ASSERT(heap != NULL, "heap allocation failed");
	
    // allocate a valid amount, then inavlid amount to test failure
    for(i = 0; i < 10; i++) {
        printf("allocating #%d\n", i);
        temp = block_alloc(heap);
        ASSERT(temp != NULL, "failed to allocate block from heap");
        // test freeing it
        block_free(heap, temp);
        // now make a leak
        temp = block_alloc(heap);
        ASSERT(temp != NULL, "failed to allocate block from heap");
    }
	
    // TODO: cause error condition and test for it
	
    // valgrind will detect if this clears the leaked memory above
    block_heap_destroy(heap);
    block_heap_loginfo();
}

void __CUT_TAKEDOWN__BaseBlockTest( void ) {
}
