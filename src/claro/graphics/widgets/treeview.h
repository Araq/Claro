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


#ifndef _CLARO_GRAPHICS_WIDGETS_TREEVIEW_H
#define _CLARO_GRAPHICS_WIDGETS_TREEVIEW_H
/** \defgroup treeview_widget_t TreeView Widget
 * \brief The TreeView Widget
 * @{
 */

typedef struct treeview_widget_ treeview_widget_t;

#define assert_only_treeview_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.treeview" )
#define assert_valid_treeview_widget(o,n)	assert_not_null( o, n, "TreeView Widget" ); \
						assert_only_treeview_widget( o, n )

/**
 * \brief TreeView widget
 */
struct treeview_widget_
{
	list_widget_t widget;
	
	list_item_t *selected;
};

/* functions */

/**
 * \brief Creates a TreeView widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new TreeView widget object.
 */
CLFEXP object_t *treeview_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Append a row to a TreeView
 * 
 * \param treeview A valid TreeView widget object.
 * \param parent The item under which to place the new item, or NULL for a root node.
 * \param image An image to go to the left of the item, or NULL for no image.
 * \param title The text for the item.
 * \return A new list item.
 */
CLFEXP list_item_t *treeview_append_row( object_t *treeview, list_item_t *parent, image_t *image, char *title );

/**
 * \brief Insert a row at the specified position into a TreeView
 * 
 * \param treeview A valid TreeView widget object.
 * \param parent The item under which to place the new item, or NULL for a root node.
 * \param pos The index at which this item will be placed.
 * \param image An image to go to the left of the item, or NULL for no image.
 * \param title The text for the item.
 * \return A new list item.
 */
CLFEXP list_item_t *treeview_insert_row( object_t *treeview, list_item_t *parent, int pos, image_t *image, char *title );

/**
 * \brief Move a row in a TreeView
 * 
 * \param treeview A valid TreeView widget object.
 * \param item A valid list item
 * \param row New position to place this item
 */
CLFEXP void treeview_move_row( object_t *treeview, list_item_t *item, int row );

/**
 * \brief Remove a row from a TreeView
 * 
 * \param treeview A valid TreeView widget object.
 * \param item A valid list item
 */
CLFEXP void treeview_remove_row( object_t *treeview, list_item_t *item );

/**
 * \brief Expand a row in a TreeView
 * 
 * \param treeview A valid TreeView widget object.
 * \param item A valid list item
 */
CLFEXP void treeview_expand( object_t *treeview, list_item_t *item );

/**
 * \brief Collapse a row in a TreeView
 * 
 * \param treeview A valid TreeView widget object.
 * \param item A valid list item
 */
CLFEXP void treeview_collapse( object_t *treeview, list_item_t *item );

/**
 * \brief Returns the currently selected TreeView item
 * 
 * \param obj A valid TreeView widget object.
 * \return The currently selected TreeView item, or NULL if no item is selected.
 */
CLFEXP list_item_t *treeview_get_selected( object_t *obj );

/**
 * \brief Returns the number of rows in a TreeView
 * 
 * \param obj A valid TreeView widget object.
 * \param parent Return the number of children of this item, or the number of
 *               root items if NULL
 * \return Number of rows
 */
CLFEXP int treeview_get_rows( object_t *obj, list_item_t *parent );

/**
 * \brief Selects a row in a TreeView
 * 
 * \param obj A valid TreeView widget object.
 * \param item A valid list item
 */
CLFEXP void treeview_select_item( object_t *obj, list_item_t *item );

/*\@}*/

#endif
