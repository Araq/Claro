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


#include <claro/graphics.h>
#include <claro/graphics/platform.h>

object_t *scrollbar_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *w;
	scrollbar_widget_t *sb;
	
	assert_valid_widget( parent, "parent" );
	w = default_widget_create(parent, sizeof(scrollbar_widget_t), 
										   "claro.graphics.widgets.scrollbar", bounds, 
										   flags, cgraphics_scrollbar_widget_create);
	
	sb = (scrollbar_widget_t *)w;
	sb->min = 0;
	sb->max = 0;
	sb->pagesize = 10;
	
	return w;
}

int scrollbar_get_sys_width( )
{
	return cgraphics_scrollbar_get_sys_width( );
}

void scrollbar_set_range( object_t *w, int min, int max )
{
	scrollbar_widget_t *sb = (scrollbar_widget_t *)w;
	
	assert_valid_scrollbar_widget( w, "w" );
	
	sb->min = min;
	sb->max = max;
	
	cgraphics_scrollbar_set_range( OBJECT(w) );
}

void scrollbar_set_pagesize( object_t *w, int pagesize )
{
	scrollbar_widget_t *sb = (scrollbar_widget_t *)w;
	
	assert_valid_scrollbar_widget( w, "w" );
	
	sb->pagesize = pagesize;
	
	cgraphics_scrollbar_set_range( OBJECT(w) );
}

void scrollbar_set_pos( object_t *w, int pos )
{
	assert_valid_scrollbar_widget( w, "w" );
	cgraphics_scrollbar_set_pos( WIDGET(w), pos );
}

int scrollbar_get_pos( object_t *w )
{
	assert_valid_scrollbar_widget( w, "w" );
	return cgraphics_scrollbar_get_pos( WIDGET(w) );
}
