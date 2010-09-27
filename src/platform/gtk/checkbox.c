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

/* Checkbox */
gint cgraphics_checkbox_clicked_handler( GtkWidget *widget, widget_t *w )
{
	checkbox_widget_t *cb = (checkbox_widget_t *)w;
	
	cb->checked = gtk_toggle_button_get_active( (GtkToggleButton *)widget );
	
	event_send( OBJECT(w), "changed", "i", cb->checked );
	
	return 1;
}

void cgraphics_checkbox_widget_create(widget_t *widget)
{
	widget->native = gtk_check_button_new_with_label("tmp");
	assert(widget->native != NULL && "failed to create GTK checkbutton");
	
	cgraphics_widget_create( widget );
	
	g_signal_connect( G_OBJECT(widget->native), "clicked", G_CALLBACK(cgraphics_checkbox_clicked_handler), widget );
}

void cgraphics_checkbox_set_text(widget_t *widget)
{
	checkbox_widget_t *chk = (checkbox_widget_t *)widget;
	
	gtk_button_set_label(widget->native, chk->text);
}

void cgraphics_checkbox_update_checked( widget_t *widget )
{
	gtk_toggle_button_set_active( widget->native, ((checkbox_widget_t *)widget)->checked );
}
