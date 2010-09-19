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

object_t *w;

void window_closed( object_t *btn, event_t *event )
{
	exit( 0 );
}

void add_stock()
{
	image_t * img = stock_get_image("edit-copy");
	image_widget_create_with_image( w, new_bounds(0,0,22,22), 0, img );
	img = stock_get_image("edit-paste");
	image_widget_create_with_image( w, new_bounds(22,0,22,22), 0, img );
}

int main( int argc, char *argv[] )
{
	claro_base_init( );
	claro_graphics_init( );
	
	bounds_t * b = new_bounds( 100, 100, 200, 200 );
	w = window_widget_create( 0, b, 0 );
	
	add_stock();
	
	object_addhandler( w, "destroy", window_closed );
	window_set_title( w, "Stock test" );
	
	window_show( w );
	window_focus( w );
	
	claro_loop( );
	
	return 0;
}
