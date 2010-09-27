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

#include "rbtree.h"
#include "store.h"
#include <string.h>
#include <stdlib.h>


typedef struct _storage {
    char *name;
    void *data;
    size_t dlen;
    int type;
    
    
    char color;
    struct _storage *left;
    struct _storage *right;
} storage_t;


#define STORAGE_COMPARE(X,Y) strcmp((X)->name, (Y)->name)

ADT_DEFINE_RBTREE_PROTOTYPES(storage_t, left, right, color, STORAGE_COMPARE);
ADT_DEFINE_RBTREE_FUNCTIONS(storage_t, left, right, color, STORAGE_COMPARE);

static storage_t *storage_tree = NULL;

void store_put(const char *name, void *data, size_t dlen, int type)
{
    storage_t *elem = NULL;
    
    assert(name != NULL && "name can't be null");
        
    elem = calloc(1, sizeof(storage_t));
    assert(elem != NULL && "memory allocation error");
    
    elem->name = strdup(name);
    elem->dlen = dlen;
    elem->data = data;
    elem->type = type;
    
    
    adt_storage_t_add(&storage_tree, elem);
}

void *store_get(const char *name, size_t *dlen, int *type)
{
    storage_t sample;
    storage_t *elem = NULL;
    
    assert(name != NULL);
    assert(dlen != NULL);
    assert(type != NULL);
    sample.name = (char *)name;
    
    elem = adt_storage_t_find_member(storage_tree, &sample);
    if(elem) {
        *dlen = elem->dlen;
        *type = elem->type;
        return elem->data;
    } else {
        return NULL;
    }
}


void *store_take(const char *name, size_t *dlen, int *type)
{
    storage_t sample;
    storage_t *elem = NULL;
    void *data = NULL;
    
    sample.name = (char *)name;
    assert(name != NULL);
    assert(dlen != NULL);
    assert(type != NULL);
    
    if(adt_storage_t_delete_if_member(&storage_tree, &sample, &elem)) {
        *dlen = elem->dlen;
        *type = elem->type;
        data = elem->data;
        assert(data != NULL && "recovered an element that had a NULL data");
        free(elem);
    }
    
    return data;
}


int store_has(const char *name) 
{
    size_t ignored_dlen;
    int ignored_type;
    assert(name != NULL && "name can't be null");

    return (store_get(name, &ignored_dlen, &ignored_type) != NULL);
    
}



GEN_PUT_TYPE_DEFINITION(int, CL_TYPE_INT);
GEN_PUT_TYPE_DEFINITION(long, CL_TYPE_LONG);
GEN_PUT_TYPE_DEFINITION(float, CL_TYPE_FLOAT);
GEN_PUT_TYPE_DEFINITION(double, CL_TYPE_DOUBLE);

void store_put_string(const char *name, const char *data, size_t length)
{
    char *new_data = calloc(1, sizeof(length)+1);

    memcpy(new_data, data, length+1);
    new_data[length] = '\0';
    
    store_put(name, new_data, length, CL_TYPE_STRING);
}



GEN_GET_TYPE_DEFINITION(int, CL_TYPE_INT);
GEN_GET_TYPE_DEFINITION(long, CL_TYPE_LONG);
GEN_GET_TYPE_DEFINITION(float, CL_TYPE_FLOAT);
GEN_GET_TYPE_DEFINITION(double, CL_TYPE_DOUBLE);

char *store_get_string(const char *name, size_t *length)
{
    char *data = NULL;
    int type = 0;
    
    assert(name != NULL && "name can't be null");
    assert(length != NULL && "length can't be null");
    
    data = store_get(name, length, &type);
    assert(type == CL_TYPE_STRING && "type of data is not CL_TYPE_STRING");
    
    return data;
}


GEN_TAKE_TYPE_DEFINITION(int, CL_TYPE_INT);
GEN_TAKE_TYPE_DEFINITION(long, CL_TYPE_LONG);
GEN_TAKE_TYPE_DEFINITION(float, CL_TYPE_FLOAT);
GEN_TAKE_TYPE_DEFINITION(double, CL_TYPE_DOUBLE);


char *store_take_string(const char *name, size_t *length)
{
    char *data = NULL;
    int type;
    
    data = (char *)store_take(name, length, &type);
    assert(type == CL_TYPE_STRING && "data is not a CL_TYPE_STRING");
    
    return data;
}
