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


#ifndef _CLARO_BASE_MEMORY_H
#define _CLARO_BASE_MEMORY_H

#ifdef CLARO_MEMMAN
#define malloc d_malloc
#define calloc d_calloc
#define realloc d_realloc
#define smalloc d_malloc
#define scalloc d_calloc
#define srealloc d_realloc
#define free(x) d_free(x,__FILE__,__LINE__)

CLFEXP void *d_malloc( size_t size );
CLFEXP void *d_calloc( size_t elsize, size_t els );
CLFEXP void *d_realloc( void *oldptr, size_t newsize );
CLFEXP void d_free( void *ptr, char *file, int line );
#else
CLFEXP void *smalloc(size_t size);
CLFEXP void *scalloc(size_t elsize, size_t els);
CLFEXP void *srealloc(void *oldptr, size_t newsize);
#endif

CLFEXP char *sstrdup(const char *s);
CLFEXP char *strscpy( char *d, const char *s, size_t len );

#endif
