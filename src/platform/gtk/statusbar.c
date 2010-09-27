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
#include <assert.h>
#include "platform.h"

/* Statusbar */
void cgraphics_statusbar_widget_create(widget_t *widget)
{
	widget_t *parent;
	gtk_window_native *nd;
	
	widget->native = gtk_statusbar_new();
	widget->ndata = (void *)0; // used to keep track of the top message id

	gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(widget->native), 1);
	
	parent = (widget_t *)widget->object.parent;
	
	if ( parent == 0 )
		return;
	
	if ( strcasecmp( parent->object.type, "claro.graphics.widgets.window" ) )
		return;
	
	nd = parent->ndata;
	
	gtk_box_pack_end( GTK_BOX(nd->vbox2), GTK_WIDGET(widget->native), FALSE, FALSE, 0 );
}

void cgraphics_statusbar_widget_update_text(statusbar_widget_t *sb)
{
	size_t msg_id = (size_t)sb->widget.ndata;
	
	if(msg_id) {
		gtk_statusbar_remove(GTK_STATUSBAR(sb->widget.native), 0, msg_id);
	}
	
	// uses ndata to keep the last msg_id around for later removal
	sb->widget.ndata = (void *)(size_t)gtk_statusbar_push(
	  GTK_STATUSBAR(sb->widget.native), 0, sb->text);
}
