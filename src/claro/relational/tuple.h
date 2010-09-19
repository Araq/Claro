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


#ifndef _CLARO_RELATIONAL_TUPLE_H
#define _CLARO_RELATIONAL_TUPLE_H

#include <claro/base.h>
#include <claro/relational.h>

struct tuple_ {
	void *parent;
	relation_t *parent_relation;
	char *key;
	int value_type;
	
	union {
		char *strval;
		int intval;
		bool boolval;
		void *ptrval;
	} value;
};

typedef struct tuple_ tuple_t;

/*
 * Types of tuples.
 * claro-relational implements a string type, an integer type,
 * a boolean type and a pointer type.
 */
enum {
	TUPLE_STR,
	TUPLE_INT,
	TUPLE_BOOL,
	TUPLE_PTR
};

CLFEXP tuple_t *tuple_init(tuple_t *);
CLFEXP void tuple_destroy(tuple_t *);
CLFEXP tuple_t *tuple_locate_by_relation(relation_t *, char *key);
CLFEXP relation_t *tuple_parent(tuple_t *);

/* These functions are used in the event that we want to reverse lookup
 * an object inside a relation providing a specific tuple. Realistically,
 * this probably will not be commonly used, but we really need this stuff
 * because it is useful.
 */
CLFEXP void *parent_locate_by_strval(list_t *, char *key, char *strval);
CLFEXP void *parent_locate_by_intval(list_t *, char *key, int intval);
CLFEXP void *parent_locate_by_boolval(list_t *, char *key, bool boolval);
CLFEXP void *parent_locate_by_ptrval(list_t *, char *key, void *ptrval);

CLFEXP tuple_t *tuple_insert_strval(relation_t *, char *key, char *strval);
CLFEXP tuple_t *tuple_insert_intval(relation_t *, char *key, int intval);
CLFEXP tuple_t *tuple_insert_boolval(relation_t *, char *key, bool boolval);
CLFEXP tuple_t *tuple_insert_ptrval(relation_t *, char *key, void *ptrval);

#define TUPLE_TYPE(x)	x->value_type

#define TUPLE_STR(x)	x->value.strval
#define TUPLE_INT(x)	x->value.intval
#define TUPLE_BOOL(x)	x->value.boolval
#define TUPLE_PTR(x)	x->value.ptrval

#endif
