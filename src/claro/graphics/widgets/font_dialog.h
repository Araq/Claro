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


#ifndef _CLARO_GRAPHICS_WIDGETS_FONT_DIALOG_H
#define _CLARO_GRAPHICS_WIDGETS_FONT_DIALOG_H
/** \defgroup font_dialog_widget_t Font Selection Widget
 * \brief The Font Selection Widget
 * @{
 */

#define assert_only_font_dialog_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.dialogs.font" )
#define assert_valid_font_dialog_widget(o,n)	assert_not_null( o, n, "Font Dialog Widget" ); \
						assert_only_font_dialog_widget( o, n )

typedef struct font_dialog_widget_ font_dialog_widget_t;

struct font_dialog_widget_
{
	widget_t widget;
	
	font_t selected;
};

/* functions */

/**
 * \brief Creates a Font Selection widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param flags Widget flags.
 * \return A new Font Selection widget object.
 */
CLFEXP object_t *font_dialog_widget_create( object_t *parent, int flags );

/**
 * \brief Changes the selected font
 * 
 * \param obj A valid Font Selection widget object
 * \param font The name of the font
 */
CLFEXP void font_dialog_set_font( object_t *obj, const char *face, int size, int weight, int slant, int decoration );

/**
 * \brief Returns a structure representing the currently selected font
 * 
 * \param obj A valid Font Selection widget object
 * \return A font_t structure containing information about the selected font.
 */
CLFEXP font_t *font_dialog_get_font( object_t *obj );

/*\@}*/
#endif
