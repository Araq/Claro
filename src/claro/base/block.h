/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */


#ifndef _CLARO_BASE_BLOCK_H
#define _CLARO_BASE_BLOCK_H

typedef struct block_ block_t;
typedef struct block_heap_ block_heap_t;

/* A block of memory allocated to the allocator */
struct block_
{
	node_t node;
	
	/* link back to our heap */
	block_heap_t *heap;
	
	/* pointer to the first item */
	void *data;
	
	/* lists of items */
	list_t free_list;
	list_t used_list;
};

/* A pile of blocks */
struct block_heap_
{
	node_t node;
	
	uint32 elem_size;
	uint32 block_elems;
	uint32 free_elems;
	
	uint32 alloc_size;
	
	uint32 flags;
	
	list_t blocks;
};

/* List of all block heaps */
extern list_t block_heaps;

/* Flag for block_heap_create */
#define BH_DONTCARE 0

#define BH_NOW 1
#define BH_LAZY 0

void block_init( );

/* Functions for heaps */
CLFEXP block_heap_t *block_heap_create( uint32 elem_size, uint32 block_elems, uint32 flags );
CLFEXP void block_heap_destroy( block_heap_t *heap );

/* Functions for blocks */
CLFEXP void *block_alloc( block_heap_t *heap );
CLFEXP void block_free( block_heap_t *heap, void *data );

CLFEXP void block_heap_loginfo( );

#endif
