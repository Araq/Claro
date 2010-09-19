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

object_t *canvas_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	assert_valid_widget( parent, "parent" );
	return default_widget_create(parent, sizeof(canvas_widget_t), 
                                           "claro.graphics.widgets.canvas", bounds, 
                                           flags, cgraphics_canvas_widget_create);    
}

void canvas_redraw( object_t *widget )
{
	assert_valid_canvas_widget( widget, "widget" );
	cgraphics_canvas_redraw( (canvas_widget_t *)widget );
}

/* claro text functions */
#ifndef NO_CAIRO
/* Run cairo_text_extents on each letter (FIXME: non-ASCII?)
 * and save the resulting sizing info. Makes it much faster.
 */
void canvas_cairo_load_charsize( canvas_widget_t *widget )
{
	char test[2] = " ";
	int a;
	
	if ( widget->csz_loaded == 1 )
		return;
	
	widget->csz_loaded = 1;
	
	for ( a = 1; a < 254; a++ )
	{
		test[0] = (char)a;
		cairo_text_extents( widget->cr, test, &widget->charsize[a] );
	}
}

/* Using the cached function above, calculate the width of
 * the string.
 */
int canvas_cairo_buffered_text_width( canvas_widget_t *widget, const char *text, int len )
{
	int a, b = 0, c;
	
	canvas_cairo_load_charsize( widget );
	
	c = strlen( text );
	
	for ( a = 0; a < c; a++ )
	{
		b += widget->charsize[text[a]].x_advance;
	}
	
	return b;
}

/* Using the cached text widths, calculate how many of the
 * characters in /text/ can be displayed in /width/ pixels.
 */
int canvas_cairo_buffered_text_display_count( canvas_widget_t *widget, const char *text, int width )
{
	int a, b = 0, c;
	
	canvas_cairo_load_charsize( widget );
	
	c = strlen( text );
	
	for ( a = 0; a < c; a++ )
	{
		b += widget->charsize[text[a]].x_advance;
		
		if ( b > width )
		{
			return a;
		}
	}

	return c;
}
#endif
/* 
 * Set the text color for Claro native font rendering.
 *
 * Part of the Claro native text functions. In some cases,
 * this can be faster than Cairo.
 *
 * When faster drawing is possible, native functions are used.
 * In other cases, Cairo may be used - so this does NOT guarentee
 * that the cairo data (current point, etc) will not change!
 */
void canvas_set_text_color( object_t *widget, double r, double g, double b, double a )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	cgraphics_canvas_set_text_color( widget, r, g, b, a );
}

/* 
 * Set the background text color for Claro native font rendering.
 *
 * Part of the Claro native text functions. In some cases,
 * this can be faster than Cairo.
 *
 * When faster drawing is possible, native functions are used.
 * In other cases, Cairo may be used - so this does NOT guarentee
 * that the cairo data (current point, etc) will not change!
 */
void canvas_set_text_bgcolor( object_t *widget, double r, double g, double b, double a )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	cgraphics_canvas_set_text_bgcolor( widget, r, g, b, a );
}

/* 
 * Set the font details for Claro native font rendering.
 *
 * Part of the Claro native text functions. In some cases,
 * this can be faster than Cairo.
 *
 * When faster drawing is possible, native functions are used.
 * In other cases, Cairo may be used - so this does NOT guarentee
 * that the cairo data (current point, etc) will not change!
 */
void canvas_set_text_font( object_t *widget, char *face, int size, int weight, int slant, int decoration )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	cgraphics_canvas_set_text_font( widget, face, size, weight, slant, decoration );
}

/* 
 * Find the width of the first /len/ characters of /text/
 * using Claro native font rendering.
 *
 * Part of the Claro native text functions. In some cases,
 * this can be faster than Cairo.
 *
 * When faster drawing is possible, native functions are used.
 * In other cases, Cairo may be used - so this does NOT guarentee
 * that the cairo data (current point, etc) will not change!
 */
int canvas_text_width( object_t *widget, const char *text, int len )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	return cgraphics_canvas_text_width( widget, text, len );
}

/* 
 * Find the bounding box width of the first /len/ characters of /text/
 * using Claro native font rendering. This is the size a background
 * would be drawn at.
 *
 * Part of the Claro native text functions. In some cases,
 * this can be faster than Cairo.
 *
 * When faster drawing is possible, native functions are used.
 * In other cases, Cairo may be used - so this does NOT guarentee
 * that the cairo data (current point, etc) will not change!
 */
int canvas_text_box_width( object_t *widget, const char *text, int len )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	return cgraphics_canvas_text_box_width( widget, text, len );
}

/* 
 * Calculate and return the number of characters in /text/ 
 * that can be displayed in /width/ pixels using Claro native
 * font rendering.
 *
 * Part of the Claro native text functions. In some cases,
 * this can be faster than Cairo.
 *
 * When faster drawing is possible, native functions are used.
 * In other cases, Cairo may be used - so this does NOT guarentee
 * that the cairo data (current point, etc) will not change!
 */
int canvas_text_display_count( object_t *widget, const char *text, int width )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	return cgraphics_canvas_text_display_count( widget, text, width );
}

/* 
 * Render the first /len/ characters of /text/ at /x/,/y/
 * using Claro native font rendering.
 *
 * Part of the Claro native text functions. In some cases,
 * this can be faster than Cairo.
 *
 * When faster drawing is possible, native functions are used.
 * In other cases, Cairo may be used - so this does NOT guarentee
 * that the cairo data (current point, etc) will not change!
 */
void canvas_show_text( object_t *widget, int x, int y, const char *text, int len )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	cgraphics_canvas_show_text( widget, x, y, text, len );
}

/*
 */
void canvas_fill_rect( object_t *widget, int x, int y, int w, int h, double r, double g, double b, double a )
{
	assert_valid_canvas_widget( widget, "widget" );
	
	cgraphics_canvas_fill_rect( widget, x, y, w, h, r, g, b, a );
}

void canvas_draw_image( object_t *widget, image_t *image, int x, int y )
{
	assert_valid_canvas_widget( widget, "widget" );
	assert_valid_image( image, "image" );
	
	cgraphics_canvas_draw_image( widget, image, x, y );
}

#ifndef NO_CAIRO
cairo_t *canvas_get_cairo_context( object_t *widget )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	
	assert_valid_canvas_widget( widget, "widget" );
	
	return cw->cr;
}
#endif
