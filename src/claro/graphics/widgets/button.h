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


#ifndef _CLARO_GRAPHICS_WIDGETS_BUTTON_H
#define _CLARO_GRAPHICS_WIDGETS_BUTTON_H
/** \defgroup button_widget_t Button Widget
 * \brief The Button Widget
 * @{
 */

#define assert_only_button_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.button" )
#define assert_valid_button_widget(o,n)	assert_not_null( o, n, "Button Widget" ); \
					assert_only_button_widget( o, n )

#define CLARO_BUTTON_MAXIMUM 256

typedef struct button_widget_ button_widget_t;

struct button_widget_
{
	widget_t widget;
	char text[CLARO_BUTTON_MAXIMUM];
};

/* functions */

/**
 * \brief Creates a Button widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Button widget object.
 */
CLFEXP object_t *button_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Creates a Button widget with a label
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \param label The label for the button
 * \return A new Button widget object.
 */
CLFEXP object_t *button_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label );

/**
 * \brief Changes the label of the button
 * 
 * \param obj A valid Button widget object
 * \param label The new label for the button
 */
CLFEXP void button_set_label( object_t *obj, const char *label );

#define button_set_text button_set_label

/**
 * \brief Changes the image of the button
 *
 * \warning This function is not implemented yet and is not portable.
 *          Do not use it.
 * 
 * \param obj A valid Button widget object
 * \param image The new image for the button
 */
CLFEXP void button_set_image( object_t *obj, image_t *image );

/*\@}*/
#endif
