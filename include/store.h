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


#ifndef CLARO_STORE_H
#define CLARO_STORE_H

#include <sys/types.h>

/** Base types used for common data stored into the, uh, store. */
enum {
    CL_TYPE_INT, CL_TYPE_LONG, CL_TYPE_DOUBLE, CL_TYPE_FLOAT, CL_TYPE_STRING,
        CL_TYPE_UNKNOWN
};

/**
 * Puts the given data into the store keeping the size and a given type you want.
 * The type is just an int but the CL_TYPE_* variables are used as some default
 * types in the store.
 *
 * All parameters must be present.
 */
CLFEXP void store_put(const char *name, void *data, size_t dlen, int type);

/**
 * Tells you if this named variable is in the store.
 */
CLFEXP int store_has(const char *name);

/**
 * Returns the data by the given name, or NULL if it doesn't
 * exist.  The dlen and type parameters are out-parameters
 * that will tell you the type and length of the returned data.
 *
 * The difference between store_get and store_take is that 
 * get just returns a reference to the data and you are 
 * not supposed to clean it or free it yourself.
 * Whereas store_take also removes the value from the store
 * so you can free it.
 */
CLFEXP void *store_get(const char *name, size_t *dlen, int *type);

/**
 * Works almost exactly like store_get except that it also
 * removes the value from the store.  This is the only way
 * to remove a value.  This is done since you almost always
 * have to clean the stored value (with free, etc.) so 
 * it needs to be returned on delete to force you to clean it.
 */
CLFEXP void *store_take(const char *name, size_t *dlen, int *type);

/**
 * Convenience method for putting a string into the store, but
 * putting a *copy* of the string.  This is important since you can
 * now share the string with others without worrying about who owns it.
 */
CLFEXP void store_put_string(const char *name, const char *data, size_t length);

/**
 * Gets a string that's stored in the store but *doesn't* remove it.  
 * Use this to read a string.
 */
CLFEXP char *store_get_string(const char *name, size_t *length);

/**
 * Takes a string from the store removing it and returning it.
 * Remember that this is a *copy* of the original string so you
 * should free it when you're done.
 */
CLFEXP char *store_take_string(const char *name, size_t *length);


/**
 * To make it easier to add various types to the store system in a consistent
 * way a series of "generator" macros are used to generate the necessary code
 * for each type.  This macro generates the declaration to put a type.
 *
 * You use it by putting GEN_PUT_TYPE_DECLARE(int); in a header file to declare that
 * function.  You then have a function store_put_int(const char *name, int val);
 */
#define GEN_PUT_TYPE_DECLARE(tType) CLFEXP void store_put_##tType(const char *name, tType val);

/**
 * Generates the code needed for the implementation function needed to store a type.
 * Macro is used as GEN_PUT_TYPE_DEFINITION(int, CL_TYPE_INT); and it generates a
 * function void store_put_int(const char *name, int data); You put this into a
 * .c file and compile it.
 */
#define GEN_PUT_TYPE_DEFINITION(tType, tId) CLFEXP void store_put_##tType(const char *name, tType data) {\
tType *new_data = calloc(1, sizeof(tType));\
*new_data = data;\
store_put(name, new_data, sizeof(tType), tId);\
}


/** Just like GEN_PUT_TYPE_DECLARE but declares a generic get function for that type. 
 * If you have GEN_GET_TYPE_DECLARE(int); in a header file it declares: 
 * int store_get_int(const char *name);
 */
#define GEN_GET_TYPE_DECLARE(tType) tType store_get_##tType(const char *name);

/** 
 * Just like GEN_PUT_TYPE_DEFINITION but it creates a function that gets the value
 * out without removing it from the store.  The macro call GEN_GET_TYPE_DEFINITION(int, CL_TYPE_INT);
 * will generate:  int store_get_int(const char *name);
 */
#define GEN_GET_TYPE_DEFINITION(tType, tId) tType store_get_##tType(const char *name) {\
tType *data = NULL;\
size_t dlen;\
int type;\
data = (tType *)store_get(name, &dlen, &type);\
assert(data != NULL);\
assert(dlen == sizeof(tType) && "data not correct length for " # tType);\
assert(type == tId && "data is not type " # tId);\
return *data;\
}


/**
 * Generates a take function declaration for the type.  The macro 
 * GEN_TAKE_TYPE_DECLARE(int) will create:  int store_take_int(const char *name);
 */
#define GEN_TAKE_TYPE_DECLARE(tType) tType store_take_##tType(const char *name);

/**
 * Generates the code for a take function for the given type and having the 
 * correct type code of tId.  A call of GEN_TAKE_TYPE_DEFINITION(int, CL_TYPE_INT);
 * will generate:  int store_take_int(const char *name);
 *
 * A main thing about this take function actually does all of the cleanup for you and
 * returns the *value* of what you want so you don't have to deal with any memory
 * management.
 */
#define GEN_TAKE_TYPE_DEFINITION(tType, tId) CLFEXP tType store_take_##tType(const char *name) {\
tType *data = NULL;\
tType value;\
size_t dlen;\
int type;\
data = (tType *)store_take(name, &dlen, &type);\
assert(dlen == sizeof(tType) && "data not correct length for " # tType);\
assert(type == tId && "data is not a " # tId);\
assert(data != NULL);\
value = *data;\
free(data);\
return value;\
}



GEN_TAKE_TYPE_DECLARE(int);
GEN_TAKE_TYPE_DECLARE(long);
GEN_TAKE_TYPE_DECLARE(float);
GEN_TAKE_TYPE_DECLARE(double);

GEN_GET_TYPE_DECLARE(int);
GEN_GET_TYPE_DECLARE(long);
GEN_GET_TYPE_DECLARE(float);
GEN_GET_TYPE_DECLARE(double);

GEN_PUT_TYPE_DECLARE(int);
GEN_PUT_TYPE_DECLARE(long);
GEN_PUT_TYPE_DECLARE(float);
GEN_PUT_TYPE_DECLARE(double);


#endif

