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


#include <claro/base.h>

#define FAIL_SIGNAL SIGUSR1

#ifdef CLARO_MEMMAN
#undef malloc
#undef calloc
#undef realloc
#undef free
#undef smalloc
#undef scalloc
#undef srealloc
#endif

/* does malloc()'s job and dies if malloc() fails */
void *smalloc( size_t size )
{
	void *buf;
	
	if ( !( buf = malloc( size ) ) )
	{
		clog( CL_CRITICAL, "Attempt to malloc() failed in %s(): (%s:%d)", __FUNCTION__, __FILE__, __LINE__ );
		raise( FAIL_SIGNAL );
	}

	return buf;
}

/* does calloc()'s job and dies if calloc() fails */
void *scalloc( size_t elsize, size_t els )
{
	void *buf;
	
	if ( !( buf = calloc( elsize, els ) ) )
	{
		clog( CL_CRITICAL, "Attempt to malloc() failed in %s(): (%s:%d)", __FUNCTION__, __FILE__, __LINE__ );
		raise( FAIL_SIGNAL );
	}
	
	return buf;
}

/* does realloc()'s job and dies if realloc() fails */
void *srealloc( void *oldptr, size_t newsize )
{
	void *buf;
	
	if ( !( buf = realloc( oldptr, newsize ) ) )
	{
		clog( CL_CRITICAL, "Attempt to malloc() failed in %s(): (%s:%d)", __FUNCTION__, __FILE__, __LINE__ );
		raise( FAIL_SIGNAL );
	}

	return buf;
}

/* does strdup()'s job, only with the above memory functions */
char *sstrdup( const char *s )
{
	char *t;
	
	t = smalloc( strlen(s) + 1 );
	
	strcpy( t, s );
	
	return t;
}

/* debug malloc: smalloc() with memory management */
void *d_malloc( size_t size )
{
	void *p = smalloc( size );
	
	clog( CL_DEBUG, "smalloc(%d) -> %p", size, p );
	
	return p;
}

/* debug calloc: scalloc() with memory management */
void *d_calloc( size_t elsize, size_t els )
{
	void *p = scalloc( elsize, els );
	
	clog( CL_DEBUG, "scalloc(%d,%d) -> %p", elsize, els, p );
	
	return p;
}

/* debug realloc: srealloc() with memory management */
void *d_realloc( void *oldptr, size_t newsize )
{
	void *p = srealloc( oldptr, newsize );
	
	clog( CL_DEBUG, "srealloc(%p,%d) -> %p", oldptr, newsize, p );
	
	return p;
}

/* debug free: free() with memory management */
void d_free( void *ptr, char *file, int line )
{
	clog( CL_DEBUG, "free(%p) from %s:%d", ptr, file, line );
	
	free( ptr );
}

/* show some stats on memory usage */
void memory_stats( )
{
	
}
