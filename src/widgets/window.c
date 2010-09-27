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


#include "graphics.h"
#include "platform.h"

object_t *window_widget_create(object_t *parent, bounds_t *bounds, int flags)
{
	object_t *window;
	
	assert_only_widget( parent, "parent" );
	
	window = default_widget_create(parent, sizeof(window_widget_t), 
						"claro.graphics.widgets.window", bounds, 
						flags, cgraphics_window_widget_create);
	
	widget_set_position(window, bounds->x, bounds->y, 0 );
	widget_set_content_size(window, bounds->w, bounds->h, 0 );
	
	object_addhandler( window, "content_resized", widget_resized_handle );
	
	return window;
}

void window_show( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_show( WIDGET(w) );
}

void window_hide( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_hide( WIDGET(w) );
}

void window_focus( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_focus( WIDGET(w) );
}

void window_set_title( object_t *w, const char *title )
{
	window_widget_t *ww = (window_widget_t *)w;
	
	assert_valid_window_widget( w, "w" );
	
	strncpy( ww->title, title, 512 );
	
	cgraphics_window_update_title( WIDGET(w) );
}

void window_maximise( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_maximise( WIDGET(w) );
}

void window_minimise( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_minimise( WIDGET(w) );
}

void window_restore( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_restore( WIDGET(w) );
}

void window_set_icon( object_t *w, image_t *icon )
{
	window_widget_t *ww = (window_widget_t *)w;
	
	assert_valid_window_widget( w, "w" );
	assert_valid_image( icon, "icon" );
	
	ww->icon = icon;
	cgraphics_window_update_icon( WIDGET(w) );
}
