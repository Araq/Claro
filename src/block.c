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

#include <stdlib.h>
#include <string.h>
#include "base.h"

list_t block_heaps = {};

/* prepare */
void block_init( )
{
	list_create( &block_heaps );
}

/* expands a block_heap_t by 1 block */
void block_heap_expand( block_heap_t *bh )
{
	block_t *block;
	char *blp;
	node_t *node;
	char *offset;
	int a;
	
	blp = smalloc( sizeof(block_t) + (bh->alloc_size * bh->block_elems) );
	block = (block_t *)blp;
	
	memset( blp, 0, sizeof(block_t) + (bh->alloc_size * bh->block_elems) );
	
	offset = blp + sizeof(block_t);
	block->data = offset;

	list_create( &block->free_list );
	list_create( &block->used_list );
	
	for ( a = 0; a < bh->block_elems; a++ )
	{
		node = (node_t *)offset;
		node->prev = node->next = 0;
		
		node_add( offset + sizeof(node_t), node, &block->free_list );
		
		offset += bh->alloc_size;
	}
	
	node_add( block, (node_t *)block, &bh->blocks );
	
	bh->free_elems += bh->block_elems;
}

/* creates a new block_heap_t */
block_heap_t *block_heap_create( uint32 elem_size, uint32 block_elems, uint32 flags )
{
	block_heap_t *bh;
	
	bh = (block_heap_t *)smalloc( sizeof(block_heap_t) );
	
	/* blank the memory for safety */
	memset( bh, 0, sizeof(block_heap_t) );
	
	bh->elem_size = elem_size;
	bh->block_elems = block_elems;
	bh->free_elems = 0;
	
	bh->alloc_size = bh->elem_size + sizeof(node_t);
	
	bh->flags = flags;
	
	list_create( &bh->blocks );
	
	node_add( bh, (node_t *)bh, &block_heaps );
	
	if ( flags & BH_NOW )
		block_heap_expand( bh );
	
	return bh;
}

/* completely frees a block_heap-t and all blocks */
void block_heap_destroy( block_heap_t *heap )
{
	node_t *n, *tn;
	
	/* just plain delete them all! they are all single blocks */
	LIST_FOREACH_SAFE( n, tn, heap->blocks.head )
		free( n );
	
	/* clear it out of the global heap list */
	node_del( (node_t *)heap, &block_heaps );
	
	/* everything related to heap has gone, time for itself */
	free( heap );
}

/* allocates a new item from a block_heap_t */
void *block_alloc( block_heap_t *heap )
{
	node_t *n, *fn;
	block_t *b;
	
	/* no free space? */
	if ( heap->free_elems == 0 )
	{
		block_heap_expand( heap );
		
		/* still no free space? */
		if ( heap->free_elems == 0 )
			return NULL;
	}

	LIST_FOREACH( n, heap->blocks.head )
	{
		b = (block_t *)n;
		
		if ( LIST_LENGTH(&b->free_list) < 1 )
			continue;
		
		/* pull the first free node from the list */
		fn = b->free_list.head;
		
		/* mark it as used */
		node_move( fn, &b->free_list, &b->used_list );
		
		/* keep count */
		heap->free_elems--;
		
		/* debug */
		clog( CL_DEBUG, "block_alloc( heap=%p ) -> %p", heap, fn->data );
		
		/* return pointer to it */
		return fn->data;
	}
	
	/* this should never happen */
	clog( CL_CRITICAL, "block_alloc() failed to allocate; heap free element count (%d) wrong: (%s:%d)", heap->free_elems, __FILE__, __LINE__ );
	
	return NULL;
}

/* frees an item back to the block_heap_t */
void block_free( block_heap_t *heap, void *data )
{
	node_t *n, *dn;
	block_t *b;
	
	LIST_FOREACH( n, heap->blocks.head )
	{
		b = (block_t *)n;
		
		/* see if the element belongs to this list */
		dn = node_find( data, &b->used_list );
		
		if ( dn )
		{
			/* mark it as free */
			node_move( dn, &b->used_list, &b->free_list );
			
			/* keep count */
			heap->free_elems++;
			
			/* debug */
			clog( CL_DEBUG, "block_free( heap=%p, data=%p )", heap, data );
			
			/* we're done */
			return;
		}
		
		/* and just in case, check it's not already free'd */
		dn = node_find( data, &b->free_list );
		
		if ( dn )
		{
			clog( CL_CRITICAL, "attempt to block_free() data (%p) which has already been free'd from heap (%p): (%s:%d)", data, heap, __FILE__, __LINE__ );
		}
	}
	
	/* this should never happen */
	clog( CL_CRITICAL, "attempt to block_free() data (%p) which doesn't belong to the heap passed (%p): (%s:%d)", data, heap, __FILE__, __LINE__ );
}

void block_heap_loginfo( )
{
	node_t *hn, *bn;
	block_heap_t *heap;
	block_t *block;
	
	int mem_usage = 0, mem_unused = 0;
	int raw_usage = 0;
	int heap_count = 0;
	int block_count = 0;
	int item_count = 0;
	
	LIST_FOREACH( hn, block_heaps.head )
	{
		heap = (block_heap_t *)hn;
		heap_count++;
		
		LIST_FOREACH( bn, heap->blocks.head )
		{
			block = (block_t *)bn;
			block_count++;
			
			mem_usage += heap->elem_size * heap->block_elems;
			raw_usage += sizeof(block_t) + (heap->alloc_size * heap->block_elems);
			
			item_count += heap->block_elems;
		}
		
		mem_unused += heap->free_elems * heap->elem_size;
		raw_usage += sizeof(block_heap_t);
	}
	
	clog( CL_INFO, "Block heap allocator stats:" );
	clog( CL_INFO, "\theaps:\t\t\t%d", heap_count );
	clog( CL_INFO, "\tblocks:\t\t\t%d", block_count );
	clog( CL_INFO, "\titems:\t\t\t%d", item_count );
	clog( CL_INFO, "\traw bytes:\t\t%d", raw_usage );
	clog( CL_INFO, "\ttotal data bytes:\t%d", mem_usage );
	clog( CL_INFO, "\tunused databytes:\t%d", mem_unused );
	clog( CL_INFO, "\tused databytes:\t\t%d", mem_usage - mem_unused );
}
