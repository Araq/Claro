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

void cgraphics_menu_item_pushed_handler( GtkMenuItem *gitem, list_item_t *i )
{
	event_send( OBJECT(i), "pushed", "" );
}

void cgraphics_menu_widget_create( widget_t *widget )
{
	widget->native = gtk_menu_new( );
	gtk_widget_show( GTK_WIDGET(widget->native) );
}

GtkMenuShell *cgraphics_menubar_get_native_parent( widget_t *widget, list_item_t *item );

void cgraphics_menu_new_item( widget_t *widget, list_item_t *item )
{
	GtkMenuShell *ps = cgraphics_menubar_get_native_parent( widget, item );
	GtkMenuItem *mi;
	
	if ( item->data[1] == 0 )
		mi = GTK_MENU_ITEM(gtk_separator_menu_item_new());
	else
	{
		char *txt = item->data[1];
		int a;
		
		// fix up mnemonics.. GTK expects _, not &.
		for ( a = 0; a < strlen( txt ); a++ )
			if ( txt[a] == '&' )
				txt[a] = '_';
		
		mi = GTK_MENU_ITEM(gtk_image_menu_item_new_with_mnemonic( item->data[1]));
		g_signal_connect( G_OBJECT(mi), "activate", G_CALLBACK(cgraphics_menu_item_pushed_handler), item );
	
		if ( item->data[0] != 0 )
		{
			image_t *im = (image_t *)item->data[0];
			GtkImage *mim = GTK_IMAGE(gtk_image_new_from_pixbuf( NULL));
			gtk_image_set_from_pixbuf( mim, (GdkPixbuf *)im->native );
			gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(mi), GTK_WIDGET(mim) );
			gtk_widget_show( GTK_WIDGET(mim) );
		}
	}
	
	item->nativeid = (size_t)mi;
	
	gtk_menu_shell_insert( ps, GTK_WIDGET(mi), item->row );
	gtk_widget_show(GTK_WIDGET(mi));
}

void cgraphics_menu_remove_item( widget_t *widget, list_item_t *item )
{
}

void cgraphics_menu_disable_item( widget_t *menu, list_item_t *item )
{
	GtkMenuItem *mi;
	
	mi = (GtkMenuItem *)item->nativeid;
	
	gtk_widget_set_sensitive( GTK_WIDGET(mi), FALSE );
}

void cgraphics_menu_enable_item( widget_t *menu, list_item_t *item )
{
	GtkMenuItem *mi;
	
	mi = (GtkMenuItem *)item->nativeid;
	
	gtk_widget_set_sensitive( GTK_WIDGET(mi), FALSE );
}

void cgraphics_menu_position( GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer user_data)
{
  widget_t* menuw = (widget_t*) user_data;
	*x = menuw->size.x;
	*y = menuw->size.y;
	*push_in = TRUE;
	printf( "%d,%d\n", *x, *y );
}

void cgraphics_menu_popup( widget_t *menu, int x, int y, int flags )
{
	GdkScreen *screen;
	GtkRequisition requisition;
	GdkRectangle monitor;
	GtkWidget *widget = GTK_WIDGET(menu->native);
	
	if ( flags & cMenuPopupAtCursor )
	{
		gtk_menu_popup( menu->native, NULL, NULL, NULL, NULL, 0, gtk_get_current_event_time( ) );
		return;
	}
	
	gtk_widget_size_request( widget, &requisition );
	
	screen = gtk_widget_get_screen( widget );
	gdk_screen_get_monitor_geometry( screen, gdk_screen_get_monitor_at_point( screen, x, y ), &monitor );
	
	/* FIXME: This might not be correct */
	if ( y + requisition.height > monitor.height )
		y -= requisition.height;
	
	menu->size.x = x;
	menu->size.y = y;
	
	gtk_menu_popup( menu->native, NULL, NULL, cgraphics_menu_position, menu, 0, gtk_get_current_event_time( ) );
}
