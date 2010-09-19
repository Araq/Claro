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

void cgraphics_listbox_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	if ( !( hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, "LISTBOX", 
	                               "",
	                               WS_CHILD | LBS_NOTIFY | WS_TABSTOP | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_HSCROLL,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create button HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_listbox_new_row( widget_t *widget, list_item_t *item )
{
	item->nativeid = SendMessage( widget->native, LB_INSERTSTRING, item->row, (LPARAM)item->data[0] );
	SendMessage( widget->native, LB_SETITEMDATA, item->nativeid, (LPARAM)item );
}

void cgraphics_listbox_remove_row( widget_t *widget, list_item_t *item )
{
	SendMessage( widget->native, LB_DELETESTRING, item->row, 0 );
}

void cgraphics_listbox_select_row( widget_t *widget, list_item_t *item )
{
	listbox_widget_t *lw = (listbox_widget_t *)widget;
	int i = -1;
	
	if ( item != NULL )
		i = item->row;
	
	SendMessage( widget->native, LB_SETCURSEL, i, 0 );
	
	lw->selected = item;
	event_send( OBJECT(widget), "selected", "p", item );
}
