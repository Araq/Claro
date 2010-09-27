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


#ifndef _CLARO_GRAPHICS_WIDGETS_TEXTAREA_H
#define _CLARO_GRAPHICS_WIDGETS_TEXTAREA_H
/** \defgroup textarea_widget_t TextArea Widget
 * \brief The TextArea Widget
 * \warning The API of this widget has \b NOT been finalised!
 * @{
 */

#define assert_only_textarea_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.textarea" )
#define assert_valid_textarea_widget(o,n)	assert_not_null( o, n, "TextArea" ); \
						assert_only_textarea_widget( o, n )

#define CLARO_TEXTAREA_MAXIMUM (1024*1024)

typedef struct textarea_widget_ {
    widget_t widget;
    char text[CLARO_TEXTAREA_MAXIMUM];
} textarea_widget_t;

/**
 * \brief Creates a TextArea widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new TextArea widget object.
 */
CLFEXP object_t *textarea_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets the text of a textarea
 * 
 * \param obj A valid TextArea widget
 * \param text The new text
 */
CLFEXP void textarea_set_text( object_t *obj, const char *text );

/**
 * \brief Retrieve the text of a textarea
 * 
 * \param obj A valid TextArea widget
 * \return Pointer to an internal reference of the text. Should not be changed.
 */
CLFEXP char *textarea_get_text( object_t *obj );

/*\@}*/
#endif
