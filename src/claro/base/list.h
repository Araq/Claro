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


#ifndef _CLARO_BASE_LIST_H
#define _CLARO_BASE_LIST_H

typedef struct node_ node_t;
typedef struct list_ list_t;

/* macros for linked lists */
#define LIST_FOREACH(n, head) for (n = (head); n; n = n->next)  
#define LIST_FOREACH_NEXT(n, head) for (n = (head); n->next; n = n->next)
#define LIST_FOREACH_PREV(n, tail) for (n = (tail); n; n = n->prev)

#define LIST_LENGTH(list) (list)->count

#define LIST_FOREACH_SAFE(n, tn, head) for (n = (head), tn = n ? n->next : NULL; n != NULL; n = tn, tn = n ? n->next : NULL)

/* list node struct */
struct node_
{
	node_t *next, *prev; 
	
	/* pointer to real structure */
	void *data;
};

/* node list struct */
struct list_
{
	node_t *head, *tail;
	
	/* how many entries in the list */
	uint32 count;
};

CLFEXP void list_init( );

CLFEXP void list_create( list_t *list );
CLFEXP node_t *node_create( );
CLFEXP void node_free( node_t *n );
CLFEXP void node_add( void *data, node_t *n, list_t *l );
#define node_append node_add
CLFEXP void node_prepend( void *data, node_t *n, list_t *l );
CLFEXP void node_del( node_t *n, list_t *l );
CLFEXP node_t *node_find( void *data, list_t *l );
CLFEXP void node_move( node_t *n, list_t *oldlist, list_t *newlist );

#endif
