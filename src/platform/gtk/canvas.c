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

#include <gtk/gtk.h>

/* Canvas */

static void cgraphics_canvas_expose_handler( GtkWidget *widget, GdkEventExpose * event, widget_t *w )
{
	canvas_widget_t *cw = (canvas_widget_t *)w;
	cw->cr = gdk_cairo_create(widget->window);

	event_send( OBJECT(w), "redraw","p", cw->cr);
	
	cairo_destroy(cw->cr);
}

static gint cgraphics_canvas_mouse_move( GtkWidget *widget, GdkEventMotion *event, widget_t *w )
{
	int x, y;
	GdkModifierType state;
	
	if ( event->is_hint )
		gdk_window_get_pointer (event->window, &x, &y, &state);
	else
	{
		x = event->x;
		y = event->y;
		state = event->state;
	}
	
	if ( w->notify_flags & cNotifyMouse )
		event_send( OBJECT(w), "mouse_moved", "ii", x, y );
	
	return TRUE;
}

static gint cgraphics_canvas_mouse_down( GtkWidget *widget, GdkEventButton *event, widget_t *w )
{
	int x, y;
	
	x = (int)event->x;
	y = (int)event->y;
	
	switch ( event->button )
	{
		case 1:
			event_send( OBJECT(w), "clicked", "ii", x, y );
			break;
		case 3:
			event_send( OBJECT(w), "right_clicked", "ii", x, y );
			break;
	}
	
	return FALSE;
}

static gint cgraphics_canvas_mouse_up( GtkWidget *widget, GdkEventButton *event, widget_t *w )
{
	int x, y;
	
	x = (int)event->x;
	y = (int)event->y;
	
	switch ( event->button )
	{
		case 1:
			event_send( OBJECT(w), "released", "ii", x, y );
			break;
		case 3:
			event_send( OBJECT(w), "right_released", "ii", x, y );
			break;
	}
	
	return FALSE;
}

void cgraphics_canvas_widget_create( widget_t *widget )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;

	widget->native = gtk_drawing_area_new( );
	
	gtk_widget_set_events( widget->native, GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_HINT_MASK );
	
	g_signal_connect( G_OBJECT(widget->native), "expose-event", G_CALLBACK(cgraphics_canvas_expose_handler), widget );
	g_signal_connect( G_OBJECT(widget->native), "motion-notify-event", G_CALLBACK(cgraphics_canvas_mouse_move), widget );
	g_signal_connect( G_OBJECT(widget->native), "button-press-event", G_CALLBACK(cgraphics_canvas_mouse_down), widget);
	g_signal_connect( G_OBJECT(widget->native), "button-release-event", G_CALLBACK(cgraphics_canvas_mouse_up), widget);
	
	cgraphics_widget_create( widget );
	
	gtk_widget_queue_draw( GTK_WIDGET(widget->native) );
	
	cw->csz_loaded = 0;
}

void cgraphics_canvas_redraw( canvas_widget_t *w )
{
	gtk_widget_queue_draw( GTK_WIDGET(w->widget.native) );
}

/* claro native text functions */

void cgraphics_canvas_set_text_color( widget_t *widget, double r, double g, double b, double a )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	
	cairo_set_source_rgba( cw->cr, r, g, b, a );
	
	cw->fr = r;
	cw->fg = g;
	cw->fb = b;
	cw->fa = a;
}

void cgraphics_canvas_set_text_bgcolor( widget_t *widget, double r, double g, double b, double a )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	
	cw->br = r;
	cw->bg = g;
	cw->bb = b;
	cw->ba = a;
}

void cgraphics_canvas_set_text_font( widget_t *widget, char *face, int size, int weight, int slant, int decoration )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	PangoFontDescription *fd;
	PangoLayout *layout;
	char fds[256], sz[64];
	
	if ( !strcasecmp( face, "Verdana" ) )
		strcpy( fds, "Sans" );
	else
		strcpy( fds, face );
	strcat( fds, " " );
	if ( weight == CTEXT_WEIGHT_BOLD )
		strcat( fds, "Bold " );
	if ( slant == CTEXT_SLANT_ITALIC )
		strcat( fds, "Italic " );
	sprintf( sz, "%d", (int)((float)size * 0.6) );
	strcat( fds, sz );
	
	layout = cw->fontdata;
	
	if ( layout == NULL )
	{
		layout = pango_cairo_create_layout ( cw->cr );
		cw->fontdata = layout;
	}
	
	pango_layout_set_text( layout, "Text", -1 );
	fd = pango_font_description_from_string( fds );
	pango_layout_set_font_description( layout, fd );
	pango_font_description_free( fd );
	
	cw->fontsize = size;
	
	return;
	/*canvas_widget_t *cw = (canvas_widget_t *)widget;
	cairo_font_weight_t w = CAIRO_FONT_WEIGHT_NORMAL;
	cairo_font_slant_t s = CAIRO_FONT_SLANT_NORMAL;
	
	if ( weight == CTEXT_WEIGHT_BOLD )
		w = CAIRO_FONT_WEIGHT_BOLD;
	
	if ( slant == CTEXT_SLANT_ITALIC )
		s = CAIRO_FONT_SLANT_ITALIC;
	//printf( "%d, %d\n", CAIRO_FONT_SLANT_ITALIC, s );
	cairo_select_font_face( cw->cr, face, CAIRO_FONT_SLANT_NORMAL, w );
	cairo_set_font_size( cw->cr, size );
	
	cw->csz_loaded = 0;
	cw->fontsize = size;*/
}

int cgraphics_canvas_text_width( widget_t *widget, const char *text, int len )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	PangoLayout *layout = cw->fontdata;
	int width, height;
	
	if ( layout == NULL )
	{
		cgraphics_canvas_set_text_font( widget, "Sans", 16, 0, 0, 0 );
		layout = cw->fontdata;
	}
	
	pango_cairo_update_layout( cw->cr, layout );
	pango_layout_set_text( layout, text, len );
	pango_layout_get_pixel_size( layout, &width, &height );
	
	return width;
/*
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	cairo_text_extents_t ex;
	
	cairo_text_extents( cw->cr, text, &ex );
	
	return ex.x_advance;*/
	//return canvas_cairo_buffered_text_width( widget, text, len );
}

int cgraphics_canvas_text_box_width( widget_t *widget, const char *text, int len )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	PangoLayout *layout = cw->fontdata;
	int width, height;
	
	if ( layout == NULL )
	{
		cgraphics_canvas_set_text_font( widget, "Sans", 16, 0, 0, 0 );
		layout = cw->fontdata;
	}
	
	pango_cairo_update_layout( cw->cr, layout );
	pango_layout_set_text( layout, text, len );
	pango_layout_get_pixel_size( layout, &width, &height );
	
	return width;
	//return cgraphics_canvas_text_width( widget, text, len );
	//return canvas_cairo_buffered_text_width( widget, text, len );
}

int cgraphics_canvas_text_display_count( widget_t *widget, const char *text, int width )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	PangoLayout *layout = cw->fontdata;
	int index, trailing;
	
	if ( layout == NULL )
	{
		cgraphics_canvas_set_text_font( widget, "Sans", 16, 0, 0, 0 );
		layout = cw->fontdata;
	}
	
	pango_cairo_update_layout( cw->cr, layout );
	pango_layout_set_text( layout, text, -1 );
	//pango_layout_get_pixel_size( layout, &width, &height );
	
	pango_layout_xy_to_index( layout, width * PANGO_SCALE, 0 * PANGO_SCALE, &index, &trailing );
	index++;
	
	return (trailing?index:index-1);
	//return (trailing==1?index+1:(index>0?index-1:0)); // can't actually fix the character that goes over this point :)
	//return canvas_cairo_buffered_text_display_count( widget, text, width );
}

void cgraphics_canvas_show_text( widget_t *widget, int x, int y, const char *text, int len )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	PangoLayout *layout = cw->fontdata;
	int width, height;
	
	if ( layout == NULL )
	{
		cgraphics_canvas_set_text_font( widget, "Sans", 16, 0, 0, 0 );
		layout = cw->fontdata;
	}
	
	pango_cairo_update_layout( cw->cr, layout );
	pango_layout_set_text( layout, text, len );
	pango_layout_get_pixel_size( layout, &width, &height );
	
	cairo_set_source_rgba( cw->cr, cw->br, cw->bg, cw->bb, cw->ba );
	cairo_rectangle( cw->cr, x, y, width, height );
	cairo_fill( cw->cr );
	
	cairo_set_source_rgba( cw->cr, cw->fr, cw->fg, cw->fb, cw->fa );
	cairo_move_to( cw->cr, x, y );
	pango_cairo_show_layout( cw->cr, layout );
	
	return;
	/*canvas_widget_t *cw = (canvas_widget_t *)widget;
	char textout[len+1];
	strncpy( textout, text, len );
	textout[len] = 0;
	
	cairo_move_to( cw->cr, x, y+(cw->fontsize?cw->fontsize:16) );
	cairo_show_text( cw->cr, textout );*/
}

void cgraphics_canvas_fill_rect( widget_t *widget, int x, int y, int w, int h, double r, double g, double b, double a )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	cairo_set_source_rgba( cw->cr, r, g, b, a );
	cairo_rectangle( cw->cr, x, y, w, h );
	cairo_fill( cw->cr );
}

void cgraphics_canvas_draw_image( widget_t *widget, image_t *image, int x, int y )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	gdk_cairo_set_source_pixbuf( cw->cr, GDK_PIXBUF(image->native), x, y );
	cairo_paint( cw->cr );
}

