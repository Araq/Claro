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

object_t *button_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	assert_valid_widget( parent, "parent" );
	return default_widget_create(parent, sizeof(button_widget_t), 
										   "claro.graphics.widgets.button", bounds, 
										   flags, cgraphics_button_widget_create);
	
}

object_t *button_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label )
{
	object_t *o;
	
	assert_valid_widget( parent, "parent" );
	
	o = button_widget_create( parent, bounds, flags );
	button_set_text( o, label );
	
	return o;
}

void button_set_label( object_t *obj, const char *label )
{
	button_widget_t *bw = (button_widget_t *)obj;
	
	assert_valid_button_widget( obj, "obj" );
	
	strscpy( bw->text, label, CLARO_BUTTON_MAXIMUM );
	
	cgraphics_button_update_text( bw );
}

void button_set_image( object_t *obj, image_t *image )
{
	assert_valid_button_widget( obj, "obj" );
	assert_valid_image( image, "image" );
	
	/*cgraphics_button_update_image((button_widget_t *)obj, filename);*/
}
