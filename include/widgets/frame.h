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


#ifndef _CLARO_GRAPHICS_WIDGETS_FRAME_H
#define _CLARO_GRAPHICS_WIDGETS_FRAME_H
/** \defgroup frame_widget_t Frame Widget
 * \brief The Frame Widget
 * @{
 */

#define assert_only_frame_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.frame" )
#define assert_valid_frame_widget(o,n)	assert_not_null( o, n, "Frame Widget" ); \
					assert_only_frame_widget( o, n )

#define CLARO_FRAME_MAXIMUM 256

typedef struct frame_widget_ {
    widget_t widget;
    char text[CLARO_FRAME_MAXIMUM];
} frame_widget_t;

/**
 * \brief Creates a Frame widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Frame widget object.
 */
CLFEXP object_t *frame_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Creates a Frame widget with a label
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \param label The initial label for the frame
 * \return A new Frame widget object.
 */
CLFEXP object_t *frame_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label );

/**
 * \brief Creates a Container widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Container widget object.
 */
CLFEXP void frame_set_label(object_t *frame, const char *label);

#define frame_set_text frame_set_label

/*\@}*/
#endif

