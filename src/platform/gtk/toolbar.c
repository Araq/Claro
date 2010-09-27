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

/* Toolbar */

void cgraphics_toolbar_item_pushed_handler( GtkToolItem *gitem, list_item_t *i )
{
	event_send( OBJECT(i), "pushed", "" );
}

void cgraphics_toolbar_widget_create( widget_t *widget )
{
	widget_t *parent;
	gtk_window_native *nd;
	widget->native = gtk_toolbar_new( );
	widget->ndata = gtk_tooltips_new( );
	
	if ( widget->flags & cToolbarShowBoth )
		gtk_toolbar_set_style( GTK_TOOLBAR(widget->native), GTK_TOOLBAR_BOTH );
	else if ( widget->flags & cToolbarShowText )
		gtk_toolbar_set_style( GTK_TOOLBAR(widget->native), GTK_TOOLBAR_TEXT );
	else
		gtk_toolbar_set_style( GTK_TOOLBAR(widget->native), GTK_TOOLBAR_ICONS );
	
	parent = (widget_t *)widget->object.parent;
	
	if ( parent == 0 )
		return;
	
	if ( strcasecmp( parent->object.type, "claro.graphics.widgets.window" ) )
		return;
	
	nd = parent->ndata;
	
	gtk_box_pack_end( GTK_BOX(nd->vboxtop), GTK_WIDGET(widget->native), FALSE, FALSE, 0 );
}

void cgraphics_toolbar_new_icon( widget_t *widget, list_item_t *item )
{
	image_t *img = item->data[0];
	char *title = item->data[1];
	char *tooltip = item->data[2];
	GtkToolItem *ti;
	
	if ( img != 0 || title != 0 )
	{
		GtkWidget *icon = NULL;
		
		if ( img != 0 )
		{
			icon = gtk_image_new_from_pixbuf( img->native );
			gtk_widget_show( icon );
		}
		
		ti = gtk_tool_button_new( icon, title );
		
		if ( tooltip != 0 )
		{
			int a = 0;
			char tmpt[strlen(tooltip)+1];
			
			// kill mnemonics..
			while ( *tooltip != 0 )
			{
				if ( *tooltip != '&' )
				{
					tmpt[a] = *tooltip;
					a++;
				}
				
				tooltip++;
			}
			tmpt[a] = 0;

			gtk_tool_item_set_tooltip( ti, widget->ndata, tmpt, "" );
		}
	}
	else
	{
		ti = gtk_separator_tool_item_new();
	}
	
	item->native = ti;
	
	gtk_widget_show( GTK_WIDGET(ti) );
	
	gtk_toolbar_insert( widget->native, ti, item->row );
	
	if ( img != 0 || title != 0 )
		g_signal_connect( G_OBJECT(ti), "clicked", G_CALLBACK(cgraphics_toolbar_item_pushed_handler), item );
}

void cgraphics_toolbar_remove_icon( widget_t *widget, list_item_t *item )
{
	
}

void cgraphics_toolbar_set_menu( widget_t *widget, list_item_t *item )
{
	image_t *img = item->data[0];
	char *title = item->data[1];
	char *tooltip = item->data[2];
	GtkToolItem *oti = item->native;
	GtkToolItem *ti;
	
	if ( img == 0 && title == 0 )
		return;
	
	if ( item->menu != NULL )
	{
		GtkWidget *icon = NULL;
		
		if ( img != 0 )
		{
			icon = gtk_image_new_from_pixbuf( img->native );
			gtk_widget_show( icon );
		}
		
		gtk_widget_destroy( GTK_WIDGET(oti) );
		
		ti = gtk_menu_tool_button_new( icon, title );
		gtk_menu_tool_button_set_menu( GTK_MENU_TOOL_BUTTON(ti), WIDGET(item->menu)->native );
		
		item->native = ti;
		
		if ( tooltip != 0 )
		{
			int a = 0;
			char tmpt[strlen(tooltip)+1];
			
			// kill mnemonics..
			while ( *tooltip != 0 )
			{
				if ( *tooltip != '&' )
				{
					tmpt[a] = *tooltip;
					a++;
				}
				
				tooltip++;
			}
			tmpt[a] = 0;
			
			gtk_tool_item_set_tooltip( ti, widget->ndata, tmpt, "" );
		}
	}
	else
		return;
	
	gtk_widget_show( GTK_WIDGET(ti) );
	
	gtk_toolbar_insert( widget->native, ti, item->row );
	
	if ( img != 0 || title != 0 )
		g_signal_connect( G_OBJECT(ti), "clicked", G_CALLBACK(cgraphics_toolbar_item_pushed_handler), item );
}


/*
void cgraphics_toolbar_new_popup_item( widget_t *widget, list_item_t *item )
{
	
}

void cgraphics_toolbar_remove_popup_item( widget_t *widget, list_item_t *item )
{
	
}*/
