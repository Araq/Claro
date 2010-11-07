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

list_t object_list = {};
list_t destroy_list = {};

void object_init( )
{
	list_create( &object_list );
	list_create( &destroy_list );
}

void object_destroy_handle( object_t *object, event_t *event )
{
	node_t *n;
	
	if ( object->destroy_pending == 1 )
		return; // already in destroy list.
	
	n = node_create( );
	
	node_add( object, n, &destroy_list );
	
	object->destroy_pending = 1;
}

int clo_override_size = 0;

void object_override_next_size( int size )
{
	clo_override_size = size;
}

object_t *object_create( object_t *parent, uint32 size, const char *type )
{
	object_t *obj;
	node_t *n;
	
	if ( clo_override_size > 0 && clo_override_size > size )
	{
		size = clo_override_size;
		clo_override_size = 0;
	}
	
	obj = (object_t *)smalloc( size );
	
	/* clean */
	memset( obj, 0, size );
	
	/* moo */
	strcpy( obj->type, type );
	obj->destroy_pending = 0;
	
	/* add to global object list */
	n = node_create( );
	node_add( obj, n, &object_list );
	
	/* set our parent */
	object_set_parent( obj, parent );
	
	/* init object event list */
	list_create( &obj->event_handlers );
	
	/* init object children list */
	list_create( &obj->children );
	
	/* set up default handler(s) */
	object_addhandler( obj, "destroy", object_destroy_handle );
	
	return obj;
}

int event_send( object_t *object, const char *event, const char *fmt, ... )
{
	va_list argp;
	node_t *n;
	event_handler_t *h;
	event_t e;
	int hn = 0;
	char tmp[1024];
	int a;

	va_start( argp, fmt );

	strcpy( e.name, event );
	e.object = object;
	e.arg_num = strlen( fmt );
	strncpy( e.format, fmt, 16 );
	
	e.arglist = NULL;
	if ( e.arg_num > 0 )
		e.arglist = (void **)malloc( sizeof(void *) * e.arg_num );
	
	for ( a = 0; a < e.arg_num; a++ )
	{
		switch ( fmt[a] )
		{
			case 'p':
				e.arglist[a] = va_arg( argp, void * );
				break;
			case 'i':
				e.arglist[a] = malloc( sizeof(int) );
				*((int *)e.arglist[a]) = va_arg( argp, int );
				break;
			case 'd':
				e.arglist[a] = malloc( sizeof(double) );
				*((double *)e.arglist[a]) = va_arg( argp, double );
				break;
			default:
				e.arglist[a] = NULL; /* this is bad. */
		}
	}
	
	e.handled = 0;

	va_end( argp );
	
	sprintf(tmp, "Event '%s' sent to object '%s' at %p", event, 
	        object->type, object);
	
	LIST_FOREACH( n, object->event_handlers.head )
	{
		event_iface_func_t *iff;
		event_t *ep = &e;
		
		h = (event_handler_t *)n->data;
		
		if ( strcmp( event, h->type ) )
			continue;
		
		if ( h->data != 0 )
		{
			iff = (event_iface_func_t *)h->func;
			(*iff)( object, ep, h->data );
		}
		else
			(*h->func)( object, ep );
		
		hn++;
	}
	
	for ( a = 0; a < e.arg_num; a++ )
	{
		if ( fmt[a] == 'i' || fmt[a] == 'd' )
			free( e.arglist[a] );
	}
	
	if ( e.arglist != NULL )
		free( e.arglist );
	
	/* debug for everything but mainloop, which is called too often to debug! */
	if ( strcmp( event, "mainloop" ) )
		clog( CL_DEBUG, "%s, %d handlers called.", tmp, hn );
	
	return e.handled;
}

void *event_get_arg_ptr( event_t *e, int arg )
{
	if ( arg >= e->arg_num || arg < 0 )
		return NULL;
	
	return e->arglist[arg];
}

int event_get_arg_int( event_t *e, int arg )
{
	int *a = (int *)event_get_arg_ptr( e, arg );
	
	if ( a == NULL )
		return 0;
	
	return *a;
}

double event_get_arg_double( event_t *e, int arg )
{
	double *a = (double *)event_get_arg_ptr( e, arg );
	
	if ( a == NULL )
		return 0;
	
	return *a;
}

void object_addhandler( object_t *object, const char *event, event_func_t *func )
{
	node_t *n;
	event_handler_t *h;
	
	n = node_create( );
	h = (event_handler_t *)smalloc( sizeof(event_handler_t) );
	
	strncpy( h->type, event, 32 );
	h->func = func;
	
	/* add to object's event list */
	node_add( h, n, &object->event_handlers );
}

void object_addhandler_interface( object_t *object, const char *event, event_func_t *func, void *data )
{
	node_t *n;
	event_handler_t *h;
	
	n = node_create( );
	h = (event_handler_t *)smalloc( sizeof(event_handler_t) );
	
	strncpy( h->type, event, 32 );
	h->func = func;
	h->data = data;
	
	/* add to object's event list */
	node_add( h, n, &object->event_handlers );
}

void object_destroy( object_t *obj )
{
	node_t *n, *nn, *tn;
	
	if ( !( n = node_find( obj, &object_list ) ) )
		return;
	
	/* clean up parenting */
	object_set_parent( obj, 0 );
	
	/* make all our children have no parent.
	   FIXME: there should be a flag to destroy children
	   NOTE: widget_destroy will automatically handle children through the OS, though? */
	LIST_FOREACH_SAFE( nn, tn, obj->children.head )
	{
		/* remove the parenting; this will free the current node too */
		object_set_parent( (object_t *)nn->data, 0 );
	}
	
	/* remove all handlers */
	LIST_FOREACH_SAFE( nn, tn, obj->event_handlers.head )
	{
		free( nn->data );
		node_del( nn, &obj->event_handlers );
		node_free( nn );
	}
	
	/* remove the object from the global list, kill the node */
	node_del( n, &object_list );
	node_free( n );
	
	/* clean up the memory */
	free( obj );
}

void object_set_parent( object_t *obj, object_t *parent )
{
	node_t *n;
	
	if ( obj->parent != 0 )
	{
		/* remove old parenting */
		n = node_find( obj, &obj->parent->children );
		
		if ( n != 0 )
		{
			node_del( n, &obj->parent->children );
			node_free( n );
		}
		else
			clog( CL_ERROR,	"Object has a parent listed, but the parent's "
			                "children list doesn't contain it." );
	}
	
	if ( parent != 0 )
	{
		/* add obj to parent's children list */
		
		n = node_create( );
		node_add( obj, n, &parent->children );
	}
	
	obj->parent = parent;
	
	if ( parent != 0 )
		event_send( obj, "parent_attach", "" );
}

char *event_get_name( event_t *event )
{
	return event->name;
}

