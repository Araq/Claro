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

void cgraphics_combo_fix_size( object_t *object, event_t *event )
{
	widget_t *widget = WIDGET(object);
	
	/* This is called before cgraphics_update_bounds.
	 * here, we ensure that the widget's size will remain
	 * correct. This is because win32 api is designed like shit
	 * 
	 * I don't really like this solution, because it overrides
	 * a part of the layout's results, but it seems ok :/
	 */
	
	widget->size_req->h = 300;
}

void cgraphics_combo_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	if ( !( hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, "COMBOBOX", 
	                               "",
	                               WS_CHILD | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, 300, /* this is the height of the dropdown itself */
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create button HWND.", "Claro error", 0 );
	
	object_addhandler( object, "update", cgraphics_combo_fix_size );
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_combo_new_row( widget_t *widget, list_item_t *item )
{
	item->nativeid = SendMessage( widget->native, CB_INSERTSTRING, item->row, (LPARAM)item->data[0] );
	SendMessage( widget->native, CB_SETITEMDATA, item->nativeid, (LPARAM)item );
}

void cgraphics_combo_remove_row( widget_t *widget, list_item_t *item )
{
	SendMessage( widget->native, CB_DELETESTRING, item->row, 0 );
}

void cgraphics_combo_select_row( widget_t *widget, list_item_t *item )
{
	combo_widget_t *lw = (combo_widget_t *)widget;
	int i = -1;
	
	if ( item != NULL )
		i = item->row;
	
	SendMessage( widget->native, CB_SETCURSEL, i, 0 );
	
	lw->selected = item;
	event_send( OBJECT(widget), "selected", "p", item );
}
