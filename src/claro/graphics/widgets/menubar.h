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


#ifndef _CLARO_GRAPHICS_WIDGETS_MENUBAR_H
#define _CLARO_GRAPHICS_WIDGETS_MENUBAR_H
/** \defgroup menubar_widget_t MenuBar Widget
 * \brief The MenuBar Widget
 * @{
 */

#define assert_only_menubar_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.menubar" )
#define assert_valid_menubar_widget(o,n)	assert_not_null( o, n, "MenuBar" ); \
						assert_only_menubar_widget( o, n )

typedef struct menubar_widget_ menubar_widget_t;

/*
 *  Menu modifiers
 */

#define 	cModifierShift		1 << 0
#define	    cModifierCommand	1 << 1

struct menubar_widget_
{
	list_widget_t widget;
};



/**
 * \brief Creates a MenuBar widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param flags Widget flags.
 * \return A new MenuBar widget object.
 */
CLFEXP object_t *menubar_widget_create( object_t *parent, int flags );

/**
 * \brief Add a key binding to a menu items
 * 
 * \param menubar A valid MenuBar widget object.
 * \param item The item
 * \param utf8_key The key to use, NOT NULL.
 * \param modifier The modifier key, or 0.
 */
CLFEXP void menubar_add_key_binding( object_t * menubar, list_item_t * item, const char * utf8_key, int modifier);

/**
 * \brief Append a row to a MenuBar widget
 * 
 * \param menubar A valid MenuBar widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \param image An image object, or NULL.
 * \param title A string title, or NULL.
 * \return A new list item.
 */
CLFEXP list_item_t *menubar_append_item( object_t *menubar, list_item_t *parent, image_t *image, char *title );

/**
 * \brief Insert a row into a MenuBar widget
 * 
 * \param menubar A valid MenuBar widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \param pos The position at which to insert this item
 * \param image An image object, or NULL.
 * \param title A string title, or NULL.
 * \return A new list item.
 */
CLFEXP list_item_t *menubar_insert_item( object_t *menubar, list_item_t *parent, int pos, image_t *image, char *title );

/**
 * \brief Append a separator to a MenuBar widget
 * 
 * \param menubar A valid MenuBar widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \return A new list item.
 */
CLFEXP list_item_t *menubar_append_separator( object_t *menubar, list_item_t *parent );

/**
 * \brief Insert a separator into a MenuBar widget
 * 
 * \param menubar A valid MenuBar widget object.
 * \param parent The item to place the new item under, or NULL for a root item.
 * \param pos The position at which to insert this item
 * \return A new list item.
 */
CLFEXP list_item_t *menubar_insert_separator( object_t *menubar, list_item_t *parent, int pos );

/**
 * \brief Move a row in a MenuBar widget
 * 
 * \param menubar A valid MenuBar widget object.
 * \param item A valid list item
 * \param row New position to place this item
 */
CLFEXP void menubar_move_item( object_t *menubar, list_item_t *item, int row );

/**
 * \brief Remove a row from a MenuBar widget
 * 
 * \param menubar A valid MenuBar widget object.
 * \param item A valid list item
 */
CLFEXP void menubar_remove_item( object_t *menubar, list_item_t *item );

/**
 * \brief Returns the number of rows in a MenuBar widget
 * 
 * \param obj A valid MenuBar widget object.
 * \param parent Item whose children count to return, or NULL for root item count.
 * \return Number of rows
 */
CLFEXP int menubar_item_count( object_t *obj, list_item_t *parent );

/**
 * \brief Disables a menu item (no focus and greyed out)
 * 
 * \param menubar A valid MenuBar widget object.
 * \param item A valid list item
 */
CLFEXP void menubar_disable_item( object_t *menubar, list_item_t *item );

/**
 * \brief Enables a menu item (allows focus and not greyed out)
 * 
 * \param menubar A valid MenuBar widget object.
 * \param item A valid list item
 */
CLFEXP void menubar_enable_item( object_t *menubar, list_item_t *item );

/*\@}*/
#endif
