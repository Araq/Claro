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


#ifndef _CLARO_GRAPHICS_WIDGETS_LABEL_H
#define _CLARO_GRAPHICS_WIDGETS_LABEL_H
/** \defgroup label_widget_t Label Widget
 * \brief The Label Widget
 * @{
 */

#define assert_only_label_widget(o,n)		assert_type_check( o, n, "claro.graphics.widgets.label" )
#define assert_valid_label_widget(o,n)		assert_not_null( o, n, "Label" ); \
						assert_only_label_widget( o, n )

#define CLARO_LABEL_MAXIMUM 256

typedef struct label_widget_ label_widget_t;

struct label_widget_
{
    widget_t widget;
    char text[CLARO_LABEL_MAXIMUM];
};

enum cLabelJustify {
	cLabelLeft = 0x01,
	cLabelRight = 0x02,
	cLabelCenter = 0x04,
	cLabelFill = 0x08
};

/**
 * \brief Creates a Label widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Label widget object.
 */
CLFEXP object_t *label_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Creates a Label widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Label widget object.
 */
CLFEXP object_t *label_widget_create_with_text( object_t *parent, bounds_t *bounds, int flags, const char *text );

/**
 * \brief Sets the text of a label widget
 * 
 * \param obj A valid label widget
 * \param text The text this label widget will show
 */
CLFEXP void label_set_text( object_t *obj, const char *text );

/**
 * \brief Sets the alignment/justification of a label
 * 
 * \param obj A valid label widget
 * \param text The justification (see cLabelJustify enum)
 */
CLFEXP void label_set_justify( object_t *obj, int flags);

/*\@}*/
#endif

