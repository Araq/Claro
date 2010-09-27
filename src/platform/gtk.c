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


#include "graphics.h"
#include <assert.h>
#include "platform.h"

gint cgraphics_button_clicked_handler( GtkWidget *widget, widget_t *w );


/* called at initialisation */
void cgraphics_init( )
{
	gtk_init( NULL, NULL );
}

/* called each loop, use to process events from the platform */
void cgraphics_check_events( )
{
	gtk_main_iteration_do( FALSE );
}

/* resize handler */
gint cgraphics_resized_handler( GtkWidget *widget, GdkEventConfigure *event, widget_t *w )
{
	widget_set_size( OBJECT(w), event->width, event->height, 1 );
	widget_set_content_size( OBJECT(w), event->width, event->height, 1 );
	widget_set_position( OBJECT(w), event->x, event->y, 1 );
	
	return 0;
}

void cgraphics_screen_changed_handler( GtkWidget *widget, GdkScreen *arg1, widget_t *w )
{
	GdkScreen *sc;
	GdkColormap *cm;
	
	gtk_widget_realize( widget );
	
	sc = gtk_widget_get_screen( widget );
	
	cm = gdk_screen_get_rgba_colormap(  gtk_widget_get_screen(widget) );
	
	w->supports_alpha = 0;
	
	if ( cm == NULL )
	{
		printf( "Warning: Alpha not supported\n" );
		cm = gdk_screen_get_rgb_colormap( gtk_widget_get_screen(widget) );
	}
	else
		w->supports_alpha = 1;
	
	gtk_widget_set_colormap( widget, cm );
}

gboolean cgraphics_cd_expose_handler( GtkWidget *widget, GdkEventExpose *event, widget_t *w )
{
	cairo_t *cr;
	
	cr = gdk_cairo_create( GDK_DRAWABLE(widget->window) );
	
	cairo_set_source_rgba( cr, 1.0, 0.0, 0.0, 1.0 );
	cairo_set_operator( cr, CAIRO_OPERATOR_SOURCE );
	cairo_paint( cr );
	
	return false;
}

gint cgraphics_resized2_handler( GtkWidget *widget, GtkAllocation *event, widget_t *w )
{
	if ( !strcmp( w->object.type, "claro.graphics.widgets.container" ) ||
		 !strcmp( w->object.type, "claro.graphics.widgets.frame" ) ||
		 !strcmp( w->object.type, "claro.graphics.widgets.workspace.window" ) ||
		 !strcmp( w->object.type, "claro.graphics.widgets.window" )	)
	{
		if ( event->x == w->size.x && event->y == w->size.y &&
			 event->width == w->size.w && event->height == w->size.h )
			return 0;
		
		widget_set_size( OBJECT(w), event->width, event->height, 1 );
		widget_set_content_size( OBJECT(w), event->width, event->height, 1 );
		widget_set_position( OBJECT(w), event->x, event->y, 1 );
		
		widget_resized_handle( OBJECT(w), 0 );
	}
	
	return 0;
}

/* destroy handler */
gint cgraphics_destroy_handler( widget_t *w )
{
	widget_destroy( OBJECT(w) );
 
	return 1;
}

/* General native widget helpers */

void cgraphics_widget_create( widget_t *widget )
{
	widget_t *parent;
	
	parent = (widget_t *)object_parent(widget);
	
	/* check for parent */
	if ( parent == NULL )
	{
		clog( CL_ERROR, "Widget requires parent, none given." );
		return;
	}
	
	/* FIXME: height should be determined from font height in pixels */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	gtk_widget_set_size_request( GTK_WIDGET(widget->native), widget->size_req->w, widget->size_req->h );
	
	g_signal_connect( G_OBJECT(widget->native), "configure_event", G_CALLBACK(cgraphics_resized_handler), widget );
	g_signal_connect_swapped( G_OBJECT(widget->native), "destroy", G_CALLBACK(cgraphics_destroy_handler), widget );
	
	if ( widget->flags & cWidgetCustomDraw )
	{
		g_signal_connect( G_OBJECT(widget->native), "screen-changed", G_CALLBACK(cgraphics_screen_changed_handler), widget );
		g_signal_connect( G_OBJECT(widget->native), "expose-event", G_CALLBACK(cgraphics_cd_expose_handler), widget );
	}
	
	if ( widget->container != 0 )
		g_signal_connect( G_OBJECT(widget->container), "size_allocate", G_CALLBACK(cgraphics_resized2_handler), widget );
	else
		g_signal_connect( G_OBJECT(widget->native), "size_allocate", G_CALLBACK(cgraphics_resized2_handler), widget );
	
	if ( !strcmp( parent->object.type, "claro.graphics.widgets.splitter" ) )
	{
		size_t splitter_num = (size_t)parent->ndata;
		splitter_widget_t *sw = (splitter_widget_t *)parent;
			
		if ( splitter_num == 0 )
			gtk_paned_pack1( GTK_PANED(parent->native), widget->native, true, false );
		else
			gtk_paned_pack2( GTK_PANED(parent->native), widget->native, true, false );
		
		sw->children[splitter_num].w = widget;
		
		parent->ndata = (void *)(splitter_num+1);
	}
	else if ( !strcmp( parent->object.type, "claro.graphics.widgets.workspace" ) )
	{
		gtk_notebook_append_page( widget_get_container(OBJECT(parent)), widget->native, NULL );
	}
	else
	{
		gtk_layout_put( GTK_LAYOUT(widget_get_container(OBJECT(parent))), widget->native, widget->size_req->x, widget->size_req->y );
	}
	
	g_object_set_data( widget->native, "claro_widget", widget );
	
	if ( widget->flags & cWidgetCustomDraw )
		cgraphics_screen_changed_handler( widget->native, NULL, widget );
	
	gtk_widget_show( GTK_WIDGET(widget->native) );
}

widget_t *cgraphics_gtk_to_claro_widget( GtkWidget *w )
{
	widget_t *objval;
	objval = g_object_get_data( G_OBJECT(w), "claro_widget" );
	return objval;
}

/* General widgets */

void cgraphics_widget_show( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_show( GTK_WIDGET(widget->native) );
}

void cgraphics_widget_hide( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_hide( GTK_WIDGET(widget->native) );
}

void cgraphics_widget_focus( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_grab_focus( GTK_WIDGET(widget->native) );
}

void cgraphics_post_init( widget_t *widget )
{
	
}

void cgraphics_widget_enable( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_set_sensitive( GTK_WIDGET(widget->native), TRUE );
}

void cgraphics_widget_disable( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_set_sensitive( GTK_WIDGET(widget->native), FALSE );
}

void cgraphics_widget_close( widget_t *widget )
{
	if ( widget->container != NULL )
		gtk_widget_destroy( widget->container );
	
	gtk_widget_destroy( widget->native );
}

widget_t *cgraphics_get_widget_window( widget_t *w )
{
	object_t *o ;
	
	o = (object_t *)w;
	
	if ( !strcmp( o->type, "claro.graphics.widgets.window" ) )
		return w;
	
	if ( o->parent == 0 )
		return 0;
	
	return cgraphics_get_widget_window( (widget_t *)o->parent );
}

/* update bounds (global widget function */

void cgraphics_update_bounds( object_t *obj )
{
	widget_t *widget = (widget_t *)obj;

	if (widget->size_req == NULL)
	{
		// these widgets use special OS-determined bounds.
		return;
	}
	
	if ( widget->native == NULL )
		return;

	if ( widget->size_req->w <= -1 || widget->size_req->h <= -1 )
		clog( CL_ERROR, "widget '%s' @ %p about to be sized with negative width or height!", obj->type, obj );
	
	gtk_widget_set_size_request( widget->native, widget->size_req->w, widget->size_req->h );
	
	if ( strcmp( obj->parent->type, "claro.graphics.widgets.splitter" ) )
		gtk_layout_move( GTK_LAYOUT(GTK_WIDGET(widget->native)->parent), widget->native, widget->size_req->x, widget->size_req->y );
}
/* "borrowed" from http://mail.gnome.org/archives/gtk-app-devel-list/2004-November/msg00028.html */
static void widget_get_rect_in_screen( GtkWidget *widget, GdkRectangle *r )
{
	gint x,y,w,h;
	GdkRectangle extents;
	GdkWindow *window;
	window = gtk_widget_get_parent_window(widget); /* getting parent window */
	gdk_window_get_root_origin(window, &x,&y); /* parent's left-top screen coordinates */
	gdk_drawable_get_size(window, &w,&h); /* parent's width and height */
	gdk_window_get_frame_extents(window, &extents); /* parent's extents (including decorations) */
	r->x = x + (extents.width-w)/2 + widget->allocation.x; /* calculating x (assuming: left border size == right border size) */
	r->y = y + (extents.height-h)-(extents.width-w)/2 + widget->allocation.y; /* calculating y (assuming: left border size == right border size == bottom border size) */
	r->width = widget->allocation.width;
	r->height = widget->allocation.height;
}

void cgraphics_widget_screen_offset( widget_t *object, int *dx, int *dy )
{
	GdkRectangle r;
	widget_get_rect_in_screen( object->native, &r );
	*dx = r.x;
	*dy = r.y;
}

GdkCursorType cgraphics_cursors[] = {
	GDK_LEFT_PTR, //cCursorNormal=0,
	0, //cCursorTextEdit,
	GDK_CLOCK, //cCursorWait,
	GDK_HAND1, //cCursorPoint,
};

void cgraphics_widget_set_cursor( widget_t *widget, int cursor )
{
	GdkCursor *gcursor;
	
	gcursor = gdk_cursor_new( cgraphics_cursors[cursor] );
	gdk_window_set_cursor( GDK_WINDOW(GTK_WIDGET(widget->native)->window), gcursor );
	gdk_cursor_destroy( gcursor );
}

/* end of platform functions */
