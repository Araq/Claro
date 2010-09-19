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


#include "../win32.h"

void cgraphics_treeview_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	HIMAGELIST himl;
	HBITMAP bmp;
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	if ( !( hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, WC_TREEVIEW, 
	                               "Tree View",
	                               WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create treeview HWND.", "Claro error", 0 );
	
	himl = ImageList_Create( 16, 16, ILC_COLOR32, 0, 1024 );
	bmp = CreateBitmap( 16, 16, 1, 1, 0 );
	ImageList_Add( himl, bmp, 0 );
	TreeView_SetImageList( hwnd, himl, TVSIL_NORMAL ); 
	widget->ndata = himl;
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_treeview_new_row( widget_t *widget, list_item_t *item )
{
	TVITEM tvi; 
	TVINSERTSTRUCT tvins;
	HTREEITEM hti;
	list_item_t *pitem, *litem;
	HIMAGELIST himl = widget->ndata;
	
	memset( &tvi, 0, sizeof(TVITEM) );
	
	tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;// | TVIF_SELECTEDIMAGE;
	
	if ( item->data[0] != 0 )
	{
		image_t *im = (image_t *)item->data[0];
		
		tvi.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvi.iImage = ImageList_Add( himl, im->native2, 0 ); //I_IMAGECALLBACK;
		tvi.iSelectedImage = ImageList_Add( himl, im->native2, 0 ); //I_IMAGECALLBACK;
	}
	
	tvi.pszText = item->data[1]; 
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
	
	tvi.lParam = item;
	
	tvins.item = tvi;
	
	tvins.hParent = TVI_ROOT;
	
	pitem = 0;
	
	if ( OBJECT(item)->parent != 0 && !strcmp( OBJECT(item)->parent->type, "claro.graphics.list_item" ) )
	{
		pitem = (list_item_t *)(OBJECT(item)->parent);
		tvins.hParent = pitem->native;
	}
	
	if ( item->row == 0 )
		tvins.hInsertAfter = TVI_FIRST;
	else
	{
		litem = list_widget_get_row( widget, pitem, item->row - 1 );
		tvins.hInsertAfter = litem->native;
	}
	
	item->native = SendMessage( widget->native, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins );
}

void cgraphics_treeview_remove_row( widget_t *widget, list_item_t *item )
{
	TreeView_DeleteItem( widget->native, item->native );
}

void cgraphics_treeview_select_row( widget_t *widget, list_item_t *item )
{
	treeview_widget_t *lw = (treeview_widget_t *)widget;
	int i = -1;
	
	if ( item != NULL )
		i = item->row;
	
	TreeView_SelectItem( widget->native, item->native );
	
	lw->selected = item;
	event_send( OBJECT(widget), "selected", "p", item );
}

void cgraphics_treeview_expand( widget_t *widget, list_item_t *item )
{
	TreeView_Expand( widget->native, item->native, TVE_EXPAND );
}

void cgraphics_treeview_collapse( widget_t *widget, list_item_t *item )
{
	TreeView_Expand( widget->native, item->native, TVE_COLLAPSE );
}

void cgraphics_treeview_edit_row( widget_t *widget, list_item_t *item )
{
	TVITEM tvi;
	
	tvi.mask = TVIF_TEXT | TVIF_HANDLE;
	
	tvi.pszText = item->data[1]; 
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]);
    
    tvi.hItem = item->native;
    
	TreeView_SetItem( widget->native, &tvi );
}
