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


#ifndef _CLARO_GRAPHICS_WIDGETS_TOOLBAR_H
#define _CLARO_GRAPHICS_WIDGETS_TOOLBAR_H
/** \defgroup toolbar_widget_t ToolBar Widget
 * \brief The ToolBar Widget
 * @{
 */

#define assert_only_toolbar_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.toolbar" )
#define assert_valid_toolbar_widget(o,n)	assert_not_null( o, n, "ToolBar" ); \
						assert_only_toolbar_widget( o, n )

#define cToolbarShowText 1
#define cToolbarShowImages 2
#define cToolbarShowBoth 3
#define cToolbarAutoSizeButtons 4

typedef struct toolbar_widget_ toolbar_widget_t;

struct toolbar_widget_
{
	list_widget_t widget;
};


/**
 * \brief Creates a ToolBar widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param flags Widget flags.
 * \return A new ToolBar widget object.
 */
CLFEXP object_t *toolbar_widget_create( object_t *parent, int flags );

/**
 * \brief Append a row to a ToolBar widget
 * 
 * \param toolbar A valid ToolBar widget object.
 * \param image An image object, or NULL.
 * \param title A string title, or NULL.
 * \param tooltip A string tooltip, or NULL.
 * \return A new list item.
 */
CLFEXP list_item_t *toolbar_append_icon( object_t *toolbar, image_t *image, char *title, char *tooltip );

/**
 * \brief Insert a row into a ToolBar widget
 * 
 * \param toolbar A valid ToolBar widget object.
 * \param pos The position at which to insert this item
 * \param image An image object, or NULL.
 * \param title A string title, or NULL.
 * \param tooltip A string tooltip, or NULL.
 * \return A new list item.
 */
CLFEXP list_item_t *toolbar_insert_icon( object_t *toolbar, int pos, image_t *image, char *title, char *tooltip );

/**
 * \brief Append a separator to a ToolBar widget
 * 
 * \param toolbar A valid ToolBar widget object.
 * \return A new list item.
 */
CLFEXP list_item_t *toolbar_append_separator( object_t *toolbar );

/**
 * \brief Insert a separator into a ToolBar widget
 * 
 * \param toolbar A valid ToolBar widget object.
 * \param pos The position at which to insert this item
 * \return A new list item.
 */
CLFEXP list_item_t *toolbar_insert_separator( object_t *toolbar, int pos );

/**
 * \brief Assign a menu widget to an item.
 *
 * This will show a small down arrow next to the item
 * that will open this menu.
 * 
 * \param toolbar A valid ToolBar widget object.
 * \param item Toolbar item the menu is for.
 * \param menu Menu widget object, or NULL to remove a menu.
 */
CLFEXP void toolbar_set_item_menu( object_t *toolbar, list_item_t *item, object_t *menu );

/*
CLFEXP list_item_t *toolbar_append_popup_item( object_t *toolbar, list_item_t *parent, image_t *image, char *title, char *tooltip );
CLFEXP list_item_t *toolbar_insert_popup_item( object_t *toolbar, list_item_t *parent, int pos, image_t *image, char *title, char *tooltip );
*/

/**
 * \brief Move a row in a ToolBar widget
 * 
 * \param toolbar A valid ToolBar widget object.
 * \param item A valid list item
 * \param row New position to place this item
 */
CLFEXP void toolbar_move_icon( object_t *toolbar, list_item_t *item, int row );

/**
 * \brief Remove a row from a ToolBar widget
 * 
 * \param toolbar A valid ToolBar widget object.
 * \param item A valid list item
 */
CLFEXP void toolbar_remove_icon( object_t *toolbar, list_item_t *item );

/**
 * \brief Returns the number of rows in a ToolBar widget
 * 
 * \param obj A valid ToolBar widget object.
 * \return Number of rows
 */
CLFEXP int toolbar_item_count( object_t *obj );

/*\@}*/
#endif
