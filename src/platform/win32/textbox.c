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

/* TextBox */

void cgraphics_textbox_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(OBJECT(parent));
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;

	if ( !( hwnd = CreateWindowEx( ( widget->flags & cWidgetNoBorder ? 0 : WS_EX_CLIENTEDGE ), "EDIT", 0,
	                               WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP |
	                               (widget->flags & cTextBoxTypePassword ? ES_PASSWORD : 0),
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create text HWND.", "Claro error", 0 );
	
	widget->native = hwnd;

	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_textbox_update_text( textbox_widget_t *widget )
{
	HWND hwnd = widget->widget.native;
	SetWindowText( hwnd, widget->text );
}

void cgraphics_textbox_set_pos( widget_t *widget, int pos )
{
	SendMessage( widget->native, EM_SETSEL, pos, pos );
}

int cgraphics_textbox_get_pos( widget_t *widget )
{
	int pos;
	SendMessage( widget->native, EM_GETSEL, (size_t)&pos, 0 );
	return pos;
}
