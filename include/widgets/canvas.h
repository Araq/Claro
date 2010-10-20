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


#ifndef _CLARO_GRAPHICS_WIDGETS_CANVAS_H
#define _CLARO_GRAPHICS_WIDGETS_CANVAS_H
/** \defgroup canvas_widget_t Canvas Widget
 * \brief The Canvas Widget
 * @{
 */

#define assert_only_canvas_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.canvas" )
#define assert_valid_canvas_widget(o,n)	assert_not_null( o, n, "Canvas Widget" ); \
					assert_only_canvas_widget( o, n )


#ifndef NO_CAIRO
#include <cairo.h>
#endif

#ifndef M_PI
#define M_PI 3.142
#endif

/* OLD */
#define CTEXT_SLANT_NORMAL cFontSlantNormal
#define CTEXT_SLANT_ITALIC cFontSlantItalic

#define CTEXT_WEIGHT_NORMAL cFontWeightNormal
#define CTEXT_WEIGHT_BOLD cFontWeightBold

#define CTEXT_EXTRA_NONE cFontDecorationNormal
#define CTEXT_EXTRA_UNDERLINE cFontDecorationUnderline
/* END OLD */

typedef struct canvas_widget_ canvas_widget_t;

struct canvas_widget_
{
	widget_t widget;
#ifndef NO_CAIRO
	cairo_surface_t *surface;
	cairo_t *cr;
#else
  /* be binary compatible! */
	void* unused1;
	void* unused2;
#endif
	
	void *surfdata;
	void *fontdata;
	int font_height;
	
	float fr, fg, fb, fa;
	float br, bg, bb, ba;
	
#ifndef NO_CAIRO
	cairo_text_extents_t charsize[256];
	int csz_loaded;
	
	int fontsize;
#else
  double unused3[6 * 256]; /* cairo_text_extents_t consists of 6 doubles */
  int unused4;
  int unused5;
#endif
};

/* functions */

/**
 * \brief Creates a Canvas widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Canvas widget object.
 */
CLFEXP object_t *canvas_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Invalidates and redraws a canvas widget
 * 
 * \param widget A valid Canvas widget object.
 */
CLFEXP void canvas_redraw( object_t *widget );

/* claro text functions */

/**
 * \brief Set the current text color
 * 
 * \param widget A valid Canvas widget object.
 * \param r Red component (0.0 - 1.0)
 * \param g Green component (0.0 - 1.0)
 * \param b Blue component (0.0 - 1.0)
 * \param a Alpha component (0.0 - 1.0)
 */
CLFEXP void canvas_set_text_color( object_t *widget, double r, double g, double b, double a );

/**
 * \brief Set the current text background color
 * 
 * \param widget A valid Canvas widget object.
 * \param r Red component (0.0 - 1.0)
 * \param g Green component (0.0 - 1.0)
 * \param b Blue component (0.0 - 1.0)
 * \param a Alpha component (0.0 - 1.0)
 */
CLFEXP void canvas_set_text_bgcolor( object_t *widget, double r, double g, double b, double a );

/**
 * \brief Set the current canvas font
 * 
 * \param widget A valid Canvas widget object.
 * \param face The font face
 * \param size The font height in pixels
 * \param weight The weight of the font
 * \param slant The slant of the font
 * \param decoration Font decorations
 */
CLFEXP void canvas_set_text_font( object_t *widget, char *face, int size, int weight, int slant, int decoration );

/**
 * \brief Calculates the width of the specified text
 * 
 * \param widget A valid Canvas widget object.
 * \param text The text to calulate the length of
 * \param len The number of characters of text to calulcate
 * \return Width of the text in pixels
 */
CLFEXP int canvas_text_width( object_t *widget, const char *text, int len );

/**
 * \brief Calculates the width of the specified text's bounding box
 * 
 * \param widget A valid Canvas widget object.
 * \param text The text to calulate the length of
 * \param len The number of characters of text to calulcate
 * \return Width of the text's bounding box in pixels
 */
CLFEXP int canvas_text_box_width( object_t *widget, const char *text, int len );

/**
 * \brief Calculates the number of characters of text that can be displayed
 *        before width pixels.
 * 
 * \param widget A valid Canvas widget object.
 * \param text The text to calulate the length of
 * \param width The width to fit the text in
 * \return The number of characters of text that will fit in width pixels.
 */
CLFEXP int canvas_text_display_count( object_t *widget, const char *text, int width );

/**
 * \brief Displays the specified text on the canvas
 * 
 * \param widget A valid Canvas widget object.
 * \param x The X position at which the text will be drawn
 * \param y The Y position at which the text will be drawn
 * \param text The text to calulate the length of
 * \param len The number of characters of text to calulcate
 */
CLFEXP void canvas_show_text( object_t *widget, int x, int y, const char *text, int len );

/**
 * \brief Draws a filled rectangle
 * 
 * \param widget A valid Canvas widget object.
 * \param x The X position at which the rectangle will start
 * \param y The Y position at which the rectangle will start
 * \param w The width of the rectangle
 * \param h The height of the rectangle
 * \param r Red component (0.0 - 1.0)
 * \param g Green component (0.0 - 1.0)
 * \param b Blue component (0.0 - 1.0)
 * \param a Alpha component (0.0 - 1.0)
 */
CLFEXP void canvas_fill_rect( object_t *widget, int x, int y, int w, int h, double r, double g, double b, double a );

/**
 * \brief Draws the specified image on the canvas
 * 
 * \param widget A valid Canvas widget object.
 * \param image The image to draw
 * \param x The X position at which the image will be drawn
 * \param y The Y position at which the image will be drawn
 */
CLFEXP void canvas_draw_image( object_t *widget, image_t *image, int x, int y );

/* claro "extensions" of cairo */
/** \internal
 * \brief Internal claro extension of cairo text functions
 */
CLFEXP int canvas_cairo_buffered_text_width( canvas_widget_t *widget, const char *text, int len );

/** \internal
 * \brief Internal claro extension of cairo text functions
 */
CLFEXP int canvas_cairo_buffered_text_display_count( canvas_widget_t *widget, const char *text, int width );


#ifndef NO_CAIRO
CLFEXP cairo_t *canvas_get_cairo_context( object_t *widget );
#else
#define canvas_get_cairo_context(w) NULL
#endif

/*\@}*/
#endif
