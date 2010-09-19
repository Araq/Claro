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


#ifndef _CLARO_GRAPHICS_WIDGETS_RADIO_H
#define _CLARO_GRAPHICS_WIDGETS_RADIO_H
/** \defgroup radio_widget_t Radio Button/Group Widgets
 * \brief The Radio Button and Group Widgets
 * @{
 */

#define assert_only_radiobutton_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.radiobutton" )
#define assert_valid_radiobutton_widget(o,n)	assert_not_null( o, n, "Radio Button" ); \
						assert_only_radiobutton_widget( o, n )

#define assert_only_radiogroup_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.radiogroup" )
#define assert_valid_radiogroup_widget(o,n)	assert_not_null( o, n, "Radio Group" ); \
						assert_only_radiogroup_widget( o, n )

#define RADIOBUTTON_TEXT_MAX 256

typedef struct radiogroup_
{
	object_t widget;
	list_t buttons;
	
	object_t *selected;
	
	void *ndata;
} radiogroup_t;

typedef struct radiobutton_widget_
{
	widget_t widget;
	char text[RADIOBUTTON_TEXT_MAX];
	
	radiogroup_t *group;
} radiobutton_widget_t;

/**
 * \brief Creates a Radio Group widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param flags Widget flags.
 * \return A new Radio Group widget object.
 */
CLFEXP object_t *radiogroup_create( object_t *parent, int flags );

/**
 * \brief Creates a Radio Button widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param group A valid Radio Group widget object
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param label The label of the radio widget
 * \param flags Widget flags.
 * \return A new Radio Button widget object.
 */
CLFEXP object_t *radiobutton_widget_create( object_t *parent, object_t *group, bounds_t *bounds, const char *label, int flags );

/**
 * \brief Set the label of a Radio Button
 * 
 * \param obj A valid Radio Button widget
 * \param label The new label for the Radio Button
 */
CLFEXP void radiobutton_set_label( object_t *obj, const char *label );

#define radiobutton_set_text radiobutton_set_label

/**
 * \brief Set the group of a Radio Button
 * 
 * \param rbutton A valid Radio Button widget
 * \param group A valid Radio Group widget object
 */
CLFEXP void radiobutton_set_group( object_t *rbutton, object_t *group );

/*\@}*/
#endif

