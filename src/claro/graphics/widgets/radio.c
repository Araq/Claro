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

object_t *radiogroup_create( object_t *parent, int flags )
{
	radiogroup_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = (radiogroup_t *)object_create( parent, sizeof(radiogroup_t), "claro.graphics.widgets.radiogroup" );
	
	obj->ndata = 0;
	
	list_create( &obj->buttons );
	
	cgraphics_radiogroup_create( obj );
	
	return OBJECT(obj);
}

object_t *radiobutton_widget_create( object_t *parent, object_t *group, bounds_t *bounds, const char *label, int flags )
{
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	assert_valid_radiogroup_widget( group, "group" );
	
	obj = default_widget_create(parent, sizeof(radiobutton_widget_t),
					"claro.graphics.widgets.radiobutton", bounds, 
					flags, cgraphics_radiobutton_widget_create);
	
	radiobutton_set_label( obj, label );
	radiobutton_set_group( obj, group );
	
	return obj;
}

void radiobutton_set_label( object_t *obj, const char *label )
{
	radiobutton_widget_t *chk = (radiobutton_widget_t *)obj;
	
	assert_valid_radiobutton_widget( obj, "obj" );
	
	strscpy( chk->text, label, RADIOBUTTON_TEXT_MAX );
	cgraphics_radiobutton_set_text( WIDGET(obj) );
}

void radiobutton_set_group( object_t *radio, object_t *group )
{
	radiobutton_widget_t *r = (radiobutton_widget_t *)radio;
	radiogroup_t *g = (radiogroup_t *)group;
	node_t *n;
	
	assert_valid_radiobutton_widget( radio, "radio" );
	assert_valid_radiogroup_widget( group, "group" );
	
	if ( r->group != 0 )
	{
		/* FIXME: remove from old group ! */
	}
	
	r->group = g;
	
	n = node_create( );
	
	node_add( radio, n, &g->buttons );
	
	cgraphics_radiobutton_set_group( WIDGET(radio) );
}
