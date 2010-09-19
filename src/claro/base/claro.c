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

object_t *claro = 0;
int claro_in_loop = 0;

/* signal catcher */
void signal_handle( int signal )
{
	clog( CL_INFO, "Caught signal %d: exiting...", signal );
	
	if ( claro_in_loop == 1 )
	{
		/* cause loop to exit */
		claro_in_loop = 0;
	}
	else
	{
		/* not in loop, just exit */
		exit( 0 );
	}
}

/* Initialise Claro base */
void claro_base_init( )
{
	if ( claro != 0 )
	{
		clog( CL_WARNING, "claro_base_init() called more than once - ignoring" );
		return;
	}
	
	signal( SIGTERM, signal_handle );
	signal( SIGINT, signal_handle );
	signal( SIGQUIT, signal_handle );
	signal( SIGABRT, signal_handle );
	
	block_init( );
	list_init( );
	log_init( );
	
	object_init( );
	
	claro = object_create( 0, sizeof( object_t ), "claro_base" );
}

/* Destroy Claro */
void claro_destroy( )
{
	
}

/* Destroy claro.base */
void claro_base_destroy( )
{
	/* FIXME: clean up stuff init'd in _init() */
}

extern list_t destroy_list;

/* Claro main loop, single-step */
void claro_run( )
{
	node_t *n, *tn;
	
	/* destroy any objects awaiting destruction */
	LIST_FOREACH_SAFE( n, tn, destroy_list.head )
	{
		object_destroy( OBJECT(n->data) );
		node_del( n, &destroy_list );
		node_free( n );
	}
	
	event_send( claro, "mainloop", "" );
}

/* Run the Claro main loop */
void claro_loop( )
{
	claro_in_loop = 1;
	
	while ( claro_in_loop )
	{
		claro_run( );
#ifndef _NIX
		mssleep( 1 );
#endif
	}
	
	claro_destroy( );
}

/* Marks to exit the main loop next turn */
void claro_shutdown( )
{
	claro_in_loop = 0;
}
