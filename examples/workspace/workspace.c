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

object_t *w, *ws;

void window_closed( object_t *btn, event_t *event )
{
	exit( 0 );
}
/*
void combo_item_selected( object_t *obj, event_t *event )
{
	char *str = "";
	list_item_t *li = combo_get_selected( obj );
	
	if ( li != 0 )
		str = li->data[0];
	
	textbox_set_text( tbox, str );
}
*/
void new_window( object_t *o, event_t *e )
{
	widget_t *wsc, *fr, *tbox, *combo;
	layout_t *lt, *lt2;
	bounds_t *b;
	
	b = new_bounds( -1, -1, 200, 200 );
	wsc = workspace_window_widget_create( ws, b, 0 );
	
	/* add components here */
	lt = layout_create( wsc, "[][_<|frame|<][]", *b, 10, 10 );
	
	/* create a frame */
	b = lt_bounds(lt,"frame");
	fr = frame_widget_create_with_label( wsc, b, 0, "Combo example" );
	lt2 = layout_create( fr, "[combo][{10}][text]", *b, 25, 25 );
	
	combo = combo_widget_create( fr, lt_bounds( lt2, "combo" ), 0 );
	tbox  = textbox_widget_create( fr, lt_bounds( lt2, "text" ), 0 );
	
	combo_append_row( combo, "True" );
	combo_append_row( combo, "False" );
	combo_append_row( combo, "NULL" );
	combo_append_row( combo, "Candy" );
	
	//object_addhandler( combo, "selected", combo_item_selected );
	
	workspace_window_show( wsc );
}

int main( int argc, char *argv[] )
{
	layout_t *lt;
	bounds_t *b;
	object_t *fr, *m, *mi, *msi;
	
	claro_base_init( );
	claro_graphics_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
	b = new_bounds( 100, 100, 300, 300 );
	w = window_widget_create( 0, b, 0 );
	object_addhandler( w, "destroy", window_closed );
	window_set_title( w, "Workspace example" );
	
	lt = layout_create( w, "[_ws]", *b, 10, 10 );
	
	ws = workspace_widget_create( w, lt_bounds( lt, "ws" ), 0 );
	
	new_window( 0, 0 );
	
	m = menubar_widget_create( w, 0 );
	mi = menubar_append_item( m, 0, 0, "File" );
	msi = menubar_append_item( m, mi, 0, "New Window" );
	object_addhandler( msi, "pushed", new_window );
	menubar_append_separator( m, mi );
	msi = menubar_append_item( m, mi, 0, "Exit" );
	object_addhandler( msi, "pushed", window_closed );
	
	window_show( w );
	window_focus( w );
	
	block_heap_loginfo( );
	
	claro_loop( );
	
	return 0;
}
