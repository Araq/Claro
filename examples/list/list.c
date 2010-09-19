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


#include <claro/base.h>
#include <claro/graphics.h>

object_t *btn_add, *btn_del, *btn_up, *btn_down;
object_t *t, *w, *e;

void window_closed( object_t *btn, event_t *event )
{
	exit( 0 );
}

void handle_text_changed( textbox_widget_t *obj, event_t *event )
{
	if ( !strcmp( obj->text, "" ) )
		widget_disable( btn_add );
	else
		widget_enable( btn_add );
}

void handle_add( object_t *obj, event_t *event )
{
	textbox_widget_t *entry = (textbox_widget_t *)e;
	listbox_append_row( t, entry->text );
	textbox_set_text( e, "" );
	widget_focus( e );
}

void handle_del( object_t *obj, event_t *event )
{
	listbox_remove_row( t, listbox_get_selected( t ) );
}

void handle_up( object_t *obj, event_t *event )
{
	list_item_t *item = listbox_get_selected( t );
	listbox_move_row( t, item, item->row - 1 );
	listbox_select_item( t, item );
}

void handle_down( object_t *obj, event_t *event )
{
	list_item_t *item = listbox_get_selected( t );
	listbox_move_row( t, item, item->row + 1 );
	listbox_select_item( t, item );
}

void handle_selected( object_t *obj, event_t *event )
{
	list_item_t *item;
	int r;
	
	item = listbox_get_selected( obj );
	r = listbox_get_rows( obj );
	
	if ( item == 0 )
	{
		widget_disable( btn_del );
		widget_disable( btn_up );
		widget_disable( btn_down );
		return;
	}
	
	widget_enable( btn_del );
	
	if ( item->row > 0 )
		widget_enable( btn_up );
	else
		widget_disable( btn_up );
	
	if ( item->row < r-1 )
		widget_enable( btn_down );
	else
		widget_disable( btn_down );
}

int main( int argc, char *argv[] )
{
	list_item_t *i;
	layout_t *lt;
	bounds_t *b;
	char tmp[64];
	int a;
	
	claro_base_init( );
	claro_graphics_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
	b = new_bounds( 100, 100, 400, 250 );
	w = window_widget_create( 0, b, 0 );
	object_addhandler( w, "destroy", window_closed );
	window_set_title( w, "List test" );
	
	lt = layout_create( w, "[{10}][(10)|text|(10)|>add|(10)|>del|(10)|>up|(10)|>down|(10)][{10}][_(10)|list|(10)][{10}]", *b, 45, 25 );
	
	t = listbox_widget_create( w, lt_bounds(lt,"list"), 0 );
	object_addhandler( t, "selected", handle_selected );
	
	e = textbox_widget_create( w, lt_bounds(lt,"text"), 0 );
	object_addhandler( e, "changed", handle_text_changed );
	
	btn_add = button_widget_create_with_label( w, lt_bounds(lt,"add"), 0, "Add" );
	object_addhandler( btn_add, "pushed", handle_add );
	btn_del = button_widget_create_with_label( w, lt_bounds(lt,"del"), 0, "Del" );
	object_addhandler( btn_del, "pushed", handle_del );
	btn_up = button_widget_create_with_label( w, lt_bounds(lt,"up"),  0, "Up" );
	object_addhandler( btn_up, "pushed", handle_up );
	btn_down = button_widget_create_with_label( w, lt_bounds(lt,"down"), 0, "Down" );
	object_addhandler( btn_down, "pushed", handle_down );
	
	handle_text_changed( e, 0 );
	handle_selected( t, 0 );
	
	window_show( w );
	window_focus( w );
	
	widget_focus( e );
	
	block_heap_loginfo( );
	
	claro_loop( );
	
	return 0;
}
