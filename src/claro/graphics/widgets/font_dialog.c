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

object_t *font_dialog_widget_create( object_t *parent, int flags )
{
	assert_only_widget( parent, "parent" );
	return default_widget_create(parent, sizeof(font_dialog_widget_t), 
				   "claro.graphics.widgets.dialogs.font", NO_BOUNDS, 
				   flags, cgraphics_font_dialog_widget_create);
	
}

void font_dialog_set_font( object_t *obj, const char *face, int size, int weight, int slant, int decoration )
{
	font_dialog_widget_t *fdw = (font_dialog_widget_t *)obj;
	
	assert_valid_font_dialog_widget( obj, "obj" );
	
	if ( fdw->selected.face != NULL )
		free( fdw->selected.face );
	
	fdw->selected.face = strdup( face );
	fdw->selected.size = size;
	fdw->selected.weight = weight;
	fdw->selected.slant = slant;
	fdw->selected.decoration = decoration;
	
	cgraphics_font_dialog_update_from_selection( WIDGET(obj) );
}

font_t *font_dialog_get_font( object_t *obj )
{
	font_dialog_widget_t *fdw = (font_dialog_widget_t *)obj;
	
	assert_valid_font_dialog_widget( obj, "obj" );
	
	cgraphics_font_dialog_update_to_selection( WIDGET(obj) );
	
	return &fdw->selected;
}
