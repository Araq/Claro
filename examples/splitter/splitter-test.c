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
#include <assert.h>

object_t *lb = 0;

event_handler( hide_button )
{
	widget_hide( WIDGET( lb ) );
}

event_handler( show_button )
{
	widget_show( WIDGET( lb ) );
}

void window_closed( object_t *btn, event_t *event ) {
    exit(0);
}

int main( int argc, char *argv[] )
{
    object_t *w, *sw, *sw2, *cw;
    layout_t *lt;
    object_t *btn;
    bounds_t *b = NULL;
	
    claro_base_init( );
    claro_graphics_init( );
	
    log_fd_set_level( CL_DEBUG, stderr );
	
    clog( CL_INFO, "%s running using Claro!", __FILE__ );
    	
    b = new_bounds(50, 50, 300, 300);
    w = window_widget_create(NULL, b, 0);
    window_set_title(w, "Splitter Demo");
    object_addhandler(w, "destroy", window_closed);

    lt = layout_create(w, "[_splitter]", *b, 10, 10 );
    assert(lt != NULL && "failed to parse layout");
        
    sw = splitter_widget_create( w, lt_bounds(lt, "splitter"), cSplitterHorizontal );
    splitter_set_info( sw, cSplitterFirst, 0, 200 );
    splitter_set_info( sw, cSplitterSecond, 1, 0 );

    btn = button_widget_create( sw, NO_BOUNDS, 0 );
    lb = btn;
    button_set_text(btn, "Button Left");
	
    sw2 = splitter_widget_create( sw, NO_BOUNDS, cSplitterVertical );
	splitter_set_info( sw2, cSplitterFirst, 1, 0 );
    splitter_set_info( sw2, cSplitterSecond, 0, 200 );
	
    btn = button_widget_create( sw2, NO_BOUNDS, 0 );
    object_addhandler( btn, "pushed", show_button );
    button_set_text(btn, "Button Top-Right");
	
    cw = container_widget_create( sw2, NO_BOUNDS, 0 );
	
    lt = layout_create(cw,  "[_ul|(10)|ur][{10}][_ll|(10)|lr]", *b, 10, 10 );
	
    btn = button_widget_create( cw, lt_bounds( lt, "ul" ), 0 );
    object_addhandler( btn, "pushed", hide_button );
    button_set_text(btn, "B-U-L");
	
    btn = button_widget_create( cw, lt_bounds( lt, "ur" ), 0 );
    button_set_text(btn, "B-U-R");

    btn = button_widget_create( cw, lt_bounds( lt, "ll" ), 0 );
    button_set_text(btn, "B-L-L");
	
    btn = button_widget_create( cw, lt_bounds( lt, "lr" ), 0 );
    button_set_text(btn, "B-L-R");
	
    window_show( w );
    window_focus( w );
	
    block_heap_loginfo( );
	
    claro_loop( );
	
    return 0;
}
