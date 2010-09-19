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


#ifndef CLARO_OBJECT_MAP_H
#define CLARO_OBJECT_MAP_H

#include "rbtree.h"
#include <claro/base.h>
#include <string.h>
#include <stdlib.h>

#define OBJECT_MAP_COMPARE(X,Y) strcmp((X)->name, (Y)->name)

typedef struct object_map_ {
    object_t *obj;
    char *name;
    
    char color;
    struct object_map_ *left, *right;
} object_map_t;

ADT_DEFINE_RBTREE_PROTOTYPES(object_map_t, left, right, color, OBJECT_MAP_COMPARE);

#endif
