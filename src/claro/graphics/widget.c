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


#include <claro/graphics.h>
#include <claro/graphics/platform.h>

bounds_t no_bounds = {0, 0, 0, 0, NULL};

void bounds_set( bounds_t *bounds, int x, int y, int w, int h )
{
	bounds->x = x;
	bounds->y = y;
	bounds->w = w;
	bounds->h = h;
}

bounds_t *new_bounds( int x, int y, int w, int h )
{
	bounds_t *bounds = (bounds_t *)malloc( sizeof(bounds_t) );
	
	bounds->x = x;
	bounds->y = y;
	bounds->w = w;
	bounds->h = h;
	
	bounds->owner = 0;
	
	return bounds;
}

void widget_size_request_changed( object_t *widget )
{
	assert_valid_widget( widget, "widget" );
	
	WIDGET(widget)->size_flags |= cSizeRequestChanged;
	
	cgraphics_update_bounds( widget );
	
	WIDGET(widget)->size_flags -= cSizeRequestChanged;
}

void widget_set_content_size( object_t *widget, int w, int h, int event )
{
	widget_t *rwidget = WIDGET(widget);
	
	assert_valid_widget( widget, "widget" );
	
	if ( rwidget->size_ct.w == w && rwidget->size_ct.h == h )
		return;
	
	rwidget->size_ct.w = w;
	rwidget->size_ct.h = h;
	
	if ( event )
	{
		event_send( OBJECT(widget), "content_resized", "" );
	}
}

void widget_set_size( object_t *widget, int w, int h, int event )
{
	widget_t *rwidget = WIDGET(widget);
	
	assert_valid_widget( widget, "widget" );
	
	if ( rwidget->size.w == w && rwidget->size.h == h )
		return;
	
	rwidget->size.w = w;
	rwidget->size.h = h;
	
	if ( event )
	{
		event_send( OBJECT(widget), "resized", "" );
	}
}

void widget_set_content_position( object_t *widget, int x, int y, int event )
{
	widget_t *rwidget = WIDGET(widget);
	
	assert_valid_widget( widget, "widget" );
	
	if ( rwidget->size_ct.x == x && rwidget->size_ct.y == y )
		return;
	
	rwidget->size_ct.x = x;
	rwidget->size_ct.y = y;
	
	if ( event )
	{
		event_send( OBJECT(widget), "content_moved", "" );
	}
}

void widget_set_position( object_t *widget, int x, int y, int event )
{
	widget_t *rwidget = WIDGET(widget);
	
	assert_valid_widget( widget, "widget" );
	
	if ( rwidget->size.x == x && rwidget->size.y == y )
		return;
	
	rwidget->size.x = x;
	rwidget->size.y = y;
	
	if ( event )
	{
		event_send( OBJECT(widget), "moved", "" );
	}
}

void widget_destroy( object_t *widget )
{
	assert_valid_widget( widget, "widget" );
	event_send( OBJECT(widget), "destroy", "" );
}

void widget_set_notify( object_t *widget, int flags )
{
	widget_t *rwidget = WIDGET(widget);
	assert_valid_widget( widget, "widget" );
	rwidget->notify_flags = flags;
}

void widget_show( object_t *widget )
{
	/* FIXME: check for window, pass on to window_X() */
	assert_valid_widget( widget, "widget" );
	cgraphics_widget_show( widget );
}

void widget_hide( object_t *widget )
{
	/* FIXME: check for window, pass on to window_X() */
	assert_valid_widget( widget, "widget" );
	cgraphics_widget_hide( widget );
}

void widget_enable( object_t *widget )
{
	assert_valid_widget( widget, "widget" );
	cgraphics_widget_enable( widget );
}

void widget_disable( object_t *widget )
{
	assert_valid_widget( widget, "widget" );
	cgraphics_widget_disable( widget );
}

void widget_close( object_t *widget )
{
	assert_valid_widget( widget, "widget" );
#ifdef _NIX
	event_send( widget, "closing", "" );
#endif
	cgraphics_widget_close( widget );
}

void widget_focus( object_t *widget )
{
	/* FIXME: check for window, pass on to window_X() */
	assert_valid_widget( widget, "widget" );
	cgraphics_widget_focus( widget );
}

void widget_destruct( object_t *widget )
{
	object_t *obj = (object_t *)widget;
	node_t *n, *tn;
	
	assert_valid_widget( widget, "widget" );
	
	/* close children too */
	LIST_FOREACH_SAFE( n, tn, obj->children.head )
	{
		widget_destruct( (widget_t *)n->data );
	}
	
	/* FIXME: close it with the platform here (AFTER :)) */
}

void widget_pre_init( object_t *widget )
{
	assert_valid_widget( widget, "widget" );
}

void widget_destroy_handle( object_t *obj, event_t *event )
{
	widget_t *w = (widget_t *)obj;
	
	assert_valid_widget( obj, "obj" );
	
	if ( w->size_req != NULL && w->size_req->owner == NULL )
	{
		free( w->size_req );
		w->size_req = 0;
	}
	
	if ( w->font.used == 1 && w->font.face != 0 )
		free( w->font.face );
}

void widget_resized_handle( object_t *obj, event_t *event )
{
	node_t *n, *tn;
	
	/* tell all children about the resize */
	LIST_FOREACH_SAFE( n, tn, obj->children.head )
	{
		if ( !strncmp( OBJECT(n->data)->type, "claro.graphics.widgets.", 23 ) ||
			 !strcmp( OBJECT(n->data)->type, "claro.graphics.layout" ) )
		{
			event_send( n->data, "update", "" );
		
			/* and the grand-children too */
			widget_resized_handle( n->data, event );
		}
	}
}

void widget_update_handle( object_t *obj, event_t *event )
{
	assert_valid_widget( obj, "obj" );
	
	/* if owner is defined, a layout (or similar) handles size_req. always
	 * ensure that this request is met */
	if ( WIDGET(obj)->size_req != 0 && WIDGET(obj)->size_req->owner != 0 )
		widget_size_request_changed( WIDGET(obj) );
}

void widget_post_init( object_t *widget )
{
	assert_valid_widget( widget, "widget" );
	
	cgraphics_post_init( widget );
	
	/* by default, show all widgets */
	if ( strcmp( widget->type, "claro.graphics.widgets.workspace.window" ) &&
		 strcmp( widget->type, "claro.graphics.widgets.window" ) )
		widget_show( widget );
	
	/* add a destroy handler to clean up after ourselves */
	object_addhandler( OBJECT(widget), "destroy", widget_destroy_handle );
	object_addhandler( OBJECT(widget), "update", widget_update_handle );
	object_addhandler( OBJECT(widget), "content_resized", widget_resized_handle );
	
	/* if we have a parent, send it a child create event */
	if ( widget->parent != NULL )
	{
		event_send( widget->parent, "child_create", "p", widget );
	}
}

/** Common functions */

object_t *default_widget_create(object_t *parent, 
                                     size_t widget_size, const char *widget_name, bounds_t *size_req,
                                     int flags, cgraphics_create_function creator) 
{
    widget_t *widget;
    int is_window = 0;

    assert(widget_name != NULL && "invalid widget name, it cannot be NULL");
    assert(widget_size > 0 && "cannot create widgets that are 0 bytes in size");

    is_window = !strcmp( widget_name, "claro.graphics.widgets.window" );
    
    widget = (widget_t *)object_create( parent, widget_size, widget_name);
    assert(widget != NULL && "failed to allocate widget base");
    
    if ( size_req != NULL )
        widget->size = *size_req;

    widget->size_req = size_req;
    
    widget->flags = flags;
	
    if ( !is_window )
        widget_pre_init( widget );
	
    if(creator) creator( widget );
	
	widget_post_init( widget );
	
    return (object_t *)widget;
}

/* Returns the container of the widget, if specified, otherwise it's native widget. */
void *widget_get_container( object_t *widget )
{
	widget_t *w = WIDGET(widget);
	
	assert_valid_widget( widget, "widget" );
	
	if ( w->container != 0 )
		return w->container;
	
	return w->native;
}

/* notify related */

int widget_get_notify_key( object_t *widget, event_t *event )
{
	assert_valid_widget( widget, "widget" );
	
	return event_get_arg_int( event, 0 );
}

void widget_screen_offset( object_t *widget, int *dx, int *dy )
{
	assert_valid_widget( widget, "widget" );
	cgraphics_widget_screen_offset( WIDGET(widget), dx, dy );
}

/* */

void widget_set_cursor( object_t *widget, int cursor )
{
	cgraphics_widget_set_cursor( WIDGET(widget), cursor );
}

bounds_t *get_req_bounds( object_t *widget )
{
	return WIDGET(widget)->size_req;
}


