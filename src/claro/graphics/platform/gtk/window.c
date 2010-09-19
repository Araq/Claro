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

void cgraphics_screen_changed_handler( GtkWidget *widget, GdkScreen *arg1, widget_t *w );
gboolean cgraphics_cd_expose_handler( GtkWidget *widget, GdkEventExpose *event, widget_t *w );

/* called when a window widget needs creating */
void cgraphics_window_widget_create( widget_t *widget )
{
	gtk_window_native *nd = (gtk_window_native *)malloc( sizeof(gtk_window_native) );
	
	widget->ndata = nd;

	widget->native = nd->window = (GtkWindow *)gtk_window_new( GTK_WINDOW_TOPLEVEL );

	//added 2/13/06, by Cody

	GtkAccelGroup * group = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW(widget->native),group);
	widget->naddress[0] = (void*)group;	

	nd->vbox = gtk_vbox_new( FALSE, 0 );
	nd->vbox2 = gtk_vbox_new( FALSE, 0 );
	nd->vboxtop = gtk_vbox_new( FALSE, 0 );
	
	widget->container = gtk_layout_new( NULL, NULL );
	
	if ( widget->flags & cWidgetCustomDraw )
	{
		g_signal_connect( G_OBJECT(widget->native), "screen-changed", G_CALLBACK(cgraphics_screen_changed_handler), widget );
		g_signal_connect( G_OBJECT(widget->container), "expose-event", G_CALLBACK(cgraphics_cd_expose_handler), widget );
		gtk_window_set_decorated( widget->native, FALSE );
		gtk_widget_set_app_paintable( widget->native, TRUE );
		gtk_widget_set_app_paintable( widget->container, TRUE );
	}
	
	gtk_window_resize( GTK_WINDOW(nd->window), widget->size_req->w, widget->size_req->h );
	gtk_window_move( GTK_WINDOW(nd->window), widget->size_req->x, widget->size_req->y );
	
	gtk_container_add( GTK_CONTAINER(nd->window), nd->vbox );
	gtk_box_pack_start( GTK_BOX(nd->vbox), GTK_WIDGET(nd->vboxtop), FALSE, FALSE, 0 );
	gtk_box_pack_end( GTK_BOX(nd->vbox), GTK_WIDGET(nd->vbox2), TRUE, TRUE, 0 );
	gtk_box_pack_start( GTK_BOX(nd->vbox2), GTK_WIDGET(widget->container), TRUE, TRUE, 0 );
	
	g_signal_connect( G_OBJECT(widget->container), "size_allocate", G_CALLBACK(cgraphics_resized2_handler), widget );
	g_signal_connect_swapped( G_OBJECT(nd->window), "destroy", G_CALLBACK(cgraphics_destroy_handler), widget );
	
	gtk_widget_show( nd->vbox );
	gtk_widget_show( nd->vbox2 );
	gtk_widget_show( nd->vboxtop );
	gtk_widget_show( widget->container );
	
	gtk_window_set_resizable( widget->native, !(widget->flags & cWindowNoResizing) );
	
	if ( widget->flags & cWindowModalDialog )
	{
		widget_t *wp = OBJECT(widget)->parent;
		
		if ( wp )
			gtk_window_set_transient_for( GTK_WIDGET(widget->native), GTK_WIDGET(wp->native) );
		gtk_window_set_modal( widget->native, TRUE );
	}
	
	if ( widget->flags & cWindowCenterParent )
		gtk_window_set_position( widget->native, GTK_WIN_POS_CENTER_ON_PARENT );
	
	if ( widget->flags & cWidgetCustomDraw )
		cgraphics_screen_changed_handler( widget->native, NULL, widget );
}

void cgraphics_window_show( widget_t *w )
{
	//gtk_window_native *nd = (gtk_window_native *)w->ndata;
	//gtk_widget_show( GTK_WIDGET(nd->window) );
	if ( w->flags & cWidgetCustomDraw )
	{
		gtk_widget_realize( GTK_WIDGET(w->native) );
		gdk_window_set_back_pixmap( GTK_WIDGET(w->native)->window, NULL, FALSE);
	}
	
	gtk_widget_show( GTK_WIDGET(w->native) );
}

void cgraphics_window_hide( widget_t *w )
{
	/*gtk_window_native *nd = (gtk_window_native *)w->ndata;
	gtk_widget_hide( GTK_WIDGET(nd->window) );*/
	gtk_widget_hide( GTK_WIDGET(w->native) );
}

void cgraphics_window_focus( widget_t *w )
{
	/*gtk_window_native *nd = (gtk_window_native *)w->ndata;
	gtk_widget_grab_focus( GTK_WIDGET(nd->window) );*/
	gtk_widget_grab_focus( GTK_WIDGET(w->native) );
}

void cgraphics_window_update_title( widget_t *w )
{
	window_widget_t *ww = (window_widget_t *)w;
	/*gtk_window_native *nd = (gtk_window_native *)w->ndata;
	
	gtk_window_set_title( GTK_WINDOW(nd->window), ww->title );*/
	gtk_window_set_title( GTK_WIDGET(w->native), ww->title );
}

void cgraphics_window_update_icon( widget_t *w )
{
	window_widget_t *ww = (window_widget_t *)w;
	gtk_window_set_icon( w->native, ww->icon->native );
}

void cgraphics_window_restore( widget_t *w )
{
	gtk_window_unmaximize( w->native );
	gtk_window_deiconify( w->native );
}

void cgraphics_window_maximise( widget_t *w )
{
	gtk_widget_show( GTK_WIDGET(w->native) );
	gtk_window_maximize( w->native );
}

void cgraphics_window_minimise( widget_t *w )
{
	gtk_window_iconify( w->native );
}
