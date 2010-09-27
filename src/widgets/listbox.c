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


#include "graphics.h"
#include "platform.h"

void listbox_new_row_handle( object_t *obj, event_t *event );
void listbox_remove_row_handle( object_t *obj, event_t *event );

object_t *listbox_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	listbox_widget_t *lw;
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = default_widget_create(parent, sizeof(listbox_widget_t), 
										   "claro.graphics.widgets.listbox", bounds, 
										   flags, cgraphics_listbox_widget_create);
	
	lw = (listbox_widget_t *)obj;
	
	list_widget_init( obj, 1, CLIST_TYPE_STRING );
	
	/* handle list operations */
	object_addhandler( obj, "new_row", listbox_new_row_handle );
	object_addhandler( obj, "remove_row", listbox_remove_row_handle );
	
	return obj;
}

list_item_t *listbox_append_row( object_t *listbox, char *text )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	return list_widget_row_append( listbox, 0, text );
}

list_item_t *listbox_insert_row( object_t *listbox, int pos, char *text )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	return list_widget_row_insert( listbox, 0, pos, text );
}

void listbox_new_row_handle( object_t *obj, event_t *event )
{
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	cgraphics_listbox_new_row( WIDGET(obj), item );
}

void listbox_move_row( object_t *listbox, list_item_t *item, int row )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	list_widget_row_move( listbox, item, row );
}

void listbox_remove_row( object_t *listbox, list_item_t *item )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	list_widget_row_remove( listbox, item );
}

void listbox_remove_row_handle( object_t *obj, event_t *event )
{
	listbox_widget_t *lw = (listbox_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	if ( lw->selected == item )
	{
		lw->selected = 0;
		event_send( OBJECT(lw), "selected", "p", 0 );
	}
	
	cgraphics_listbox_remove_row( WIDGET(obj), item );
}

list_item_t *listbox_get_selected( object_t *obj )
{
	listbox_widget_t *lw = (listbox_widget_t *)obj;
	assert_valid_listbox_widget( obj, "obj" );
	return lw->selected;
}

int listbox_get_rows( object_t *obj )
{
	list_widget_t *lw = (list_widget_t *)obj;
	assert_valid_listbox_widget( obj, "obj" );
	return LIST_LENGTH( &lw->items );
}

void listbox_select_item( object_t *obj, list_item_t *item )
{
	assert_valid_listbox_widget( obj, "obj" );
	assert_valid_list_item( item, "item" );
	
	cgraphics_listbox_select_row( WIDGET(obj), item );
}
