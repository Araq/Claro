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


#ifndef _CLARO_GRAPHICS_WIDGETS_WINDOW_H
#define _CLARO_GRAPHICS_WIDGETS_WINDOW_H
/** \defgroup window_widget_t Window Widget
 * \brief The Window Widget
 * @{
 */

#define assert_only_window_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.window" )
#define assert_valid_window_widget(o,n)	assert_not_null( o, n, "Window" ); \
					assert_only_window_widget( o, n )

typedef struct window_widget_ window_widget_t;
struct layout_;

#define cWindowModalDialog 1
#define cWindowCenterParent 2
#define cWindowNoResizing 4

struct window_widget_
{
	widget_t widget;
	char title[512];
	
	image_t *icon;
	
	widget_t *menubar;
	widget_t *workspace;
	
	int exsp_tools, exsp_status, exsp_init;
};

enum
{
	cWindowFixedSize = 1,
};

/* functions */

/**
 * \brief Creates a Window widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Window widget object.
 */
CLFEXP object_t *window_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets a Window's title
 * 
 * \param w A valid Window widget object
 * \param title The new title for the window
 */
CLFEXP void window_set_title( object_t *w, const char *title );

/**
 * \brief Makes a window visible
 * 
 * \param w A valid Window widget object
 */
CLFEXP void window_show( object_t *w );

/**
 * \brief Makes a window invisible
 * 
 * \param w A valid Window widget object
 */
CLFEXP void window_hide( object_t *w );

/**
 * \brief Gives focus to a window
 * 
 * \param w A valid Window widget object
 */
CLFEXP void window_focus( object_t *w );

/**
 * \brief Maximises a window
 * 
 * \param w A valid Window widget object
 */
CLFEXP void window_maximise( object_t *w );

/**
 * \brief Minimises a window
 * 
 * \param w A valid Window widget object
 */
CLFEXP void window_minimise( object_t *w );

/**
 * \brief Restores a window
 * 
 * \param w A valid Window widget object
 */
CLFEXP void window_restore( object_t *w );

/**
 * \brief Sets a window's icon
 * 
 * \param w A valid Window widget object
 * \param icon A valid Image object
 */
CLFEXP void window_set_icon( object_t *w, image_t *icon );

#define window_maximize window_maximise
#define window_minimize window_minimise

/*\@}*/
#endif
