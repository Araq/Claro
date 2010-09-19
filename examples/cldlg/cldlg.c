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


/**
 * cldlg -- A very simple xdialog replacement that lets you build the
 * dialog using LEL rather than command line options.
 *
 * You configure it to have text boxes or buttons, it fills in the text
 * based on arguments and names buttons after their cell names in the layout.
 * When a user clicks a button it exits with a number for the button cell.
 */

#include <claro/base.h>
#include <claro/graphics.h>
#include <assert.h>

enum { TYPE_WINDOW, TYPE_LAYOUT };

void claro_application_init()
{
    claro_base_init( );
    claro_graphics_init( );

    log_fd_set_level( CL_DEBUG, stderr );    
}


int claro_application_run(object_t *window, layout_t *lt)
{
    store_put("cldlg.window", window, sizeof(object_t), TYPE_WINDOW);
    store_put("cldlg.layout", lt, sizeof(layout_t), TYPE_LAYOUT);
    
    window_show( window );
    window_focus( window );
    
    claro_loop( );
    
    layout_destroy(lt);
    
    return 0;
}


int error(const char *msg, int err)
{
    clog(CL_ERROR, msg);

    if(err) {
        clog(CL_ERROR, strerror(err));
    }
    
    return 1;
}


void button_pushed( object_t *obj, event_t *event )
{
    int id = 0;
    button_widget_t *bw = (button_widget_t *)obj;
    id = store_take_int(bw->text);
    printf("%s %d\n", bw->text, id);
    exit(id);
}


void text_create(object_t *parent, bounds_t *bounds, char *name) {
    object_t *txt = NULL;
    
    txt = label_widget_create_with_text(parent, bounds, cLabelCenter, name );
}


void button_create(object_t *parent, bounds_t *bounds, char *name) {
    object_t *btn = NULL;
    btn = button_widget_create(parent, bounds, 0 );
    button_set_text(btn, name);
    object_addhandler(btn, "pushed", button_pushed);
}

int main( int argc, char *argv[] )
{
    size_t i = 0;
    int argi = 0;
    size_t size = 0;
    cell_t **cl = NULL;
    int min_w = 40, min_h = 30;
    bounds_t *b = NULL;
    object_t *window = NULL;
    layout_t *lt = NULL;
    
    if(argc < 2) {
        printf("usage:  cldlg <layout>\n");
        printf("cldlg will parse <layout> and build an interface that lets "
               "you interact with a user from a shell script.\n");
        return 1;
    }
    
    claro_application_init();

    b = new_bounds(50, 50, 300, 100);
    window = window_widget_create(NULL, b, 0);
    window_set_title(window, "Dialog");
    
    lt = layout_create(window, argv[1], *b, min_w, min_h);
    assert(lt != NULL && "failed to parse layout specification");
    
    if(!lt) {
        return error("Could not parse the interface", 0);
    }

    argi = 2;
    for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
        if(strlen(cl[i]->name) > 0) {
            /* this has contents, place widget that matches */
            if(cl[i]->name[0] == 'T' && cl[i]->name[1] == '.') {
                if(argc < argi) {
                    return error("Each text widget needs a string after the layout.", 0);
                } else {
                    text_create(window, &cl[i]->bounds, argv[argi]);
                    argi++;
                }
            } else {
                // we just put the buttons in play in the store so we can get them later
                store_put_int(cl[i]->name, i);
                button_create(window, &cl[i]->bounds, cl[i]->name);
            }
        }
    }

    return claro_application_run(window, lt);
}
