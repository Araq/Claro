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


#ifndef _CLARO_GRAPHICS_WIDGETS_SCROLLBAR_H
#define _CLARO_GRAPHICS_WIDGETS_SCROLLBAR_H
/** \defgroup scrollbar_widget_t ScrollBar Widget
 * \brief The ScrollBar Widget
 * @{
 */

#define assert_only_scrollbar_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.scrollbar" )
#define assert_valid_scrollbar_widget(o,n)	assert_not_null( o, n, "ScrollBar" ); \
						assert_only_scrollbar_widget( o, n )

typedef struct scrollbar_widget_ scrollbar_widget_t;

#define CLARO_SCROLLBAR_MAXIMUM 256

struct scrollbar_widget_
{
	widget_t widget;
	
	int min;
	int max;
	int pagesize;
};

enum
{
	cScrollbarHorizontal=0,
	cScrollbarVertical=1,
};

/* functions */

/**
 * \brief Creates a ScrollBar widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new ScrollBar widget object.
 */
CLFEXP object_t *scrollbar_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Returns the width that scrollbars should be on this platform
 * 
 * \return Width of vertical scrollbars
 */
CLFEXP int scrollbar_get_sys_width( );

/**
 * \brief Sets the range of a ScrollBar widget
 * 
 * \param w A valid ScrollBar widget object
 * \param min The minimum value
 * \param max The maximum value
 */
CLFEXP void scrollbar_set_range( object_t *w, int min, int max );

/**
 * \brief Sets the position of a ScrollBar widget
 * 
 * \param w A valid ScrollBar widget object
 * \param pos The new position
 */
CLFEXP void scrollbar_set_pos( object_t *w, int pos );

/**
 * \brief Gets the position of a ScrollBar widget
 * 
 * \param w A valid ScrollBar widget object
 * \return The current position
 */
CLFEXP int scrollbar_get_pos( object_t *w );

/**
 * \brief Sets the page size of a ScrollBar widget
 *
 * \param w A valid ScrollBar widget object
 * \param pagesize The size of a page (the number of units visible at one time)
 */
CLFEXP void scrollbar_set_pagesize( object_t *w, int pagesize );

/*\@}*/
#endif
