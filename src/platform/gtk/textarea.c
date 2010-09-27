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

/* Textarea */

gint cgraphics_textarea_changed_handler( GtkWidget *widget, widget_t *w )
{
	event_send( OBJECT(w), "changed", "" );		
	return 1;
}

gint cgraphics_textarea_modified_handler( GtkWidget *widget, widget_t *w )
{
	event_send( OBJECT(w), "modified", "" );
	return 1;
}


void cgraphics_textarea_widget_create(widget_t *widget)
{
	widget->native = gtk_text_view_new ();
	
	widget->ndata  = gtk_text_view_get_buffer (GTK_TEXT_VIEW (widget->native));
	
	cgraphics_widget_create(widget);
	
	g_signal_connect( G_OBJECT(widget->native), "changed", G_CALLBACK(cgraphics_textarea_changed_handler), widget );
	g_signal_connect( G_OBJECT(widget->native), "modified-changed", G_CALLBACK(cgraphics_textarea_modified_handler), widget );
}


void cgraphics_textarea_set_text( widget_t *obj )
{
	textarea_widget_t *tw = (textarea_widget_t *)obj;
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(obj->ndata), tw->text, -1);
}


char *cgraphics_textarea_get_text( widget_t *obj)
{
	GtkTextIter start;
	GtkTextIter end;
	
	gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(obj->ndata), &start);
	gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(obj->ndata), &end);
	
	return gtk_text_buffer_get_text(GTK_TEXT_BUFFER(obj->ndata), &start, &end, FALSE);
}
