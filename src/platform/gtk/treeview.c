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

gboolean cgraphics_treeview_selected_handler( GtkTreeSelection *selection, gpointer data )
{
	treeview_widget_t *lw = (treeview_widget_t *)data;
	GtkTreeIter iter;
	GtkTreeModel *model;
	list_item_t *selected = 0;
	
	if ( gtk_tree_selection_get_selected( selection, &model, &iter ) )
	{
		gtk_tree_model_get( model, &iter, 2, &selected, -1 );
	}
	
	lw->selected = selected;
	event_send( OBJECT(lw), "selected", "p", selected );
	
	return FALSE;
}

gboolean cgraphics_treeview_rightclick_handler( GtkWidget *widget, GdkEventButton *event, widget_t *cw )
{
	if ( event->type == GDK_BUTTON_PRESS && event->button == 3 )
	{
		GtkTreeModel *model = gtk_tree_view_get_model( GTK_TREE_VIEW(widget) );
		GtkTreePath *path;
		GtkTreeIter iter;
		list_item_t *selected = 0;
		
		if ( gtk_tree_view_get_path_at_pos( GTK_TREE_VIEW(widget), (gint)event->x, (gint)event->y, &path, NULL, NULL, NULL ) == TRUE )
		{
			gtk_tree_model_get_iter( model, &iter, path );
			gtk_tree_model_get( model, &iter, 2, &selected, -1 );
			gtk_tree_path_free( path );
			
			event_send( OBJECT(cw), "right_clicked", "p", selected );
		}
	}
	
	return FALSE;
}

void cgraphics_treeview_widget_create( widget_t *widget )
{
	GtkTreeSelection *select;
	GtkTreeViewColumn *tvc;
	GtkCellRenderer *irenderer, *trenderer;
	GtkTreeModel *model;
	
	widget->native = gtk_scrolled_window_new( NULL, NULL );
	gtk_scrolled_window_set_policy( widget->native, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	gtk_scrolled_window_set_shadow_type( widget->native, GTK_SHADOW_IN );
	widget->container = gtk_tree_view_new( );
	gtk_container_add( widget->native, widget->container );
	gtk_widget_show( GTK_WIDGET( widget->container ) );
	
	cgraphics_widget_create( widget );
	gtk_tree_view_set_headers_visible( widget->container, FALSE );
	
	select = gtk_tree_view_get_selection( GTK_TREE_VIEW(widget->container) );
	gtk_tree_selection_set_mode( select, GTK_SELECTION_SINGLE );
	
	g_signal_connect( G_OBJECT(select), "changed", G_CALLBACK(cgraphics_treeview_selected_handler), widget );
	g_signal_connect( G_OBJECT(widget->container), "button-press-event", G_CALLBACK(cgraphics_treeview_rightclick_handler), widget );
	
	model = (GtkTreeModel *)gtk_tree_store_new( 3, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_POINTER );
	gtk_tree_view_set_model( GTK_TREE_VIEW(widget->container), model );
	g_object_unref( model );
	
	irenderer = gtk_cell_renderer_pixbuf_new( );
	trenderer = gtk_cell_renderer_text_new( );
	
	tvc = gtk_tree_view_column_new( );
	gtk_tree_view_column_set_title( tvc, "Column" );
	gtk_tree_view_column_pack_start( tvc, irenderer, FALSE );
	gtk_tree_view_column_pack_end( tvc, trenderer, TRUE );
	gtk_tree_view_column_add_attribute( tvc, irenderer, "pixbuf", 0 );
	gtk_tree_view_column_add_attribute( tvc, trenderer, "text", 1 );
	
	gtk_tree_view_append_column( GTK_TREE_VIEW(widget->container), tvc );
}

void cgraphics_treeview_item_to_iter( widget_t *widget, list_item_t *item, GtkTreeIter *iter )
{
	GtkTreeStore *store;
	char path[256], tmp[256];
	GtkTreeIter parent;
	list_item_t *curr;
	
	store = (GtkTreeStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	
	if ( item->parent_item == NULL )
	{
		gtk_tree_model_iter_nth_child( GTK_TREE_MODEL(store), iter, NULL, item->row );
		return;
	}
	
	curr = item->parent_item;
	
	strcpy( tmp, "" );
	strcpy( path, "" );
	
	for ( ; curr != 0; curr = curr->parent_item )
	{
		sprintf( tmp, "%d:%s", curr->row, path );
		strcpy( path, tmp );
	}
	
	path[strlen(path)-1] = 0;
	
	gtk_tree_model_get_iter_from_string( GTK_TREE_MODEL(store), &parent, path );
	gtk_tree_model_iter_nth_child( GTK_TREE_MODEL(store), iter, &parent, item->row );
}

void cgraphics_treeview_new_row( widget_t *widget, list_item_t *item )
{
	GtkTreeStore *store;
	GtkTreeIter iter, parent;
	
	store = (GtkTreeStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	
	if ( store == NULL )
		return;
	
	if ( item->parent_item == 0 )
		gtk_tree_store_insert( store, &iter, NULL, item->row );
	else
	{
		cgraphics_treeview_item_to_iter( widget, item->parent_item, &parent );
		gtk_tree_store_insert( GTK_TREE_STORE(store), &iter, &parent, item->row );
	}
	
	if ( item->data[0] != 0 )
	{
		image_t *im = (image_t *)item->data[0];
		gtk_tree_store_set( store, &iter, 0, im->native, -1 );
	}
	gtk_tree_store_set( store, &iter, 1, item->data[1], -1 );
	gtk_tree_store_set( store, &iter, 2, item, -1 );
}

void cgraphics_treeview_remove_row( widget_t *widget, list_item_t *item )
{
	GtkTreeStore *store;
	GtkTreeIter iter;
	
	store = (GtkTreeStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	
	cgraphics_treeview_item_to_iter( widget, item, &iter );
	
	gtk_tree_store_remove( GTK_TREE_STORE(store), &iter );
}

void cgraphics_treeview_select_row( widget_t *widget, list_item_t *item )
{
	GtkTreeSelection *select;
	GtkTreeIter iter;
	
	cgraphics_treeview_item_to_iter( widget, item, &iter );
	
	select = gtk_tree_view_get_selection( GTK_TREE_VIEW(widget->container) );
	gtk_tree_selection_select_iter( select, &iter );
}

void cgraphics_treeview_expand( widget_t *widget, list_item_t *item )
{
	GtkTreeStore *store;
	GtkTreeIter iter;
	
	store = (GtkTreeStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	
	cgraphics_treeview_item_to_iter( widget, item, &iter );
	
	gtk_tree_view_expand_row( GTK_TREE_VIEW(widget->container), gtk_tree_model_get_path(GTK_TREE_MODEL(store),&iter), TRUE );
}

void cgraphics_treeview_collapse( widget_t *widget, list_item_t *item )
{
	GtkTreeStore *store;
	GtkTreeIter iter;
	
	store = (GtkTreeStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	
	cgraphics_treeview_item_to_iter( widget, item, &iter );
	
	gtk_tree_view_collapse_row( GTK_TREE_VIEW(widget->container), gtk_tree_model_get_path(GTK_TREE_MODEL(store),&iter) );
}

void cgraphics_treeview_edit_row( widget_t *widget, list_item_t *item )
{
	GtkTreeStore *store;
	GtkTreeIter iter;
	
	store = (GtkTreeStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	
	cgraphics_treeview_item_to_iter( widget, item, &iter );
	
	if ( item->data[0] != 0 )
	{
		image_t *im = (image_t *)item->data[0];
		gtk_tree_store_set( store, &iter, 0, im->native, -1 );
	}
	gtk_tree_store_set( store, &iter, 1, item->data[1], -1 );
	gtk_tree_store_set( store, &iter, 2, item, -1 );
}
