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
#include <assert.h>
#include <claro/graphics/platform.h>

/* Frame */
void cgraphics_frame_widget_create(widget_t *widget)
{
	widget->native = gtk_frame_new("");
	widget->container = gtk_layout_new(0,0);
	
	gtk_container_add( widget->native, widget->container );
	
	gtk_widget_show( widget->container );
	
	cgraphics_widget_create(widget);
}

void cgraphics_frame_set_text( widget_t *w )
{
	frame_widget_t *widget = (frame_widget_t *)w;
	gtk_frame_set_label(GTK_FRAME(widget->widget.native), widget->text);
}
