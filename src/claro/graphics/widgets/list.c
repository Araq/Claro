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
#include <claro/graphics/platform.h>

/* These functions should not be used in client applications. They are used internally and
 * widgets like ListView should be used to actually do something
 */

void list_widget_init_ptr( object_t *obj, unsigned int col_num, int *cols )
{
	list_widget_t *lw = (list_widget_t *)obj;
	
	lw->columns = col_num;
	lw->coltypes = (int *)malloc( sizeof(int) * col_num );
	
	memcpy( lw->coltypes, cols, sizeof(int) * col_num );
	
	list_create( &lw->items );
}

void list_widget_init_vaptr( object_t *obj, unsigned int col_num, va_list argpi )
{
	int *cols, a;
	va_list argp;
	
	va_copy( argp, argpi );
	
	cols = (int *)malloc( sizeof(int) * col_num );
	
	for ( a = 0; a < col_num; a++ )
		cols[a] = va_arg( argp, int );
	
	va_end( argp );
	
	list_widget_init_ptr( obj, col_num, cols );
	
	free( cols );
}

void list_widget_init( object_t *obj, unsigned int col_num, ... )
{
	va_list argp;
	
	va_start( argp, col_num );
	
	list_widget_init_vaptr( obj, col_num, argp );
	
	va_end( argp );
}

void list_widget_row_remove( object_t *list, list_item_t *item )
{
	list_widget_t *lw = (list_widget_t *)list;
	node_t *n, *n2;
	list_item_t *curr;
	int a;
	
	event_send( list, "remove_row", "p", item );
	
	LIST_FOREACH_SAFE( n, n2, item->parent->head )
	{
		curr = (list_item_t *)n->data;
		
		if ( curr == item )
		{
			node_del( n, item->parent );
			node_free( n );
			continue;
		}
		
		/* fix row numbers */
		if ( curr->row > item->row )
			curr->row--;
	}
	
	for ( a = 0; a < lw->columns; a++ )
	{
		/* special case: free strings */
		/*if ( lw->coltypes[a] == CLIST_TYPE_STRING && item->data[a] != 0 )
		{
			free( item->data[a] );
		}*/
		if ( lw->coltypes[a] == CLIST_TYPE_PTR )
			continue;
		
		if ( item->data[a] != 0 )
			free( item->data[a] );
	}
	
	free( item->data );
	//free( item );
	
	event_send( OBJECT(item), "destroy", "" );
}

void list_widget_row_move( object_t *list, list_item_t *item, int row )
{
	node_t *n;
	list_item_t *curr;
	
	event_send( list, "remove_row", "p", item );
	
	LIST_FOREACH( n, item->parent->head )
	{
		curr = (list_item_t *)n->data;
		
		if ( curr == item )
			continue;
		
		/* fix row numbers */
		if ( curr->row > item->row )
			curr->row--;
		
		if ( curr->row >= row )
			curr->row++;
	}
	
	item->row = row;
	
	event_send( list, "new_row", "p", item );
}

list_item_t *list_widget_row_insert_ptr( object_t *list, list_item_t *parent, int row, va_list argpi )
{
	va_list argp;
	list_widget_t *lw = (list_widget_t *)list;
	list_item_t *item, *curr;
	list_t *l;
	node_t *n;
	int a;

	va_copy(argp, argpi);
	
	/* create the item and fill it */
	item = (list_item_t *)object_create( (parent==NULL?list:OBJECT(parent)), sizeof(list_item_t), "claro.graphics.list_item" );
	
	item->data = (void **)malloc( sizeof(void *) * lw->columns );
	
	//memcpy( item->data, argp, sizeof(void *) * lw->columns );
	
	for ( a = 0; a < lw->columns; a++ )
	{
		item->data[a] = 0;
		
		/* special case: strings get duplicated */
		if ( lw->coltypes[a] == CLIST_TYPE_STRING )
		{
			char *str = va_arg( argp, char * );
			
			if ( str != NULL )
				item->data[a] = (void *)strdup( str );
		}
		else if ( lw->coltypes[a] == CLIST_TYPE_DOUBLE )
		{
			double *fp;
			
			fp = (double *)malloc( sizeof(double) );
			item->data[a] = (void *)fp;
			
			*fp = va_arg( argp, double );
		}
		else if ( lw->coltypes[a] == CLIST_TYPE_INT || lw->coltypes[a] == CLIST_TYPE_UINT )
		{
			int *ip;
			
			ip = (int *)malloc( sizeof(int) );
			item->data[a] = (void *)ip;
			
			*ip = va_arg( argp, int );
		}
		else if ( lw->coltypes[a] == CLIST_TYPE_PTR )
		{
			item->data[a] = va_arg( argp, void * );
		}
	}
	
	list_create( &item->children );
	
	/* pick the parent list.. */
	if ( parent != 0 )
		l = &parent->children;
	else
		l = &lw->items;
	
	item->parent = l;
	item->parent_item = parent;
	
	/* save our row number */
	if ( row == -1 )
	{
		item->row = LIST_LENGTH(l);
	}
	else
	{
		if ( row < 0 )
			row = 0;
		
		if ( row > LIST_LENGTH(l) )
			row = LIST_LENGTH(l);
		
		item->row = row;
		
		/* increment row numbers that are in our new spot or further */
		LIST_FOREACH( n, l->head )
		{
			curr = (list_item_t *)n->data;
			
			if ( curr->row >= row )
				curr->row++;
		}
	}

	/* create a node, append to the list */
	n = node_create( );
	node_append( item, n, l );

	event_send( list, "new_row", "p", item );

	va_end(argp);
	
	return item;
}

list_item_t *list_widget_row_insert( object_t *list, list_item_t *parent, int pos, ... )
{
	list_item_t *item;
	va_list argp;
	
	va_start( argp, pos );
	
	item = list_widget_row_insert_ptr( list, parent, pos, argp );
	
	va_end( argp );
	
	return item;
}

list_item_t *list_widget_row_append( object_t *list, list_item_t *parent, ... )
{
	list_item_t *item;
	va_list argp;
	
	va_start( argp, parent );
	
	item = list_widget_row_insert_ptr( list, parent, -1, argp );
	
	va_end( argp );
	
	return item;
}

list_item_t *list_widget_get_row( object_t *list, list_item_t *parent, int row )
{
	list_widget_t *lw = (list_widget_t *)list;
	list_item_t *curr;
	list_t *l;
	node_t *n;
	
	if ( parent != 0 )
		l = &parent->children;
	else
		l = &lw->items;
	
	LIST_FOREACH( n, l->head )
	{
		curr = (list_item_t *)n->data;
		
		if ( curr->row == row )
			return curr;
	}
	
	return NULL;
}

void list_widget_edit_row_ptr( object_t *list, list_item_t *item, va_list argpi )
{
	va_list argp;
	list_widget_t *lw = (list_widget_t *)list;
	int a;

	va_copy(argp, argpi);
	
	for ( ; ; )
	{
		a = va_arg( argp, int );
		
		if ( a == -1 )
			break;
		
		if ( lw->coltypes[a] != CLIST_TYPE_PTR )
		{
			if ( item->data[a] != 0 )
				free( item->data[a] );
		}
		
		/* special case: strings get duplicated */
		if ( lw->coltypes[a] == CLIST_TYPE_STRING )
		{
			char *str = va_arg( argp, char * );
			
			if ( str != NULL )
				item->data[a] = (void *)strdup( str );
		}
		else if ( lw->coltypes[a] == CLIST_TYPE_DOUBLE )
		{
			double *fp;
			
			fp = (double *)malloc( sizeof(double) );
			item->data[a] = (void *)fp;
			
			*fp = va_arg( argp, double );
		}
		else if ( lw->coltypes[a] == CLIST_TYPE_INT || lw->coltypes[a] == CLIST_TYPE_UINT )
		{
			int *ip;
			
			ip = (int *)malloc( sizeof(int) );
			item->data[a] = (void *)ip;
			
			*ip = va_arg( argp, int );
		}
		else if ( lw->coltypes[a] == CLIST_TYPE_PTR )
		{
			item->data[a] = va_arg( argp, void * );
		}
	}

	va_end(argp);
	
	event_send( list, "edit_row", "p", item );
}

void list_widget_edit_row( object_t *list, list_item_t *item, ... )
{
	va_list argp;
	
	va_start( argp, item );
	
	list_widget_edit_row_ptr( list, item, argp );
	
	va_end( argp );
}



void list_item_set_text_color( list_item_t *item, float r, float g, float b, float a )
{
	item->text_color.r = r;
	item->text_color.g = g;
	item->text_color.b = b;
	item->text_color.a = a;
	
	item->text_color.used = 1;
}

void list_item_set_text_bgcolor( list_item_t *item, float r, float g, float b, float a )
{
	item->back_color.r = r;
	item->back_color.g = g;
	item->back_color.b = b;
	item->back_color.a = a;
	
	item->back_color.used = 1;
}

void list_item_set_sel_text_color( list_item_t *item, float r, float g, float b, float a )
{
	item->sel_text_color.r = r;
	item->sel_text_color.g = g;
	item->sel_text_color.b = b;
	item->sel_text_color.a = a;
	
	item->sel_text_color.used = 1;
}

void list_item_set_sel_text_bgcolor( list_item_t *item, float r, float g, float b, float a )
{
	item->sel_back_color.r = r;
	item->sel_back_color.g = g;
	item->sel_back_color.b = b;
	item->sel_back_color.a = a;
	
	item->sel_back_color.used = 1;
}

void list_item_set_font_extra( list_item_t *item, int weight, int slant, int decoration )
{
	if ( item->font.used == 0 )
	{
		item->font.size = 16; // FIXME: this should be the OS's default.. hmm.
	}
	
	item->font.used = 2;
	item->font.weight = weight;
	item->font.slant = slant;
	item->font.decoration = decoration;
}
