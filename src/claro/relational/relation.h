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


#ifndef _CLARO_RELATIONAL_RELATION_H
#define _CLARO_RELATIONAL_RELATION_H

#include <claro/base.h>
#include <claro/relational.h>

struct relation_ {
	void *parent;
	int *cardinality;	/* pointer to tuples.count */
	list_t tuples;
};

typedef struct relation_ relation_t;

CLFEXP relation_t *relation_create(relation_t *);

CLFEXP int relation_get_cardinality(void *parent);
CLFEXP int relation_get_cardinality_local(relation_t *);

CLFEXP void relation_destroy(relation_t *);
CLFEXP void *relation_parent(relation_t *);

#endif
