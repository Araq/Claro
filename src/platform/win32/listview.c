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


#include "platform/win32.h"

LRESULT CALLBACK cg_listview_intercept_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w = (widget_t *)GetWindowLong( hWnd, GWL_USERDATA );
	listview_widget_t *lvw = (listview_widget_t *)w;
	list_item_t *li;
	int a, col;
	HWND *hwnds;
	
	switch ( uMsg )
	{
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
			SetBkMode( (HDC)wParam, TRANSPARENT );
			return (LRESULT)RGB(255,255,255); /* FIXME: hack! */
		case WM_COMMAND:
			li = (list_item_t *)GetWindowLong( (HWND)lParam, GWL_USERDATA );

			if ( li == 0 )
				break;
			
			if ( li->native == 0 )
				break;
			
			hwnds = li->native;
			col = -1;
			
			for ( a = 0; a < lvw->widget.columns; a++ )
			{
				if ( hwnds[a] == (HWND)lParam )
					col = a;
			}
			
			if ( col == -1 )
				break;
			
			switch ( HIWORD( wParam ) )
			{
				case BN_CLICKED:
					*((int *)li->data[col]) = ( SendMessage( (HWND)lParam, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
					
					event_send( OBJECT(li), "changed", "ii", col, *((int *)li->data[col]) );
					
					break;
			}		
	}
	
	return CallWindowProc( lvw->nativep, hWnd, uMsg, wParam, lParam );
}

void cgraphics_listview_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	listview_widget_t *lvw = (listview_widget_t *)widget;
	HWND hwnd, hwnd_parent = widget_get_container(OBJECT(parent));
	
	if ( !( hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, WC_LISTVIEW, 
	                               "",
	                               WS_CHILD | WS_TABSTOP | LVS_AUTOARRANGE | LVS_OWNERDATA | LVS_REPORT,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create listview HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	
	DWORD dw = LVS_EX_FULLROWSELECT;
	
	if ( widget->flags & cListViewRowCheckBoxes )
		dw |= LVS_EX_CHECKBOXES;
	
	SendMessage( hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dw );
	
	SetWindowLong( widget->native, GWL_USERDATA, (LONG)widget );
	lvw->nativep = (void*)SetWindowLong( widget->native, GWL_WNDPROC, (LONG)cg_listview_intercept_proc );
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_listview_widget_create_columns( widget_t *widget )
{
	listview_widget_t *lvw = (listview_widget_t *)widget;
	LVCOLUMN lc;
	int a;
	
	memset( &lc, 0, sizeof( LVCOLUMN ) );
	
	for ( a = 0; a < lvw->widget.columns; a++ )
	{
		lc.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
		lc.fmt = LVCFMT_LEFT;
		lc.pszText = lvw->titles[a];
		lc.cchTextMax = strlen( lvw->titles[a] ) + 1;
		lc.iSubItem = a;
		
		SendMessage( widget->native, LVM_INSERTCOLUMN, a, (LPARAM)&lc );
	}
	
	for ( a = 0; a < lvw->widget.columns; a++ )
		SendMessage( widget->native, LVM_SETCOLUMNWIDTH, a, LVSCW_AUTOSIZE_USEHEADER );
}

void cgraphics_listview_new_row( widget_t *widget, list_item_t *item )
{
	listview_widget_t *lvw = (listview_widget_t *)widget;
	
	item->native = (void *)malloc( sizeof(HWND) * lvw->widget.columns );
	memset( item->native, 0, sizeof(HWND) * lvw->widget.columns );
	
	SendMessage( widget->native, LVM_SETITEMCOUNT, (WPARAM)listview_get_rows(OBJECT(widget)), 0 );
}

void cgraphics_listview_remove_row( widget_t *widget, list_item_t *item )
{
	listview_widget_t *lvw = (listview_widget_t *)widget;
	int a;
	HWND *hwnds;
	
	SendMessage( widget->native, LVM_SETITEMCOUNT, (WPARAM)listview_get_rows(OBJECT(widget)), 0 );
	
	hwnds = item->native;
	
	for ( a = 0; a < lvw->widget.columns; a++ )
	{
		if ( hwnds[a] != 0 )
			DestroyWindow( hwnds[a] );
	}
	
	free( item->native );
}

void cgraphics_listview_select_row( widget_t *widget, list_item_t *item )
{
	listview_widget_t *lw = (listview_widget_t *)widget;
	int i = -1;
	
	if ( item != NULL )
		i = item->row;
	
	SendMessage( widget->native, LVM_SETHOTITEM, i, 0 );
	
	lw->selected = item;
	event_send( OBJECT(widget), "selected", "p", item );
}
