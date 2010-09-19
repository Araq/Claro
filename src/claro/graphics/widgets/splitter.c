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

object_t *splitter_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	assert_valid_widget( parent, "parent" );
	object_t *o = default_widget_create(parent, sizeof(splitter_widget_t), 
						"claro.graphics.widgets.splitter", bounds, 
						flags, cgraphics_splitter_widget_create);
	splitter_widget_t *sw = (splitter_widget_t *)o;
	
	sw->children[0].flex = 1;
	sw->children[0].size = 0;
	
	sw->children[1].flex = 1;
	sw->children[1].size = 0;
	
	return o;
}

void splitter_set_info( object_t *splitter, int child, int flex, int size )
{
	splitter_widget_t *sw = (splitter_widget_t *)splitter;
	
	assert_valid_splitter_widget( splitter, "splitter" );
	
	sw->children[child].flex = flex;
	sw->children[child].size = size;
	
	cgraphics_splitter_updated( splitter, child );
}
