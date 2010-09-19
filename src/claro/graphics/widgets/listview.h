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


#ifndef _CLARO_GRAPHICS_WIDGETS_LISTVIEW_H
#define _CLARO_GRAPHICS_WIDGETS_LISTVIEW_H
/** \defgroup listview_widget_t ListView Widget
 * \brief The ListView Widget
 * @{
 */

#define assert_only_listview_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.listview" )
#define assert_valid_listview_widget(o,n)	assert_not_null( o, n, "ListView" ); \
						assert_only_listview_widget( o, n )

enum
{
	cListViewTypeNone = 0,
	cListViewTypeText,
	cListViewTypeCheckBox,
	cListViewTypeProgress,
};

#define CLISTVIEW_TYPE_NONE cListViewTypeNone
#define CLISTVIEW_TYPE_TEXT cListViewTypeText
#define CLISTVIEW_TYPE_CHECKBOX cListViewTypeCheckBox
#define CLISTVIEW_TYPE_PROGRESS cListViewTypeProgress

// whole row checkboxes.. will we really need this? hmm.
#define cListViewRowCheckBoxes 1

typedef struct listview_widget_ listview_widget_t;

struct listview_widget_
{
	list_widget_t widget;
	
	char **titles;
	void *nativep;
	
	list_item_t *selected;
};

/* functions */

/**
 * \brief Creates a ListView widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \param columns The number of columns in the listview
 * \param ... specifies the titles and types of each column. 
 *            ("Enable",cListViewTypeCheckBox,"Title",cListViewTypeText,...)
 * \return A new ListView widget object.
 */
CLFEXP object_t *listview_widget_create( object_t *parent, bounds_t *bounds, int columns, int flags, ... );

/**
 * \brief Append a row to a ListView widget
 * 
 * \param listview A valid ListView widget object.
 * \param ... A list of values for each column
 * \return A new list item.
 */
CLFEXP list_item_t *listview_append_row( object_t *listview, ... );

/**
 * \brief Insert a row at the specified position into a ListView widget
 * 
 * \param listview A valid ListView widget object.
 * \param pos The index at which this item will be placed.
 * \param ... A list of values for each column
 * \return A new list item.
 */
CLFEXP list_item_t *listview_insert_row( object_t *listview, int pos, ... );

/**
 * \brief Move a row in a ListView widget
 * 
 * \param listview A valid ListView widget object.
 * \param item A valid list item
 * \param row New position to place this item
 */
CLFEXP void listview_move_row( object_t *listview, list_item_t *item, int row );

/**
 * \brief Remove a row from a ListView widget
 * 
 * \param listview A valid ListView widget object.
 * \param item A valid list item
 */
CLFEXP void listview_remove_row( object_t *listview, list_item_t *item );

/**
 * \brief Returns the currently selected ListView item
 * 
 * \param obj A valid ListView widget object.
 * \return The currently selected ListView item, or NULL if no item is selected.
 */
CLFEXP list_item_t *listview_get_selected( object_t *obj );

/**
 * \brief Returns the number of rows in a ListView widget
 * 
 * \param obj A valid ListView widget object.
 * \return Number of rows
 */
CLFEXP int listview_get_rows( object_t *obj );

/**
 * \brief Selects a row in a ListView widget
 * 
 * \param obj A valid ListView widget object.
 * \param item A valid list item
 */
CLFEXP void listview_select_item( object_t *obj, list_item_t *item );

/*\@}*/
#endif
