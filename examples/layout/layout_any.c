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

object_t *window, *wgt;
layout_t *lt = NULL;

void window_closed( object_t *btn, event_t *event ) {
    exit(0);
}


int main( int argc, char *argv[] )
{
    size_t i = 0;
    size_t size = 0;
    cell_t **cl = NULL;
    int min_w, min_h;
    bounds_t *b = NULL;
    b = new_bounds(50, 50, 0, 0);
    
    if(argc != 6) {
        printf("usage:  layout_any w h min_w min_h layout\nlayout can be anything and will be rendered with lots of buttons");
        return 1;
    }
    
    b->w = atoi(argv[1]);
    b->h = atoi(argv[2]);
    min_w = atoi(argv[3]);
    min_h = atoi(argv[4]);
    
    printf("w: %d, h: %d, min_w: %d, min_h: %d\n", b->w, b->h, min_w, min_h);
    
    claro_base_init( );
    claro_graphics_init( );

    log_fd_set_level( CL_DEBUG, stderr );
    
    
    clog( CL_INFO, "%s running using Claro!", __FILE__ );    
    
    window = window_widget_create(NULL, b, 0);
    window_set_title(window, "Layout Anything");
    object_addhandler(window, "destroy", window_closed);

    lt = layout_create(window, argv[5], *b, min_w, min_h);
    
    if(!lt) {
        printf("- error parsing\n");
        return 1;
    }

    layout_dump(lt);
        
    for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
        if(strlen(cl[i]->name) > 0) {
            if(cl[i]->name[0] == 't') {
                wgt = textbox_widget_create(window, &cl[i]->bounds, 0 );
                textbox_set_text( wgt, cl[i]->name );
            } else {
                wgt = button_widget_create( window, &cl[i]->bounds, 0 );
                button_set_text(wgt, cl[i]->name);
            }
        }
    }
    
    window_show( window );
    window_focus( window );
    
    claro_loop( );
	
    layout_destroy(lt);
    
    return 0;
}
