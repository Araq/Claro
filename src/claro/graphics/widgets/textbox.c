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

object_t *textbox_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	assert_valid_widget( parent, "parent" );
	
	return default_widget_create(parent, sizeof(textbox_widget_t), 
					"claro.graphics.widgets.textbox", bounds, 
					flags, cgraphics_textbox_widget_create);
}

void textbox_set_text( object_t *obj, char *text )
{
	textbox_widget_t *tb = (textbox_widget_t *)obj;
	
	assert_valid_textbox_widget( obj, "obj" );
	
	strscpy( tb->text, text, CLARO_TEXTBOX_MAXIMUM );
	
	cgraphics_textbox_update_text( tb );
}

char *textbox_get_text( object_t *obj )
{
	textbox_widget_t *tb = (textbox_widget_t *)obj;
	
	assert_valid_textbox_widget( obj, "obj" );
	
	return (char *)&tb->text;
}

int textbox_get_pos( object_t *obj )
{
	assert_valid_textbox_widget( obj, "obj" );
	return cgraphics_textbox_get_pos( WIDGET(obj) );
}

void textbox_set_pos( object_t *obj, int pos )
{
	assert_valid_textbox_widget( obj, "obj" );
	cgraphics_textbox_set_pos( WIDGET(obj), pos );
}
