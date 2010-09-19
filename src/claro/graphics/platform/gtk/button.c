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

/* Button */

gint cgraphics_button_clicked_handler( GtkWidget *widget, widget_t *w )
{
	event_send( OBJECT(w), "pushed", "" );
	
	return 1;
}

void cgraphics_button_widget_create( widget_t *widget )
{
	widget->native = gtk_button_new_with_label( "" );
	
	cgraphics_widget_create( widget );
	
	g_signal_connect( G_OBJECT(widget->native), "clicked", G_CALLBACK(cgraphics_button_clicked_handler), widget );
}

void cgraphics_button_update_text( button_widget_t *widget )
{
	gtk_button_set_label( GTK_BUTTON(widget->widget.native), widget->text );
}


void cgraphics_button_update_image( button_widget_t *widget, const char *filename)
{
	GtkWidget* image = gtk_image_new_from_file(filename);
	assert(image != NULL && "failed to load image");
	gtk_button_set_image(GTK_BUTTON(widget->widget.native), image);
}
