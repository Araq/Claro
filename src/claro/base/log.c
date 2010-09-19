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

list_t log_outputs = {};
block_heap_t *log_heap = NULL;

void log_init( )
{
	log_heap = block_heap_create( sizeof(log_type_t), 5, BH_NOW );;
	
	list_create( &log_outputs );
	
#ifdef LOG_DEBUG
	log_add_fd( CL_ANY, stderr );
#else
	log_add_fd( CL_INFO, stderr );
#endif
}

void log_add_fd( int level, FILE *fd )
{
	log_type_t *l;
	node_t *n;
	
	n = node_create( );
	l = block_alloc( log_heap );
	
	l->level = level;
	l->fd = fd;

	node_add( l, n, &log_outputs );
}

void log_fd_set_level( int level, FILE *fd )
{
	node_t *n;
	log_type_t *l;
	
	LIST_FOREACH( n, log_outputs.head )
	{
		l = (log_type_t *)n->data;
		
		if ( l->fd == fd )
			l->level = level;
	}
}

void claro_log( int level, char *application, char *file, int line, const char *function, const char *fmt, ... )
{
	va_list args;
	char buf[VA_BUF_SIZE];
	char place[4096];
	node_t *n;
	log_type_t *l;
	
	struct tm *tm_ptr;
	time_t tm;
	char tm_str[60];
	
	tm = time( NULL );
	tm_ptr = localtime( &tm );
	strftime( tm_str, 100, "%Y-%m-%d %T", tm_ptr );
	
	sprintf( place, "[%s at %s %s:%d in %s]", tm_str, application, file, line, function );
	
	va_start( args, fmt );
	vsnprintf( buf, VA_BUF_SIZE, fmt, args );
	va_end( args );
	
	if ( LIST_LENGTH(&log_outputs) == 0 )
	{
		fprintf( stderr, "%s %s\n", place, buf );
	}

	LIST_FOREACH( n, log_outputs.head )
	{
		l = (log_type_t *)n->data;
		
		if ( level >= l->level )
		{
			/* FIXME: timestamp this */
			fprintf( l->fd, "%s %s\n", place, buf );
		}
	}
	
	if ( level >= CL_CRITICAL )
	{
		/* FIXME: this should probably send a signal instead */
		raise( SIGABRT ); /* ?? */
		exit( 0 );
	}
}
