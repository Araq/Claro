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


#include "base.h"

block_heap_t *node_heap = 0;

/* init internal data structures */
void list_init( )
{
	clog( CL_INFO, "allocating in list_init( );" );
	node_heap = block_heap_create( sizeof(node_t), 10, BH_NOW );
}

/* safety: inits a list to default values */
void list_create( list_t *list )
{	
	memset( list, 0, sizeof(list_t) );
}

/* creates a new node */
node_t *node_create( )
{
	node_t *n;
	
	cassert( node_heap != 0, "node heap not initialised. Did you run claro_base_init()?" );
	
	/* allocate it */
	n = (node_t *)block_alloc( node_heap );
	
	/* initialize */
	n->next = n->prev = n->data = NULL;
	
	/* return a pointer to the new node */
	return n;
}

/* frees a node */
void node_free( node_t *n )
{
	/* free it */
	block_free( node_heap, n );
}

/* adds a node to the end of a list */
void node_add( void *data, node_t *n, list_t *l )
{
	node_t *tn;
	
	n->data = data;

	/* first node? */
	if (!l->tail)
	{
		n->next = n->prev = 0;
		
		l->head = n;
		l->tail = n;
		l->count++;
		
		return;
	}

	/* Speed increase. */
	tn = l->tail;

	/* set our `prev' to the last node */
	n->prev = tn;

	/* set the last node's `next' to us */
	tn->next = n;
	
	/* clean up our `next` pointer */
	n->next = 0;

	/* set the list's `tail' to us */
	l->tail = n;

	/* up the count */
	l->count++;
}

/* adds a node to the beginning of a list */
void node_prepend( void *data, node_t *n, list_t *l )
{
	node_t *hn;
	
	n->data = data;

	/* first node? */
	if (!l->tail)
	{
		n->next = n->prev = 0;
		
		l->head = n;
		l->tail = n;
		l->count++;
		
		return;
	}

	/* Speed increase. */
	hn = l->head;

	/* set our `next' to the first node */
	n->next = hn;

	/* set the first node's `prev' to us */
	hn->prev = n;
	
	/* clean up our `prev` pointer */
	n->prev = 0;

	/* set the list's `head' to us */
	l->head = n;

	/* up the count */
	l->count++;
}

void node_del( node_t *n, list_t *l )
{
	/* do we even have a node? */
	if (!n)
	{
		clog( CL_WARNING, "node_del(): called with NULL node" );
		return;
	}

	/* are we the head? */
	if (!n->prev)
		l->head = n->next;
	else
		n->prev->next = n->next;

	/* are we the tail? */
	if (!n->next)
		l->tail = n->prev;
	else
		n->next->prev = n->prev;

	/* down the count */
	l->count--;
}

/* finds a node by `data' */
node_t *node_find( void *data, list_t *l )
{
	node_t *n;
	
	LIST_FOREACH(n, l->head)
	{
		if (n->data == data)
			return n;
	}

	return NULL;
}

/* delete node from old list, add to new list */
void node_move( node_t *n, list_t *oldlist, list_t *newlist )
{
	node_del( n, oldlist );
	node_add( n->data, n, newlist );
}
