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


#ifndef _CLARO_GRAPHICS_WIDGETS_CHECKBOX_H
#define _CLARO_GRAPHICS_WIDGETS_CHECKBOX_H
/** \defgroup checkbox_widget_t Checkbox Widget
 * \brief The Checkbox Widget
 * @{
 */

#define assert_only_checkbox_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.checkbox" )
#define assert_valid_checkbox_widget(o,n)	assert_not_null( o, n, "Checkbox Widget" ); \
						assert_only_checkbox_widget( o, n )


#define CHECKBOX_TEXT_MAX 256

typedef struct checkbox_widget_ {
    widget_t widget;
    char text[CHECKBOX_TEXT_MAX];
    int checked;
} checkbox_widget_t;

/**
 * \brief Creates a Checkbox widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Checkbox widget object.
 */
CLFEXP object_t *checkbox_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Creates a Checkbox widget with a label
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \param label The label for the checkbox
 * \return A new Checkbox widget object.
 */
CLFEXP object_t *checkbox_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label );

/**
 * \brief Sets a new label for the Checkbox widget
 * 
 * \param obj A valid Checkbox widget object.
 * \param label The new label for the checkbox
 */
CLFEXP void checkbox_set_label( object_t *obj, const char *label );

#define checkbox_set_text checkbox_set_label

/**
 * \brief Retrieves the checkbox's check state
 * 
 * \param obj A valid Checkbox widget object.
 * \return 1 if the checkbox is checked, otherwise 0
 */
CLFEXP int checkbox_get_checked( object_t *obj );

#define checkbox_checked checkbox_get_checked

/**
 * \brief Sets the checkbox's checked state
 * 
 * \param obj A valid Checkbox widget object.
 * \param checked 1 if the checkbox should become checked, otherwise 0
 */
CLFEXP void checkbox_set_checked( object_t *obj, int checked );

/*\@}*/
#endif

