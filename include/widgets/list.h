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


#ifndef _CLARO_GRAPHICS_LIST_H
#define _CLARO_GRAPHICS_LIST_H
/** \defgroup list_widget_t List Widget
 * \brief A base widget for other listable widgets
 * @{
 */

#define assert_only_list_item(o,n)	assert_type_check( o, n, "claro.graphics.list_item" )
#define assert_valid_list_item(o,n)	assert_not_null( o, n, "List Item" ); \
					assert_only_list_item( o, n )
enum
{
	CLIST_TYPE_PTR = 0,
	CLIST_TYPE_STRING,
	CLIST_TYPE_INT,
	CLIST_TYPE_UINT,
	CLIST_TYPE_DOUBLE,
};

/**
 * List items define items in a list_widget
 */
typedef struct list_item_
{
	object_t object;
	
	int row;
	
	void *native;
	size_t nativeid;
	
	object_t *menu;
	
	int enabled;
	
	void **data;
	
	list_t children;
	list_t *parent;
	struct list_item_ *parent_item;
	
	// drawing related info, not always required
	color_t text_color, sel_text_color;
	color_t back_color, sel_back_color;
	font_t font;
} list_item_t;

/**
 * List widget, a base for widgets containing items
 */
typedef struct list_widget_
{
	widget_t widget;
	
	int columns;
	int *coltypes;
	
	list_t items;
} list_widget_t;

/* functions */

/**
 * \brief Initialises a list_widget_t derivative's storage space.
 *
 * \param obj list widget
 * \param col_num number of columns to be used
 * \param cols An array of col_num integers, specifying the types of the columns.
 */
CLFEXP void list_widget_init_ptr( object_t *obj, unsigned int col_num, int *cols );

/**
 * \brief Copies and passes on the arg list to list_widget_init_ptr.
 *
 * \param obj list widget
 * \param col_num number of columns to be used
 * \param argpi A pointer to a va_list to parse
 */
CLFEXP void list_widget_init_vaptr( object_t *obj, unsigned int col_num, va_list argpi );

/**
 * Shortcut function, simply calls list_widget_init_ptr with
 * it's own arguments, and a pointer to the first variable argument.
 */
CLFEXP void list_widget_init( object_t *obj, unsigned int col_num, ... );

/**
 * \brief Inserts a row to a list under parent at the position specified.
 *
 * \param list list to insert item in
 * \param parent item in tree to be used as parent. NULL specifies
 *  that it should be a root node.
 * \param row item will be inserted before the item currently at
 *  this position. -1 specifies an append.
 * \param argp points to the first element of an array containing
 * the column data as specified by the types in list_widget_init.
 */
CLFEXP list_item_t *list_widget_row_insert_ptr( object_t *list, list_item_t *parent, int row, va_list argp );

/**
 * Shortcut function, calls list_widget_row_insert_ptr with
 * it's own arguments, a position at the end of the list, and
 * a pointer to the first variable argument.
 */
CLFEXP list_item_t *list_widget_row_append( object_t *list, list_item_t *parent, ... );

/**
 * Shortcut function, calls list_widget_row_insert_ptr with
 * it's own arguments, and a pointer to the first variable argument.
 */
CLFEXP list_item_t *list_widget_row_insert( object_t *list, list_item_t *parent, int pos, ... );

/**
 * \brief Removes a row from a list
 *
 * \param list List widget to operate on
 * \param item The item to remove
 */
CLFEXP void list_widget_row_remove( object_t *list, list_item_t *item );

/**
 * \brief Moves a row to a new position in the list
 *
 * \param list List widget to operate on
 * \param item The item to move 
 * \param row Row position to place item before. Passing the current
 *            position will result in no change.
 */
CLFEXP void list_widget_row_move( object_t *list, list_item_t *item, int row );

/**
 * \brief Return the nth row under parent in the list
 *
 * \param list List widget search
 * \param parent Parent of the item
 * \param row Row index of item to return
 */
CLFEXP list_item_t *list_widget_get_row( object_t *list, list_item_t *parent, int row );

/**
 * \brief Edit items of a row in the list.
 *
 * \param list List widget to edit
 * \param item Row to modify
 * \param args num,val,...,-1 where num is the column and val is the new 
 *             value of the column's type. Terminate with -1. Don't forget the -1.
 */
CLFEXP void list_widget_edit_row_ptr( object_t *list, list_item_t *item, va_list argp );

/**
 * \brief Edit items of a row in the list.
 *
 * \param list List-based (list_widget_t) object
 * \param item Row to modify
 * \param ... num,val,...,-1 where num is the column and val is the new 
 *             value of the column's type. Terminate with -1. Don't forget the -1.
 */
CLFEXP void list_widget_edit_row( object_t *list, list_item_t *item, ... );

/**
 * \brief Set the text color of an item.
 * This is currently only supported by the TreeView widget.
 *
 * \param item Target list item
 * \param r Red component between 0.0 and 1.0
 * \param g Green component between 0.0 and 1.0
 * \param b Blue component between 0.0 and 1.0
 * \param a Alpha component between 0.0 and 1.0 (reserved for future use, should be 1.0)
 */
CLFEXP void list_item_set_text_color( list_item_t *item, float r, float g, float b, float a );

/**
 * \brief Set the text background color of an item.
 * This is currently only supported by the TreeView widget.
 *
 * \param item Target list item
 * \param r Red component between 0.0 and 1.0
 * \param g Green component between 0.0 and 1.0
 * \param b Blue component between 0.0 and 1.0
 * \param a Alpha component between 0.0 and 1.0 (reserved for future use, should be 1.0)
 */
CLFEXP void list_item_set_text_bgcolor( list_item_t *item, float r, float g, float b, float a );

/**
 * \brief Set the text color of a selected item.
 * This is currently only supported by the TreeView widget.
 *
 * \param item Target list item
 * \param r Red component between 0.0 and 1.0
 * \param g Green component between 0.0 and 1.0
 * \param b Blue component between 0.0 and 1.0
 * \param a Alpha component between 0.0 and 1.0 (reserved for future use, should be 1.0)
 */
CLFEXP void list_item_set_sel_text_color( list_item_t *item, float r, float g, float b, float a );

/**
 * \brief Set the text background color of a selected item.
 * This is currently only supported by the TreeView widget.
 *
 * \param item Target list item
 * \param r Red component between 0.0 and 1.0
 * \param g Green component between 0.0 and 1.0
 * \param b Blue component between 0.0 and 1.0
 * \param a Alpha component between 0.0 and 1.0 (reserved for future use, should be 1.0)
 */
CLFEXP void list_item_set_sel_text_bgcolor( list_item_t *item, float r, float g, float b, float a );

/**
 * \brief Set the font details of the specified item.
 *
 * \param item Target list item
 * \param weight The weight of the font
 * \param slant The slant of the font
 * \param decoration Font decorations
 */
CLFEXP void list_item_set_font_extra( list_item_t *item, int weight, int slant, int decoration );

/*\@}*/
#endif
