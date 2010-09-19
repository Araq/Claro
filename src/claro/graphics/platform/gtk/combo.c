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

void cgraphics_combo_changed_handler( GtkComboBox *widget, widget_t *cw )
{
	combo_widget_t *cbw = (combo_widget_t *)cw;
	int active = gtk_combo_box_get_active( widget );	
	list_item_t *item = 0, *curr;
	node_t *n;
		
	if ( active != -1 )
	{
		LIST_FOREACH( n, cbw->widget.items.head )
		{
			curr = (list_item_t *)n->data;
			
			if ( curr->row == active )
				item = curr;
		}
	}
	
	cbw->selected = item;
	event_send( OBJECT(cw), "selected", "p", item );
}

void cgraphics_combo_widget_create( widget_t *widget )
{
	widget->native = gtk_combo_box_new_text( );
	
	cgraphics_widget_create( widget );
	
	g_signal_connect( G_OBJECT(widget->native), "changed", G_CALLBACK(cgraphics_combo_changed_handler), widget );
}

void cgraphics_combo_new_row( widget_t *widget, list_item_t *item )
{
	gtk_combo_box_insert_text( widget->native, item->row, item->data[0] );
}

void cgraphics_combo_remove_row( widget_t *widget, list_item_t *item )
{
	gtk_combo_box_remove_text( widget->native, item->row );
}

void cgraphics_combo_select_row( widget_t *widget, list_item_t *item )
{
	combo_widget_t *lw = (combo_widget_t *)widget;
	int i = -1;
	
	if ( item != NULL )
		i = item->row;
	
	gtk_combo_box_set_active( widget->native, i );
	
	lw->selected = item;
	event_send( OBJECT(widget), "selected", "p", item );
}
