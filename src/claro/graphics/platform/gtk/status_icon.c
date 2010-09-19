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

/* Status Icon */

#if GTK_CHECK_VERSION(2,10,0)

static void _on_status_icon_activate(GtkStatusIcon * icon, gpointer arg)
{
    event_send( OBJECT(arg), "pushed", "" );
}

static void _on_status_icon_popup_menu(GtkStatusIcon *icon, guint button, guint32 activate_time, gpointer arg)
{
    status_icon_t * status = (status_icon_t*)arg;
    gtk_menu_popup (GTK_MENU (WIDGET(status->native2)->native), NULL, NULL, NULL, NULL, button, activate_time);
}

void cgraphics_status_icon_create(status_icon_t * status, int flags)
{
    GtkStatusIcon * icon = gtk_status_icon_new(); 
    
    g_signal_connect(G_OBJECT(icon), "activate", G_CALLBACK(_on_status_icon_activate), (gpointer)status); 
    g_signal_connect(G_OBJECT(icon), "popup-menu", G_CALLBACK (_on_status_icon_popup_menu), (gpointer)status);
 
    status->native = (void*)icon;        
}

void cgraphics_status_icon_set_icon(status_icon_t * status, image_t * icon)
{
    gtk_status_icon_set_from_pixbuf(GTK_STATUS_ICON(status->native), (GdkPixbuf*)icon->native);
}

void cgraphics_status_icon_set_menu(status_icon_t * status, object_t * menu)
{
    status->native2 = (void*)menu;
}

void cgraphics_status_icon_set_visible(status_icon_t * status, int visible)
{
    gtk_status_icon_set_visible(GTK_STATUS_ICON(status), visible? TRUE : FALSE);
}

void cgraphics_status_icon_set_tooltip(status_icon_t * status, const char * tooltip)
{
    gtk_status_icon_set_tooltip(GTK_STATUS_ICON(status), tooltip);
}

#else
#error "Claro requires Gtk+ 2.10 !"
#endif


