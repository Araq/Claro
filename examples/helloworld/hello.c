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

object_t *w, *t, *b;

void push_my_button( object_t *obj, event_t *event )
{
	textbox_set_text( t, "You pushed my button!" );
	button_set_text( obj, "Ouch!" );
}

int main( int argc, char *argv[] )
{
	claro_base_init( );
	claro_graphics_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
	w = window_widget_create( 0, new_bounds( 100, 100, 230, 230 ), 0 );
	window_set_title( w, "Hello, World!" );
	
	t = textbox_widget_create( w, new_bounds( 10, 10, 210, -1 ), 0 );
	widget_set_notify( WIDGET(t), cNotifyKey );
	textbox_set_text( t, "Yeehaw!" );
	
	b = button_widget_create( w, new_bounds( 40, 45, 150, -1 ), 0 );
	button_set_text( b, "Push my button!" );
	
	object_addhandler( b, "pushed", push_my_button );
	
	window_show( w );
	window_focus( w );
	
	block_heap_loginfo( );
	
	claro_loop( );
	
	return 0;
}
