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

object_t *frame_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = default_widget_create(parent, sizeof(frame_widget_t),
					"claro.graphics.widgets.frame", bounds, 
					flags, cgraphics_frame_widget_create);
	
	frame_set_text( obj, "" );
	
	return obj;
}

object_t *frame_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label )
{
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = frame_widget_create( parent, bounds, flags );
	
	frame_set_text( obj, label );
	
	return obj;
}

void frame_set_text( object_t *frame, const char *label )
{
	frame_widget_t *fw = (frame_widget_t *)frame;
	
	assert_valid_frame_widget( frame, "frame" );
	
	strscpy( fw->text, label, CLARO_FRAME_MAXIMUM );
	
	cgraphics_frame_set_text((widget_t *)frame);
}
