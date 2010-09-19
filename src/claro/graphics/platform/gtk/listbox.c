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

/* ListBox */

static void cgraphics_listbox_selected_handler( GtkTreeSelection *selection, gpointer data )
{
	listbox_widget_t *lw = (listbox_widget_t *)data;
	GtkTreeIter iter;
	GtkTreeModel *model;
	list_item_t *selected = 0;
	
	if ( gtk_tree_selection_get_selected( selection, &model, &iter ) )
	{
		gtk_tree_model_get( model, &iter, 1, &selected, -1 );
	}
	
	lw->selected = selected;
	event_send( OBJECT(lw), "selected", "p", selected );
}

void cgraphics_listbox_widget_create( widget_t *widget )
{
	GtkTreeSelection *select;
	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	
	widget->native = gtk_scrolled_window_new( NULL, NULL );
	gtk_scrolled_window_set_policy( widget->native, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	gtk_scrolled_window_set_shadow_type( widget->native, GTK_SHADOW_IN );
	widget->container = gtk_tree_view_new( );
	gtk_container_add( widget->native, widget->container );
	gtk_widget_show( GTK_WIDGET( widget->container ) );
	
	cgraphics_widget_create( widget );
	
	renderer = gtk_cell_renderer_text_new( );
	gtk_tree_view_insert_column_with_attributes( GTK_TREE_VIEW(widget->container),
					-1, "Field", renderer, "text", 0, NULL );
	
	model = (GtkTreeModel *)gtk_list_store_new( 2, G_TYPE_STRING, G_TYPE_UINT );
	
	gtk_tree_view_set_model( GTK_TREE_VIEW(widget->container), model );
	
	g_object_unref( model );
	
	gtk_tree_view_set_headers_visible( GTK_TREE_VIEW(widget->container), FALSE );
	
	select = gtk_tree_view_get_selection( GTK_TREE_VIEW(widget->container) );
	gtk_tree_selection_set_mode( select, GTK_SELECTION_SINGLE );
	g_signal_connect( G_OBJECT(select), "changed", G_CALLBACK(cgraphics_listbox_selected_handler), widget );
}

void cgraphics_listbox_new_row( widget_t *widget, list_item_t *item )
{
	GtkListStore *store = (GtkListStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	GtkTreeIter iter;
	
	if ( store == NULL )
		return;
	
	gtk_list_store_insert( store, &iter, item->row );
	gtk_list_store_set( store, &iter, 0, item->data[0], 1, item, -1 );
}

void cgraphics_listbox_remove_row( widget_t *widget, list_item_t *item )
{
	GtkListStore *store = (GtkListStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	GtkTreeIter iter;
	
	if ( store == NULL )
		return;
	
	gtk_tree_model_iter_nth_child( (GtkTreeModel *)store, &iter, NULL, item->row );
	
	gtk_list_store_remove( store, &iter );
}

void cgraphics_listbox_select_row( widget_t *widget, list_item_t *item )
{
	GtkListStore *store = (GtkListStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	GtkTreeSelection *select;
	GtkTreeIter iter;
	
	if ( store == NULL )
		return;
	
	gtk_tree_model_iter_nth_child( (GtkTreeModel *)store, &iter, NULL, item->row );
	
	select = gtk_tree_view_get_selection( GTK_TREE_VIEW(widget->container) );
	gtk_tree_selection_select_iter( select, &iter );
}
