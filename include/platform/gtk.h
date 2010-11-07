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


#ifndef _CLARO_GRAPHICS_PLATFORMS_GTK_H
#define _CLARO_GRAPHICS_PLATFORMS_GTK_H

#include <gtk/gtk.h>
#include "gtkcairo.h"

typedef struct
{
	GtkWindow *window;
	GtkWidget *vbox, *vbox2, *vboxtop;
} gtk_window_native;

gint cgraphics_resized2_handler( GtkWidget *widget, GtkAllocation *event, widget_t *w );
gint cgraphics_destroy_handler( widget_t *w );

widget_t *cgraphics_gtk_to_claro_widget( GtkWidget *w );

#endif
