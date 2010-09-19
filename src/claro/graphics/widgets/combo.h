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


#ifndef _CLARO_GRAPHICS_WIDGETS_COMBO_H
#define _CLARO_GRAPHICS_WIDGETS_COMBO_H
/** \defgroup combo_widget_t Combo Widget
 * \brief The Combo Widget
 * @{
 */

#define assert_only_combo_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.combo" )
#define assert_valid_combo_widget(o,n)	assert_not_null( o, n, "Combo Widget" ); \
					assert_only_combo_widget( o, n )

typedef struct combo_widget_ combo_widget_t;

struct combo_widget_
{
	list_widget_t widget;
	
	list_item_t *selected;
};

/* functions */

/**
 * \brief Creates a Combo widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Combo widget object.
 */
CLFEXP object_t *combo_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Append a row to a Combo widget
 * 
 * \param combo A valid Combo widget object.
 * \param text The text for the item.
 * \return A new list item.
 */
CLFEXP list_item_t *combo_append_row( object_t *combo, char *text );

/**
 * \brief Insert a row at the specified position into a Combo widget
 * 
 * \param combo A valid Combo widget object.
 * \param pos The index at which this item will be placed.
 * \param text The text for the item.
 * \return A new list item.
 */
CLFEXP list_item_t *combo_insert_row( object_t *combo, int pos, char *text );

/**
 * \brief Move a row in a Combo widget
 * 
 * \param combo A valid Combo widget object.
 * \param item A valid list item
 * \param row New position to place this item
 */
CLFEXP void combo_move_row( object_t *combo, list_item_t *item, int row );

/**
 * \brief Remove a row from a Combo widget
 * 
 * \param combo A valid Combo widget object.
 * \param item A valid list item
 */
CLFEXP void combo_remove_row( object_t *combo, list_item_t *item );

/**
 * \brief Returns the currently selected Combo item
 * 
 * \param obj A valid Combo widget object.
 * \return The currently selected Combo item, or NULL if no item is selected.
 */
CLFEXP list_item_t *combo_get_selected( object_t *obj );

/**
 * \brief Returns the number of rows in a Combo widget
 * 
 * \param obj A valid Combo widget object.
 * \return Number of rows
 */
CLFEXP int combo_get_rows( object_t *obj );

/**
 * \brief Selects a row in a Combo widget
 * 
 * \param obj A valid Combo widget object.
 * \param item A valid list item
 */
CLFEXP void combo_select_item( object_t *obj, list_item_t *item );

/**
 * \brief Removes all entries from a Combo widget
 * 
 * \param obj A valid Combo widget object.
 */
CLFEXP void combo_clear( object_t *obj );

/*\@}*/
#endif
