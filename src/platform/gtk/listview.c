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

typedef struct
{
	widget_t *widget;
	int column;
} cgcelldata;

/* ListView */

static void cgraphics_listview_selected_handler( GtkTreeSelection *selection, gpointer data )
{
	listview_widget_t *lw = (listview_widget_t *)data;
	GtkTreeIter iter;
	GtkTreeModel *model;
	list_item_t *selected = 0;
	
	if ( gtk_tree_selection_get_selected( selection, &model, &iter ) )
	{
		gtk_tree_model_get( model, &iter, lw->widget.columns, &selected, -1 );
	}
	
	lw->selected = selected;
	event_send( OBJECT(lw), "selected", "p", selected );
}

void cgraphics_listview_toggled_handler( GtkCellRendererToggle *cell_renderer, gchar *path, cgcelldata *data )
{
	GtkListStore *store = (GtkListStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(data->widget->container) );
	listview_widget_t *lw = (listview_widget_t *)data->widget;
	GtkTreeIter iter;
	list_item_t *selected = 0;
	
	if ( gtk_tree_model_get_iter_from_string( GTK_TREE_MODEL(store), 
	     &iter, path ) )
	{
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, lw->widget.columns, &selected, -1 );
	}

	if ( selected == 0 )
		return;
	
	unsigned int *idt = selected->data[data->column];
	idt[0] = !idt[0];
	gtk_list_store_set( store, &iter, data->column, idt[0], -1 );
	event_send( OBJECT(selected), "changed", "ii", data->column, idt[0] );
}

void cgraphics_listview_widget_create( widget_t *widget )
{
	GtkTreeSelection *select;
	
	widget->native = gtk_scrolled_window_new( NULL, NULL );
	gtk_scrolled_window_set_policy( widget->native, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	gtk_scrolled_window_set_shadow_type( widget->native, GTK_SHADOW_IN );
	widget->container = gtk_tree_view_new( );
	gtk_container_add( widget->native, widget->container );
	gtk_widget_show( GTK_WIDGET( widget->container ) );
	
	cgraphics_widget_create( widget );
	
	select = gtk_tree_view_get_selection( GTK_TREE_VIEW(widget->container) );
	gtk_tree_selection_set_mode( select, GTK_SELECTION_SINGLE );
	g_signal_connect( G_OBJECT(select), "changed", G_CALLBACK(cgraphics_listview_selected_handler), widget );
}

void cgraphics_listview_widget_create_columns( widget_t *widget )
{
	listview_widget_t *lvw = (listview_widget_t *)widget;
	GtkCellRenderer *renderer;
	GType *types;
	GtkTreeModel *model;
	int a;
	
	types = (GType *)malloc( sizeof(GType) * (lvw->widget.columns+1) );
	
	for ( a = 0; a < lvw->widget.columns; a++ )
	{
		if ( lvw->widget.coltypes[a] == CLIST_TYPE_STRING )
			types[a] = G_TYPE_STRING;
		else if ( lvw->widget.coltypes[a] == CLIST_TYPE_UINT )
			types[a] = G_TYPE_UINT;
		else if ( lvw->widget.coltypes[a] == CLIST_TYPE_DOUBLE )
			types[a] = G_TYPE_DOUBLE;
		else
		{
			printf( "Bad error: invalid type found (%d)\n", lvw->widget.coltypes[a] );
			exit( 0 );
		}
	}
	
	types[a] = G_TYPE_UINT;
	
	model = (GtkTreeModel *)gtk_list_store_newv( lvw->widget.columns + 1, types );
	free( types );
	
	gtk_tree_view_set_model( GTK_TREE_VIEW(widget->container), model );
	
	g_object_unref( model );
	
	for ( a = 0; a < lvw->widget.columns; a++ )
	{
		if ( lvw->widget.coltypes[a] == CLIST_TYPE_STRING )
		{
			renderer = gtk_cell_renderer_text_new( );
			gtk_tree_view_insert_column_with_attributes( GTK_TREE_VIEW(widget->container),
						-1, lvw->titles[a], renderer, "text", a, NULL );
		}
		else if ( lvw->widget.coltypes[a] == CLIST_TYPE_UINT )
		{
			cgcelldata *cgcd = (cgcelldata *)malloc( sizeof(cgcelldata) ); /* FIXME: free me somewhere. */
			renderer = gtk_cell_renderer_toggle_new( );
			gtk_tree_view_insert_column_with_attributes( GTK_TREE_VIEW(widget->container),
						-1, lvw->titles[a], renderer, "active", a, NULL );
			cgcd->widget = widget;
			cgcd->column = a;
			g_signal_connect( G_OBJECT(renderer), "toggled", G_CALLBACK(cgraphics_listview_toggled_handler), cgcd );
		}
		else if ( lvw->widget.coltypes[a] == CLIST_TYPE_DOUBLE )
		{
			renderer = gtk_cell_renderer_progress_new( );
			gtk_tree_view_insert_column_with_attributes( GTK_TREE_VIEW(widget->container),
						-1, lvw->titles[a], renderer, "value", a, NULL );
		}
	}
}

void cgraphics_listview_new_row( widget_t *widget, list_item_t *item )
{
	GtkListStore *store = (GtkListStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	GtkTreeIter iter;
	listview_widget_t *lvw = (listview_widget_t *)widget;
	int a;
	
	if ( store == NULL )
		return;
	
	gtk_list_store_insert( store, &iter, item->row );
	
	for ( a = 0; a < lvw->widget.columns; a++ )
	{
		if ( lvw->widget.coltypes[a] == CLIST_TYPE_STRING )
			gtk_list_store_set( store, &iter, a, item->data[a], -1 );
		else if ( lvw->widget.coltypes[a] == CLIST_TYPE_UINT )
		{
			unsigned int *idt = item->data[a];
			gtk_list_store_set( store, &iter, a, idt[0], -1 );
		}
		else if ( lvw->widget.coltypes[a] == CLIST_TYPE_DOUBLE )
		{
			double *ddt = item->data[a];
			gtk_list_store_set( store, &iter, a, ddt[0]*100, -1 );
		}
	}
	
	gtk_list_store_set( store, &iter, a, item, -1 );
}

void cgraphics_listview_remove_row( widget_t *widget, list_item_t *item )
{
	GtkListStore *store = (GtkListStore *)gtk_tree_view_get_model( GTK_TREE_VIEW(widget->container) );
	GtkTreeIter iter;
	
	if ( store == NULL )
		return;
	
	gtk_tree_model_iter_nth_child( (GtkTreeModel *)store, &iter, NULL, item->row );
	
	gtk_list_store_remove( store, &iter );
}

void cgraphics_listview_select_row( widget_t *widget, list_item_t *item )
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
