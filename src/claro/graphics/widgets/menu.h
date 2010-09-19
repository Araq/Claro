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


#ifndef _CLARO_GRAPHICS_WIDGETS_MENU_H
#define _CLARO_GRAPHICS_WIDGETS_MENU_H
/** \defgroup menu_widget_t Menu Widget
 * \brief The Menu Widget
 * @{
 */

#define assert_only_menu_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.menu" )
#define assert_valid_menu_widget(o,n)	assert_not_null( o, n, "Menu" ); \
					assert_only_menu_widget( o, n )

typedef struct menu_widget_ menu_widget_t;

#define cMenuPopupAtCursor 1

struct menu_widget_
{
	list_widget_t widget;
};

/**
 * \brief Creates a Menu widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param flags Widget flags.
 * \return A new Menu widget object.
 */
CLFEXP object_t *menu_widget_create( object_t *parent, int flags );

/**
 * \brief Append a row to a Menu widget
 * 
 * \param menu A valid Menu widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \param image An image object, or NULL.
 * \param title A string title, or NULL.
 * \return A new list item.
 */
CLFEXP list_item_t *menu_append_item( object_t *menu, list_item_t *parent, image_t *image, char *title );

/**
 * \brief Insert a row into a Menu widget
 * 
 * \param menu A valid Menu widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \param pos The position at which to insert this item
 * \param image An image object, or NULL.
 * \param title A string title, or NULL.
 * \return A new list item.
 */
CLFEXP list_item_t *menu_insert_item( object_t *menu, list_item_t *parent, int pos, image_t *image, char *title );

/**
 * \brief Append a separator to a Menu widget
 * 
 * \param menu A valid Menu widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \return A new list item.
 */
CLFEXP list_item_t *menu_append_separator( object_t *menu, list_item_t *parent );

/**
 * \brief Insert a separator into a Menu widget
 * 
 * \param menu A valid Menu widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \param pos The position at which to insert this item
 * \return A new list item.
 */
CLFEXP list_item_t *menu_insert_separator( object_t *menu, list_item_t *parent, int pos );

/**
 * \brief Move a row in a Menu widget
 * 
 * \param menu A valid Menu widget object.
 * \param item A valid list item
 * \param row New position to place this item
 */
CLFEXP void menu_move_item( object_t *menu, list_item_t *item, int row );

/**
 * \brief Remove a row from a Menu widget
 * 
 * \param menu A valid Menu widget object.
 * \param item A valid list item
 */
CLFEXP void menu_remove_item( object_t *menu, list_item_t *item );

/**
 * \brief Returns the number of rows in a Menu widget
 * 
 * \param obj A valid Menu widget object.
 * \param parent Item whose children count to return, or NULL for root item count.
 * \return Number of rows
 */
CLFEXP int menu_item_count( object_t *obj, list_item_t *parent );

/**
 * \brief Disables a menu item (no focus and greyed out)
 * 
 * \param menu A valid Menu widget object.
 * \param item A valid list item
 */
CLFEXP void menu_disable_item( object_t *menu, list_item_t *item );

/**
 * \brief Enables a menu item (allows focus and not greyed out)
 * 
 * \param menu A valid Menu widget object.
 * \param item A valid list item
 */
CLFEXP void menu_enable_item( object_t *menu, list_item_t *item );

/**
 * \brief Pops up the menu at the position specified
 * 
 * \param menu A valid Menu widget object.
 * \param x The X position
 * \param y The Y position
 * \param flags Flags
 */
CLFEXP void menu_popup( object_t *menu, int x, int y, int flags );

/*\@}*/
#endif
