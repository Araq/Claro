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

static object_t * w, *s;
static int is_shown = TRUE;

static void status_icon_pushed(object_t * status, event_t * event)
{
    if(is_shown)
    {
        window_hide(w);
        is_shown = FALSE;
    }
    else
    {
        window_show(w);
        is_shown = TRUE;
    }
}

static void window_closed(object_t *window, event_t *event) 
{
    claro_shutdown();
}


int main(int argc, char *argv[])
{
    image_t * img;
    object_t * menu;
    list_item_t * quit_item;
    
	claro_base_init();
	claro_graphics_init();
    	
	log_fd_set_level(CL_DEBUG, stderr);
	
	clog(CL_INFO, "%s running using Claro!", __FILE__);
	
	w = window_widget_create(0, new_bounds(100, 100, 230, 230), 0);
	object_addhandler(w, "destroy", window_closed);
	window_set_title(w, "Status Icon");
	
	menu = menu_widget_create(w, 0);
    quit_item = menu_append_item(menu, NULL, stock_get_image("system-log-out"), "Quit");
    object_addhandler(OBJECT(quit_item), "pushed", window_closed);

	img = image_load(w, "icon.png");
	s = status_icon_create(w, img, 0);
	status_icon_set_tooltip(s, "Claro - Status Icon");
	status_icon_set_menu(s, menu);
	object_addhandler(s, "pushed", status_icon_pushed);
	
	window_show(w);
	window_focus(w);
	
	block_heap_loginfo();
	
	claro_loop();
    
	return 0;
}

