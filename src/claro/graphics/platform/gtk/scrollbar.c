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

void cgraphics_scrollbar_value_changed( GtkAdjustment *adjustment, widget_t *widget )
{
	event_send( widget, "scroll", "" );
}

void cgraphics_scrollbar_widget_create( widget_t *widget )
{
	scrollbar_widget_t *sb = (scrollbar_widget_t *)widget;
	widget->ndata = gtk_adjustment_new( 1, 1, 1, 1, 1, 1 );
	
	if ( widget->flags & cScrollbarVertical )
		widget->native = gtk_vscrollbar_new( widget->ndata );
	else
		widget->native = gtk_hscrollbar_new( widget->ndata );
	
	cgraphics_widget_create( widget );
	
	g_signal_connect( G_OBJECT(widget->ndata), "value-changed", G_CALLBACK(cgraphics_scrollbar_value_changed), widget );
	
	sb->min = sb->max = 0;
	cgraphics_scrollbar_set_range( widget );
}

int cgraphics_scrollbar_get_sys_width( )
{
	return 15; // FIXME!
}

void cgraphics_scrollbar_set_range( widget_t *w )
{
	scrollbar_widget_t *sb = (scrollbar_widget_t *)w;
	GtkAdjustment *a = (GtkAdjustment *)w->ndata;
	
	a->lower = sb->min;
	a->upper = sb->max + 1;
	
	gtk_widget_set_sensitive( GTK_WIDGET(w->native), !(sb->min == sb->max) );
	
	g_signal_emit_by_name( G_OBJECT(a), "changed" );
}

void cgraphics_scrollbar_set_pos( widget_t *w, int pos )
{
	scrollbar_widget_t *sb = (scrollbar_widget_t *)w;
	GtkAdjustment *a = (GtkAdjustment *)w->ndata;
	
	a->value = pos;
	
	g_signal_emit_by_name( G_OBJECT(a), "changed" );
	
	event_send( w, "scroll", "" );
}

int cgraphics_scrollbar_get_pos( widget_t *w )
{
	GtkAdjustment *a = (GtkAdjustment *)w->ndata;
	
	return a->value;
}
