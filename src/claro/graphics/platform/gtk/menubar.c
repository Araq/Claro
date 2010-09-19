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

/* Menubar */

void cgraphics_menubar_item_pushed_handler( GtkMenuItem *gitem, list_item_t *i )
{
	event_send( OBJECT(i), "pushed", "" );
}

void cgraphics_menubar_widget_create( widget_t *widget )
{
	widget_t *parent;
	gtk_window_native *nd;
	
	widget->native = gtk_menu_bar_new( );
	gtk_widget_show( GTK_WIDGET(widget->native) );
	
	parent = (widget_t *)widget->object.parent;
	
	if ( parent == 0 )
		return;
	
	if ( strcasecmp( parent->object.type, "claro.graphics.widgets.window" ) )
		return;
	
	nd = parent->ndata;	
	
	gtk_box_pack_start( GTK_BOX(nd->vboxtop), GTK_WIDGET(widget->native), FALSE, FALSE, 0 );
}

GtkMenuShell *cgraphics_menubar_get_native_parent( widget_t *widget, list_item_t *item )
{
	GtkMenuShell *ps;
	list_item_t *pitem = 0;
	GtkMenuItem *mi;
	
	if ( strcmp( item->object.parent->type, "claro.graphics.list_item" ) ) // parent isn't an item
		return widget->native;
	else
	{
		pitem = item->object.parent;
		
		if ( pitem->native == 0 )
		{
			pitem->native = gtk_menu_new( );
			gtk_menu_item_set_submenu( (GtkMenuItem *)pitem->nativeid, pitem->native );
		}
		
		return pitem->native;
	}
}

void cgraphics_menubar_add_key_binding(widget_t * widget, list_item_t *item, const char * utf8_key,int modifier)
{
	GtkWindow *window = (GtkWindow *)WIDGET(OBJECT(widget)->parent)->native; 
	GtkAccelGroup * group = (GtkAccelGroup *)WIDGET(OBJECT(widget)->parent)->naddress[0];
	GtkMenuItem *mi = (GtkMenuItem *)item->nativeid;

	int key_code = gdk_unicode_to_keyval( g_utf8_get_char(utf8_key) );

	int mask = 0;
	if(modifier & cModifierCommand)
		mask |= GDK_CONTROL_MASK;
	if(modifier & cModifierShift)
		mask |= GDK_SHIFT_MASK;

	gtk_widget_add_accelerator(GTK_WIDGET(mi), "activate", group, key_code, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
}

void cgraphics_menubar_new_item( widget_t *widget, list_item_t *item )
{
	GtkMenuShell *ps = cgraphics_menubar_get_native_parent( widget, item );
	GtkMenuItem *mi;
	
	if ( item->data[1] == 0 )
		mi = gtk_separator_menu_item_new( );
	else
	{
		char *txt = item->data[1];
		int a;
		
		// fix up mnemonics.. GTK expects _, not &.
		for ( a = 0; a < strlen( txt ); a++ )
			if ( txt[a] == '&' )
				txt[a] = '_';
		
		mi = gtk_image_menu_item_new_with_mnemonic( item->data[1] );
		g_signal_connect( G_OBJECT(mi), "activate", G_CALLBACK(cgraphics_menubar_item_pushed_handler), item );
	
		if ( item->data[0] != 0 )
		{
			image_t *im = (image_t *)item->data[0];
			GtkImage *mim = gtk_image_new_from_pixbuf( NULL );
			gtk_image_set_from_pixbuf( mim, (GdkPixbuf *)im->native );
			gtk_image_menu_item_set_image( mi, mim );
			gtk_widget_show( mim );
		}
	}
	
	item->nativeid = (int)mi;
	
	gtk_menu_shell_insert( ps, mi, item->row );
	gtk_widget_show( mi );
}

void cgraphics_menubar_remove_item( widget_t *widget, list_item_t *item )
{
}

void cgraphics_menubar_disable_item( widget_t *menubar, list_item_t *item )
{
	GtkMenuItem *mi;
	
	mi = (GtkMenuItem *)item->nativeid;
	
	gtk_widget_set_sensitive( GTK_WIDGET(mi), FALSE );
}

void cgraphics_menubar_enable_item( widget_t *menubar, list_item_t *item )
{
	GtkMenuItem *mi;
	
	mi = (GtkMenuItem *)item->nativeid;
	
	gtk_widget_set_sensitive( GTK_WIDGET(mi), FALSE );
}
