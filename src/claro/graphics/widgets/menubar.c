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

void menubar_new_item_handle( object_t *obj, event_t *event );
void menubar_remove_item_handle( object_t *obj, event_t *event );

object_t *menubar_widget_create( object_t *parent, int flags ) 
{
	window_widget_t *pwin = (window_widget_t *)parent;
	
	assert_valid_widget( parent, "parent" );
	
	object_t *obj = default_widget_create(parent, sizeof(menubar_widget_t), 
										   "claro.graphics.widgets.menubar", NULL, 
										   flags, cgraphics_menubar_widget_create);
	
	list_widget_init( obj, 2, CLIST_TYPE_PTR, CLIST_TYPE_STRING );
	
	/* handle list operations */
	object_addhandler( obj, "new_row", menubar_new_item_handle );
	object_addhandler( obj, "remove_row", menubar_remove_item_handle );
	
	pwin->menubar = obj;
	
	return obj;
}

void menubar_add_key_binding( object_t * menubar, list_item_t * item, const char * utf8_key, int modifier)
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_only_list_item( item, "item" );

	cgraphics_menubar_add_key_binding( WIDGET(menubar), item, utf8_key, modifier);
}

list_item_t *menubar_append_item( object_t *menubar, list_item_t *parent, image_t *image, char *title )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_only_image( image, "image" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_append( menubar, parent, image, title );
}

list_item_t *menubar_insert_item( object_t *menubar, list_item_t *parent, int pos, image_t *image, char *title )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_only_image( image, "image" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_insert( menubar, parent, pos, image, title );
}

list_item_t *menubar_append_separator( object_t *menubar, list_item_t *parent )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_append( menubar, parent, 0, 0 );
}

list_item_t *menubar_insert_separator( object_t *menubar, list_item_t *parent, int pos )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_insert( menubar, parent, pos, 0, 0 );
}

void menubar_new_item_handle( object_t *obj, event_t *event )
{
	list_widget_t *lw = (list_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	cgraphics_menubar_new_item( WIDGET(obj), item );
}

void menubar_move_item( object_t *menubar, list_item_t *item, int row )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_move( menubar, item, row );
}

void menubar_remove_item( object_t *menubar, list_item_t *item )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_remove( menubar, item );
}

void menubar_remove_item_handle( object_t *obj, event_t *event )
{
	list_widget_t *lw = (list_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	cgraphics_menubar_remove_item( WIDGET(obj), item );
}

int menubar_item_count( object_t *obj, list_item_t *parent )
{
	list_widget_t *lw = (list_widget_t *)obj;
	
	assert_valid_menubar_widget( obj, "obj" );
	assert_only_list_item( parent, "parent" );
	
	return LIST_LENGTH( &lw->items );
}

void menubar_disable_item( object_t *menubar, list_item_t *item )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_only_list_item( item, "item" );
	
	cgraphics_menubar_disable_item( WIDGET(menubar), item );
}

void menubar_enable_item( object_t *menubar, list_item_t *item )
{
	assert_valid_menubar_widget( menubar, "menubar" );
	assert_only_list_item( item, "item" );
	
	cgraphics_menubar_enable_item( WIDGET(menubar), item );
}

