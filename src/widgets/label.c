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

object_t *label_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *label;
	
	assert_valid_widget( parent, "parent" );
	
	label = default_widget_create(parent, sizeof(label_widget_t), 
										   "claro.graphics.widgets.label", bounds, 
										   flags, cgraphics_label_widget_create);
	
	label_set_justify( label, flags );
	label_set_text( label, "" );
	
	return label;
}

object_t *label_widget_create_with_text( object_t *parent, bounds_t *bounds, int flags, const char *text )
{
	object_t *label;
	
	assert_valid_widget( parent, "parent" );
	
	label = label_widget_create( parent, bounds, flags );
	
	label_set_text( label, text );
	
	return label;
}

void label_set_justify( object_t *obj, int flags )
{
	WIDGET(obj)->flags = flags;
	
	assert_valid_label_widget( obj, "obj" );
	
	cgraphics_label_update_justify( (widget_t *)obj );
}

void label_set_text( object_t *obj, const char *text )
{
	label_widget_t *w = (label_widget_t *)obj;
	
	assert_valid_label_widget( obj, "obj" );
	
	strscpy( w->text, text, CLARO_LABEL_MAXIMUM );
	
	cgraphics_label_update_text( (widget_t *)w );
}
