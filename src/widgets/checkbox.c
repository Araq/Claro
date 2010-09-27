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

object_t *checkbox_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = default_widget_create(parent, sizeof(checkbox_widget_t), 
					 "claro.graphics.widgets.checkbox", bounds, 
					 flags, cgraphics_checkbox_widget_create);
	
	checkbox_set_text( obj, "" );
	
	return obj;
}

object_t *checkbox_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label )
{
	object_t *obj = checkbox_widget_create( parent, bounds, flags );
	
	checkbox_set_text( obj, label );
	
	return obj;
}

void checkbox_set_label( object_t *obj, const char *label )
{
	checkbox_widget_t *chk = (checkbox_widget_t *)obj;
	
	assert_valid_checkbox_widget( obj, "obj" );
	
	strscpy( chk->text, label, CHECKBOX_TEXT_MAX );
	cgraphics_checkbox_set_text(WIDGET(obj));
}

int checkbox_get_checked( object_t *obj )
{
	checkbox_widget_t *chk = (checkbox_widget_t *)obj;
	
	assert_valid_checkbox_widget( obj, "obj" );
	
	return chk->checked;
}

void checkbox_set_checked( object_t *obj, int checked )
{
	checkbox_widget_t *chk = (checkbox_widget_t *)obj;
	
	assert_valid_checkbox_widget( obj, "obj" );
	
	chk->checked = checked;
	cgraphics_checkbox_update_checked( WIDGET(obj) );
}
