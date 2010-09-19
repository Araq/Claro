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

void window_closed( object_t *btn, event_t *event ) {
    exit( 0 );
}

int main( int argc, char *argv[] )
{
	layout_t *lt, *lt2, *lt3, *lt4;
	bounds_t *b;
	object_t *rg, *btn, *fr;
	
	claro_base_init( );
	claro_graphics_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
	b = new_bounds( 100, 100, 460, 230 );
	w = window_widget_create( 0, b, 0 );
	object_addhandler( w, "destroy", window_closed );
	window_set_title( w, "Radio button example" );
	
	lt = layout_create( w, "[][_<|set1|<|set3|<][][_<|set2|<|set4|<][]", *b, 10, 10 );
	
	/* create a frame */
	b = lt_bounds(lt,"set1");
	fr = frame_widget_create_with_label( w, b, 0, "Group 1" );
	lt2 = layout_create( fr, "[r1][r2][r3]", *b, 20, 20 );
	
	rg = radiogroup_create( fr, 0 );
	
	btn = radiobutton_widget_create( fr, rg, lt_bounds(lt2,"r1"), "Button 1", 0 );
	btn = radiobutton_widget_create( fr, rg, lt_bounds(lt2,"r2"), "Button 2", 0 );
	btn = radiobutton_widget_create( fr, rg, lt_bounds(lt2,"r3"), "Button 3", 0 );
	
	/* create a frame */
	b = lt_bounds(lt,"set2");
	fr = frame_widget_create_with_label( w, b, 0, "Group 2" );
	lt3 = layout_create( fr, "[r1][r2][r3]", *b, 20, 20 );
	
	rg = radiogroup_create( fr, 0 );
	
	btn = radiobutton_widget_create( fr, rg, lt_bounds(lt3,"r1"), "Button 1", 0 );
	btn = radiobutton_widget_create( fr, rg, lt_bounds(lt3,"r2"), "Button 2", 0 );
	btn = radiobutton_widget_create( fr, rg, lt_bounds(lt3,"r3"), "Button 3", 0 );
	
	/* create a frame */
	b = lt_bounds(lt,"set3");
	fr = frame_widget_create_with_label( w, b, 0, "Group 3" );
	lt4 = layout_create( fr, "[r1][r2][r3]", *b, 20, 20 );
	
	btn = checkbox_widget_create_with_label( fr, lt_bounds(lt4,"r1"), 0, "Button 1" );
	btn = checkbox_widget_create_with_label( fr, lt_bounds(lt4,"r2"), 0, "Button 2" );
	btn = checkbox_widget_create_with_label( fr, lt_bounds(lt4,"r3"), 0, "Button 3" );
	
	window_show( w );
	window_focus( w );
	
	block_heap_loginfo( );
	
	claro_loop( );
	
	return 0;
}
