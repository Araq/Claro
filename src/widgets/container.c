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

object_t *container_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	widget_t *widget;
	container_widget_t *cw;
	
	assert_valid_widget( parent, "parent" );
	
	widget = (widget_t *)object_create( parent, sizeof(container_widget_t), "claro.graphics.widgets.container" );
	
	cw = (container_widget_t *)widget;
	widget->size_req = bounds;
	
	widget->flags = flags;
	
	widget_pre_init( OBJECT(widget) );
	
	cgraphics_container_widget_create( widget );
	
	widget_post_init( OBJECT(widget) );
	
	return (object_t *)widget;
}
