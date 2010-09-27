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


#ifndef _CLARO_BASE_CLARO_H
#define _CLARO_BASE_CLARO_H

#define VA_BUF_SIZE 16384

#define cassert(x,t...) if (!(x)) clog( CL_ERROR, ## t )

/* Checks an object type STRICTLY. Insists it's identical
   NOTE: This will return TRUE if the object is NULL! */
#define type_check(x,y) ( !x || !strcasecmp(OBJECT(x)->type,y) )

/* Checks an object type in a relaxed way. Further ".something" can be appended
   and this will still pass. Useful, for example, to accept widget.<anything>
   NOTE: This will return TRUE if the object is NULL! */
#define type_check_relaxed(x,y) ( !x || !strncasecmp(OBJECT(x)->type,y,strlen(y)) )

/* Assert a type check */
#define assert_type_check(x,n,y) cassert(type_check(x,y), "%s: Object %p failed type assertion: not of type '%s' (was '%s' instead).", n, x, y, OBJECT(x)->type )

/* Assert a relaxed type check */
#define assert_type_check_relaxed(x,n,y) cassert(type_check_relaxed(x,y), "%s: Object %p failed type assertion: not of relaxed type '%s' (was '%s' instead).", n, x, y, OBJECT(x)->type )

/* Assert that value is not NULL
   assert_not_null(value, "value", "valuetype")
   example:
   assert_not_null( obj, "obj", "object_t" ) */
#define assert_not_null(x,n,t) cassert(x!=NULL, "%s: Expected non-NULL %s, got NULL pointer.", n, t)

CLVEXP object_t *claro;

CLFEXP void claro_base_init( );
CLFEXP void claro_loop( );
CLFEXP void claro_run( );
CLFEXP void claro_shutdown( );

CLFEXP void mssleep( int ms );

#endif
