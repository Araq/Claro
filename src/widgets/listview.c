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

void listview_new_row_handle( object_t *obj, event_t *event );
void listview_remove_row_handle( object_t *obj, event_t *event );

void listview_handle_destroy( object_t *obj, event_t *e )
{
	int a;
	listview_widget_t *o;
	
	o = (listview_widget_t*)obj;
	for ( a = 0; a < o->widget.columns; a++ )
	{
		free( o->titles[a] );
	}
	
	free( o->titles );
}

object_t *listview_widget_create( object_t *parent, bounds_t *bounds, int columns, int flags, ... )
{
	va_list argp;
	int c;
	int cols[columns];
	listview_widget_t *lw;
	object_t *obj;
	
	va_start( argp, flags );
	
	assert_valid_widget( parent, "parent" );
	
	obj = default_widget_create(parent, sizeof(listview_widget_t), 
										   "claro.graphics.widgets.listview", bounds, 
										   flags, cgraphics_listview_widget_create);
	
	lw = (listview_widget_t *)obj;
	
	lw->titles = (char **)malloc( sizeof(char *) * columns );
	
	for ( c = 0; c < columns; c++ )
	{
		char *tmp = va_arg( argp, char * );
		int type = va_arg( argp,  int );
		
		lw->titles[c] = strdup( tmp );
		
		switch ( type )
		{
			case CLISTVIEW_TYPE_CHECKBOX:
				cols[c] = CLIST_TYPE_UINT;
				break;
			case CLISTVIEW_TYPE_PROGRESS:
				cols[c] = CLIST_TYPE_DOUBLE;
				break;
			default:
			case CLISTVIEW_TYPE_TEXT:
				cols[c] = CLIST_TYPE_STRING;
				break;
		}
	}
	
	//list_widget_init( obj, 1, CLIST_TYPE_STRING );
	list_widget_init_ptr( obj, columns, cols );
	cgraphics_listview_widget_create_columns( WIDGET(obj) );
	
	/* handle list operations */
	object_addhandler( obj, "new_row", listview_new_row_handle );
	object_addhandler( obj, "remove_row", listview_remove_row_handle );
	object_addhandler( obj, "destroy", listview_handle_destroy );
	
	va_end( argp );
	
	return obj;
}

list_item_t *listview_append_row( object_t *listview, ... )
{
	list_item_t *item;
	va_list argp;
	
	assert_valid_listview_widget( listview, "listview" );
	
	va_start( argp, listview );
	
	item = list_widget_row_insert_ptr( listview, 0, -1, argp );
	
	va_end( argp );
	
	return item;
}

list_item_t *listview_insert_row( object_t *listview, int pos, ... )
{
	list_item_t *item;
	va_list argp;
	
	assert_valid_listview_widget( listview, "listview" );
	
	va_start( argp, pos );
	
	item = list_widget_row_insert_ptr( listview, 0, pos, argp );
	
	va_end( argp );
	
	return item;
}

void listview_new_row_handle( object_t *obj, event_t *event )
{
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	cgraphics_listview_new_row( WIDGET(obj), item );
}

void listview_move_row( object_t *listview, list_item_t *item, int row )
{
	assert_valid_listview_widget( listview, "listview" );
	
	list_widget_row_move( listview, item, row );
}

void listview_remove_row( object_t *listview, list_item_t *item )
{
	assert_valid_listview_widget( listview, "listview" );
	
	list_widget_row_remove( listview, item );
}

void listview_remove_row_handle( object_t *obj, event_t *event )
{
	listview_widget_t *lw = (listview_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	if ( lw->selected == item )
	{
		lw->selected = 0;
		event_send( OBJECT(lw), "selected", "p", 0 );
	}
	
	cgraphics_listview_remove_row( WIDGET(obj), item );
}

list_item_t *listview_get_selected( object_t *obj )
{
	listview_widget_t *lw = (listview_widget_t *)obj;
	
	assert_valid_listview_widget( obj, "obj" );
	
	return lw->selected;
}

int listview_get_rows( object_t *obj )
{
	list_widget_t *lw = (list_widget_t *)obj;
	
	assert_valid_listview_widget( obj, "obj" );
	
	return LIST_LENGTH( &lw->items );
}

void listview_select_item( object_t *obj, list_item_t *item )
{
	assert_valid_listview_widget( obj, "obj" );
	cgraphics_listview_select_row( WIDGET(obj), item );
}
