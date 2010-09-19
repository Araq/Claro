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

object_t *window, *input, *chat, *send_btn;
layout_t *lt;

void push_my_button( object_t *obj, event_t *event )
{
}

void window_closed( object_t *btn, event_t *event ) {
    exit(0);
}


int main( int argc, char *argv[] )
{
    bounds_t *b = NULL;

    if(argc != 2) {
        printf("usage:  myapp layout\n");
        printf("Must have textbox, send, input cells\n");
        return 1;
    }
    
    claro_base_init( );
    claro_graphics_init( );

    log_fd_set_level( CL_DEBUG, stderr );
    
    
    clog( CL_INFO, "%s running using Claro!", __FILE__ );    
    
    b = new_bounds(50, 50, 400, 500);
    window = window_widget_create(NULL, b, 0);
    window_set_title(window, "My Simple Application");
    object_addhandler(window, "destroy", window_closed);

    lt = layout_create(window, argv[1], *b, 30, 40);
    assert(lt != NULL && "failed to parse layout specification");

    chat = textbox_widget_create(window, lt_bounds(lt, "textbox"), 0 );
    widget_set_notify( WIDGET(chat), cNotifyKey );
    textbox_set_text( chat, "> Welcome." );
    
    send_btn = button_widget_create( window, lt_bounds(lt, "send"), 0 );
    button_set_text(send_btn, "Send" );

    object_addhandler(send_btn, "pushed", push_my_button );

    input = textbox_widget_create(window, lt_bounds(lt, "input"), 0);
    widget_set_notify( WIDGET(input), cNotifyKey );
    
    window_show( window );
    window_focus( window );
    
    block_heap_loginfo( );
        
    claro_loop( );
	
    layout_destroy(lt);
    
    return 0;
}
