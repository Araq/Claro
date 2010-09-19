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


#ifndef _CLARO_GRAPHICS_WIDGETS_PROGRESS_H
#define _CLARO_GRAPHICS_WIDGETS_PROGRESS_H
/** \defgroup progress_widget_t Progress Widget
 * \brief The Progress Widget
 * @{
 */

#define assert_only_progress_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.progress" )
#define assert_valid_progress_widget(o,n)	assert_not_null( o, n, "Progress" ); \
						assert_only_progress_widget( o, n )

typedef struct progress_widget_ {
    widget_t widget;
} progress_widget_t;


enum cProgressStyle {
	cProgressLeftRight = 0x0,
	cProgressRightLeft = 0x01,
	cProgressTopBottom = 0x02,
	cProgressBottomTop = 0x04
};

/**
 * \brief Creates a Progress widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Progress widget object.
 */
CLFEXP object_t *progress_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets the value of a progress widget
 * 
 * \param progress A valid progress widget object
 * \param percentage Progress value
 */
CLFEXP void progress_set_level( object_t *progress, double percentage );

/**
 * \brief Sets the orientation of a progress widget
 * 
 * \param progress A valid progress widget object
 * \param flags One of the cProgressStyle values
 */
CLFEXP void progress_set_orientation( object_t *progress, int flags );

/*\@}*/
#endif

