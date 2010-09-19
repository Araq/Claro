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

object_t *progress_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	assert_valid_widget( parent, "parent" );
	return default_widget_create(parent, sizeof(progress_widget_t), 
					"claro.graphics.widgets.progress", bounds, 
					flags, cgraphics_progress_widget_create);
}

void progress_set_level(object_t *progress, double percentage)
{
	assert_valid_progress_widget( progress, "progress" );
	
	cgraphics_progress_set_level(WIDGET(progress), percentage);
}

void progress_set_orientation(object_t *progress, int flags)
{
	assert_valid_progress_widget( progress, "progress" );
	
	cgraphics_progress_set_orientation(WIDGET(progress), flags);
}

