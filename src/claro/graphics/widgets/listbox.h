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


#ifndef _CLARO_GRAPHICS_WIDGETS_LISTBOX_H
#define _CLARO_GRAPHICS_WIDGETS_LISTBOX_H
/** \defgroup listbox_widget_t ListBox Widget
 * \brief The ListBox Widget
 * @{
 */

#define assert_only_listbox_widget(o,n)		assert_type_check( o, n, "claro.graphics.widgets.listbox" )
#define assert_valid_listbox_widget(o,n)	assert_not_null( o, n, "ListBox" ); \
						assert_only_listbox_widget( o, n )


typedef struct listbox_widget_ listbox_widget_t;

struct listbox_widget_
{
	list_widget_t widget;
	
	list_item_t *selected;
};

/* functions */

/**
 * \brief Creates a ListBox widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new ListBox widget object.
 */
CLFEXP object_t *listbox_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Insert a row at the specified position into a ListBox widget
 * 
 * \param listbox A valid ListBox widget object.
 * \param pos The index at which this item will be placed.
 * \param text The text for the item.
 * \return A new list item.
 */
CLFEXP list_item_t *listbox_insert_row( object_t *listbox, int pos, char *text );

/**
 * \brief Append a row to a ListBox widget
 * 
 * \param listbox A valid ListBox widget object.
 * \param text The text for the item.
 * \return A new list item.
 */
CLFEXP list_item_t *listbox_append_row( object_t *listbox, char *text );

/**
 * \brief Move a row in a ListBox widget
 * 
 * \param listbox A valid ListBox widget object.
 * \param item A valid list item
 * \param row New position to place this item
 */
CLFEXP void listbox_move_row( object_t *listbox, list_item_t *item, int row );

/**
 * \brief Remove a row from a ListBox widget
 * 
 * \param listbox A valid ListBox widget object.
 * \param item A valid list item
 */
CLFEXP void listbox_remove_row( object_t *listbox, list_item_t *item );

/**
 * \brief Returns the currently selected ListBox item
 * 
 * \param obj A valid ListBox widget object.
 * \return The currently selected ListBox item, or NULL if no item is selected.
 */
CLFEXP list_item_t *listbox_get_selected( object_t *obj );

/**
 * \brief Returns the number of rows in a ListBox widget
 * 
 * \param obj A valid ListBox widget object.
 * \return Number of rows
 */
CLFEXP int listbox_get_rows( object_t *obj );

/**
 * \brief Selects a row in a ListBox widget
 * 
 * \param obj A valid ListBox widget object.
 * \param item A valid list item
 */
CLFEXP void listbox_select_item( object_t *obj, list_item_t *item );

/*\@}*/
#endif
