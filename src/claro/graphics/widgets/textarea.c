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

object_t *textarea_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	assert_valid_widget( parent, "parent" );
	
	return default_widget_create(parent, sizeof(textarea_widget_t),
					"claro.graphics.widgets.textarea", bounds, 
					flags, cgraphics_textarea_widget_create);
}

void textarea_set_text( object_t *obj, const char *text )
{
	textarea_widget_t *tw = (textarea_widget_t *)obj;
	
	assert_valid_textarea_widget( obj, "obj" );
	
	strncpy( tw->text, text, CLARO_TEXTAREA_MAXIMUM-1 );
	tw->text[CLARO_TEXTAREA_MAXIMUM] = 0;
	
	cgraphics_textarea_set_text((widget_t *)obj);
}


char *textarea_get_text( object_t *obj)
{
	textarea_widget_t *tw = (textarea_widget_t *)obj;
	assert_valid_textarea_widget( obj, "obj" );
	return &tw->text;
}
