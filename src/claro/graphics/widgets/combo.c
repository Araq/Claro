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


#include <claro/graphics.h>
#include <claro/graphics/platform.h>

void combo_new_row_handle( object_t *obj, event_t *event );
void combo_remove_row_handle( object_t *obj, event_t *event );

object_t *combo_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	combo_widget_t *lw;
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = default_widget_create(parent, sizeof(combo_widget_t), 
										   "claro.graphics.widgets.combo", bounds, 
										   flags, cgraphics_combo_widget_create);
	
	lw = (combo_widget_t *)obj;
	
	list_widget_init( obj, 1, CLIST_TYPE_STRING );
	
	/* handle list operations */
	object_addhandler( obj, "new_row", combo_new_row_handle );
	object_addhandler( obj, "remove_row", combo_remove_row_handle );
	
	return obj;
}

list_item_t *combo_append_row( object_t *combo, char *text )
{
	assert_valid_combo_widget( combo, "combo" );
	
	list_widget_row_append( combo, 0, text );
}

list_item_t *combo_insert_row( object_t *combo, int pos, char *text )
{
	assert_valid_combo_widget( combo, "combo" );
	
	list_widget_row_insert( combo, 0, pos, text );
}

void combo_new_row_handle( object_t *obj, event_t *event )
{
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	cgraphics_combo_new_row( WIDGET(obj), item );
}

void combo_move_row( object_t *combo, list_item_t *item, int row )
{
	assert_valid_combo_widget( combo, "combo" );
	
	list_widget_row_move( combo, item, row );
}

void combo_remove_row( object_t *combo, list_item_t *item )
{
	assert_valid_combo_widget( combo, "combo" );
	
	list_widget_row_remove( combo, item );
}

void combo_remove_row_handle( object_t *obj, event_t *event )
{
	combo_widget_t *lw = (combo_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	if ( lw->selected == item )
	{
		lw->selected = 0;
		event_send( OBJECT(lw), "selected", "p", 0 );
	}
	
	cgraphics_combo_remove_row( WIDGET(obj), item );
}

list_item_t *combo_get_selected( object_t *obj )
{
	combo_widget_t *lw = (combo_widget_t *)obj;
	assert_valid_combo_widget( obj, "obj" );
	return lw->selected;
}

int combo_get_rows( object_t *obj )
{
	list_widget_t *lw = (list_widget_t *)obj;
	assert_valid_combo_widget( obj, "obj" );
	return LIST_LENGTH( &lw->items );
}

void combo_select_item( object_t *obj, list_item_t *item )
{
	assert_valid_combo_widget( obj, "obj" );
	cgraphics_combo_select_row( WIDGET(obj), item );
}

void combo_clear( object_t *obj )
{
	assert_valid_combo_widget( obj, "obj" );
	while ( combo_get_rows( obj ) > 0 )
		combo_remove_row( obj, list_widget_get_row( obj, 0, 0 ) );
}

