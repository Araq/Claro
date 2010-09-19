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

object_t *w, *i;
image_t *img;

void window_closed( object_t *btn, event_t *event ) {
	exit( 0 );
}

int main( int argc, char *argv[] )
{
	claro_base_init( );
	claro_graphics_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
	w = window_widget_create( 0, new_bounds( 100, 100, 155, 73 ), 0 );
	window_set_title( w, "Image test!" );
	object_addhandler( w, "destroy", window_closed );
	
	img = image_load( w, "logo.png" );
	i = image_widget_create_with_image( w, new_bounds(0, 0, 155, 73), 0, img );
	
	window_show( w );
	window_focus( w );
	
	block_heap_loginfo( );
	
	claro_loop( );
	
	return 0;
}
