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


#ifndef _CLARO_GRAPHICS_WIDGETS_STATUSBAR_H
#define _CLARO_GRAPHICS_WIDGETS_STATUSBAR_H
/** \defgroup statusbar_widget_t StatusBar Widget
 * \brief The StatusBar Widget
 * @{
 */

#define assert_only_statusbar_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.statusbar" )
#define assert_valid_statusbar_widget(o,n)	assert_not_null( o, n, "StatusBar" ); \
						assert_only_statusbar_widget( o, n )

typedef struct statusbar_widget_ {
    widget_t widget;
    char text[CLARO_BUTTON_MAXIMUM];
} statusbar_widget_t;

/**
 * \brief Creates a StatusBar widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param flags Widget flags.
 * \return A new StatusBar widget object.
 */
CLFEXP object_t *statusbar_widget_create( object_t *parent, int flags );

/**
 * \brief Sets the text of a statusbar
 * 
 * \param obj A valid StatusBar widget
 * \param text The new text
 */
CLFEXP void statusbar_set_text( object_t *obj, const char *text );

/*\@}*/
#endif
