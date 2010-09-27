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

/* Radiobutton */
gint cgraphics_radio_clicked_handler( GtkWidget *widget, widget_t *w )
{
	radiobutton_widget_t *rb = (radiobutton_widget_t *)w;
	radiogroup_t *grp = rb->group;
	
	grp->selected = OBJECT(rb);
	
	event_send( OBJECT(rb), "selected", "" );
	event_send( OBJECT(grp), "changed", "p", rb );
	
	return 1;
}

void cgraphics_radiogroup_create( radiogroup_t *group )
{
	/* Empty GSList */
	group->ndata = NULL;
}

void cgraphics_radiobutton_widget_create( widget_t *widget )
{
	widget->native = gtk_radio_button_new(NULL);
	assert(widget->native != NULL && "failed to create GTK radiobutton");
	
	cgraphics_widget_create( widget );
	
	g_signal_connect( G_OBJECT(widget->native), "clicked", G_CALLBACK(cgraphics_radio_clicked_handler), widget );
}

void cgraphics_radiobutton_set_group( widget_t *rr )
{
	radiobutton_widget_t *radio = (radiobutton_widget_t *)rr;
	
	/* first, set the group to the list we have */
	gtk_radio_button_set_group( radio->widget.native, radio->group->ndata );
	
	/* then get it back so we have the new head pointer (incase it changed, like on first item) */
	radio->group->ndata = gtk_radio_button_get_group( radio->widget.native );
}

void cgraphics_radiobutton_set_text( widget_t *widget )
{
	radiobutton_widget_t *radio = (radiobutton_widget_t *)widget;
	
	gtk_button_set_label( widget->native, radio->text );
}
