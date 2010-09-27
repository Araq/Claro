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

object_t *statusbar_widget_create( object_t *parent, int flags )
{
	assert_valid_window_widget( parent, "parent" );
	
	return default_widget_create(parent, sizeof(statusbar_widget_t), 
					"claro.graphics.widgets.statusbar", NULL, 
					flags, cgraphics_statusbar_widget_create);
}

void statusbar_set_text( object_t *obj, const char *text )
{
	statusbar_widget_t *sb = (statusbar_widget_t *)obj;
	
	assert_valid_statusbar_widget( obj, "obj" );
	
	strscpy( sb->text, text, CLARO_BUTTON_MAXIMUM );
	cgraphics_statusbar_widget_update_text(sb);
}
