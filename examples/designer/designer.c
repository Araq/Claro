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

enum { TYPE_TEXTBOX=CL_TYPE_UNKNOWN+1, TYPE_WINDOW, TYPE_BOUNDS};

GEN_PUT_TYPE_DEFINITION(bounds_t, TYPE_BOUNDS);
GEN_TAKE_TYPE_DEFINITION(bounds_t, TYPE_BOUNDS);
GEN_GET_TYPE_DEFINITION(bounds_t, TYPE_BOUNDS);

object_t *status = NULL;
int has_toolbar = 0;
int has_statusbar = 0;

layout_t *make_layout(object_t *parent, bounds_t *b, const char *text) 
{
	layout_t *lt = NULL;
	object_t *wgt = NULL;
	size_t i, size;
	cell_t **cl = NULL;
		
	lt = layout_create(parent, text, *b, 30, 30);
	
	if(lt) {
		// setup the toolbar or status bar based on settings
		if(has_toolbar) {
			wgt = toolbar_widget_create(parent, 0);
			/*toolbar_add_button(wgt, NULL, "sample", "examples/designer/icon.png");
			toolbar_add_button(wgt, NULL, "sample", "examples/designer/icon.png");
			toolbar_add_button(wgt, NULL, "sample", "examples/designer/icon.png");
			toolbar_add_button(wgt, NULL, "sample", "examples/designer/icon.png");*/
		}
		
		if(has_statusbar) {
			// statusbar
			wgt = statusbar_widget_create(parent, 0);
			statusbar_set_text(wgt, "Status Good");
		}
		
		for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
			if(strlen(cl[i]->name) > 0) {
				if(!strncmp(cl[i]->name, "BT.", 3)) {
					// button
					wgt = button_widget_create(parent, &cl[i]->bounds, 0 );
					button_set_text(wgt, cl[i]->name+3);
				} else if(!strncmp(cl[i]->name, "CV.", 3)) {
					// canvas (just a label for now)
					wgt = label_widget_create_with_text(parent, &cl[i]->bounds, cLabelCenter, cl[i]->name+3);
				} else if(!strncmp(cl[i]->name, "PB.", 3)) {
					// progressbar
					wgt = progress_widget_create(parent, &cl[i]->bounds, cProgressLeftRight);
					progress_set_level(wgt, 0.5);
				} else if(!strncmp(cl[i]->name, "LB.", 3)) {
					// label
					wgt = label_widget_create_with_text(parent, &cl[i]->bounds, 0, cl[i]->name+3 );
				} else if(!strncmp(cl[i]->name, "CT.", 3)) {
					// container
					wgt = container_widget_create (parent, &cl[i]->bounds, 0);
				} else if(!strncmp(cl[i]->name, "TX.", 3)) {
					// textbox
					wgt = textbox_widget_create(parent, &cl[i]->bounds, 0 );
					textbox_set_text( wgt, cl[i]->name+3 );
				} else {
					// default to a button
					wgt = button_widget_create(parent, &cl[i]->bounds, 0 );
					button_set_text(wgt, cl[i]->name);
				}
			}
			
			statusbar_set_text(status, "Layout parsed");
		}
	} else {
		statusbar_set_text(status, "ERROR: Failed to parse your lelex");
	}
	
	return lt;
}

void window_destroy_handle(object_t *obj, event_t *evt)
{
	size_t len;
	int type;
	store_take("display.window", &len, &type);
}

void window_moved_handle(object_t *obj, event_t *evt)
{
	widget_t *window = (widget_t *)obj;
	store_take_bounds_t("window.bounds");
	store_put_bounds_t("window.bounds", window->size);
}

void has_toolbar_handle(object_t *obj, event_t *evt)
{
	has_toolbar = !has_toolbar;
}

void has_statusbar_handle(object_t *obj, event_t *evt)
{
	has_statusbar = !has_statusbar;
}

void parse_pushed(object_t *obj, event_t *evt)
{
	object_t *window = NULL;
	bounds_t *b;
	object_t *lelex = NULL;
	size_t len;
	int type;
	
	lelex = (object_t *)store_get("lelex", &len, &type);
	assert(len == sizeof(textbox_widget_t) && type == TYPE_TEXTBOX && "invalid object in store for lelex");
	
	// first kill the old window if it is still around
	window = store_take("display.window", &len, &type);
	if(window) {
		window_hide(window);
		event_send(window, "destroy", "");
	}

	b = new_bounds(0,0,0,0);
	*b = store_get_bounds_t("window.bounds");
	
	window = window_widget_create(NULL, b, 0);
	window_set_title(window, "Layout Display");
	object_addhandler(window, "destroy", window_destroy_handle);
	object_addhandler(window, "moved", window_moved_handle);
	object_addhandler(window, "resized", window_moved_handle);
	
	if(!make_layout(window, b, textarea_get_text(lelex))) {
		event_send(window, "destroy", "");
		printf("FAILED TO PARSE");
	} else {
		store_put("display.window", window, sizeof(window_widget_t), TYPE_WINDOW);
		window_show( window );
		window_focus( window );
	}
}

void window_closed( object_t *btn, event_t *event ) {
	exit(0);
}


static const char *main_layout = "[][_<|lelex|<|options(150)|<][]";


int main( int argc, char *argv[] )
{
	object_t *window = NULL;
	layout_t *lt = NULL, *lt2 = NULL, *lt3 = NULL;
	object_t *wgt = NULL, *frm = NULL;
	object_t *btn = NULL;

	bounds_t *b = NULL;
	
	claro_base_init( );
	claro_graphics_init( );

	log_fd_set_level( CL_DEBUG, stderr );	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );	
	
	b = new_bounds( 100, 100, 800, 300 );
	window = window_widget_create(NULL, b, 0);
	window_set_title(window, "Claro Designer");
	object_addhandler(window, "destroy", window_closed);

	lt = layout_create(window, main_layout, *b, 10, 10);
	assert(lt != NULL && "Failed to parse the main layout.");

	b = lt_bounds(lt, "lelex");
	wgt = frame_widget_create_with_label(window, b, 0, "Lelex");
	lt2 = layout_create(wgt, "[_lelex]", *b, 10, 10);
	wgt = textarea_widget_create(wgt, lt_bounds(lt2, "lelex"), 0);
	store_put("lelex", wgt, sizeof(textbox_widget_t), TYPE_TEXTBOX);
	
	b = lt_bounds(lt, "options");
	frm = frame_widget_create_with_label(window, b, 0, "Options");
	lt3 = layout_create(frm, "[chk.toolbar][chk.statusbar][_][{25}run]", *b, 20, 20);
	wgt = checkbox_widget_create_with_label(frm, lt_bounds(lt3, "chk.toolbar"), 0, "Has Toolbar");
	object_addhandler(wgt, "changed", has_toolbar_handle);
	wgt = checkbox_widget_create_with_label(frm, lt_bounds(lt3, "chk.statusbar"), 0, "Has Statusbar");
	object_addhandler(wgt, "changed", has_statusbar_handle);
	wgt = button_widget_create( frm, lt_bounds(lt3, "run"), 0 );
	button_set_text( wgt, "Parse" );
	object_addhandler(wgt, "pushed", parse_pushed);
	
	list_item_t *item, *sitem;
	image_t *img = 0;
	
	img = image_load( window, "icon.png" );
	
	wgt = menubar_widget_create( window, 0 );
	item = menubar_append_item( wgt, 0, 0, "File" );
		sitem = menubar_append_item( wgt, item, img, "Test 1" );
			menubar_append_item( wgt, sitem, 0, "Sub 1" );
			menubar_append_item( wgt, sitem, 0, "Sub 2" );
			menubar_append_item( wgt, sitem, img, "Sub 3" );
		sitem = menubar_append_item( wgt, item, 0, "Test 2" );
			menubar_append_item( wgt, sitem, 0, "Sub 1" );
			menubar_append_item( wgt, sitem, img, "Sub 2" );
			menubar_append_item( wgt, sitem, 0, "Sub 3" );
	
	menubar_append_separator( wgt, item );
		
	item = menubar_append_item( wgt, item, stock_get_image("system-log-out"), "&Quit" );
	menubar_add_key_binding( wgt, item, "Q", cModifierCommand );
	object_addhandler( item, "pushed", window_closed );

	menubar_append_item( wgt, 0, 0, "Help" );
	
	// and fill in our component toolbar
	wgt = toolbar_widget_create( window, cToolbarShowBoth );
	
	btn = toolbar_append_icon( OBJECT(wgt), img, "Parse", "Parse the lelex" );
	object_addhandler( btn, "pushed", parse_pushed );
	toolbar_append_separator( OBJECT(wgt) );
	btn = toolbar_append_icon( OBJECT(wgt), img, "Insert", "*shrug*" );
	//toolbar_append_popup_item( OBJECT(wgt), btn, img, "I like", "*shrug*" );
	//toolbar_append_popup_item( OBJECT(wgt), btn, img, "Candy", "*shrug*" );
	/*btn = toolbar_add_button(wgt, "Parse", "Parse the lelex", "examples/designer/icon.png");
	object_addhandler(btn, "pushed", parse_pushed);
	toolbar_add_button(wgt, "BT", "Button", "examples/designer/icon.png");
	toolbar_add_button(wgt, "CV", "Canvas", "examples/designer/icon.png");
	toolbar_add_button(wgt, "PB", "Progress", "examples/designer/icon.png");
	toolbar_add_button(wgt, "LB", "Label", "examples/designer/icon.png");
	toolbar_add_button(wgt, "CT", "Container", "examples/designer/icon.png");
	toolbar_add_button(wgt, "TX", "Textbox", "examples/designer/icon.png");*/
	
	status = statusbar_widget_create(window, 0);
	statusbar_set_text(status, "Started up");
	
	// setup some initial last bounds for the first window
	b = new_bounds( 0, 200, 800, 600 );
	store_put_bounds_t("window.bounds", *b);
	
	window_show( window );
	window_focus( window );
	
	claro_loop( );
	
	return 0;
}
