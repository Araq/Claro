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

/* Label */

void cgraphics_label_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	if ( !( hwnd = CreateWindowEx( 0, "STATIC", 
	                               "",
	                               WS_CHILD | SS_LEFTNOWORDWRAP,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create label HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_label_update_text( label_widget_t *widget )
{
	HWND hwnd = widget->widget.native;
	SetWindowText( hwnd, widget->text );
}

void cgraphics_label_update_justify( widget_t *widget )
{
	int a = GetWindowLong( widget->native, GWL_STYLE );
	
	a &= ~(SS_LEFTNOWORDWRAP | SS_RIGHT | SS_LEFT | SS_CENTER);
	
	switch ( widget->flags ) {
		case cLabelRight:
			a |= SS_RIGHT;
			break;
		case cLabelCenter:
			a |= SS_CENTER;
			break;
		case cLabelFill:
		case cLabelLeft:
		default:
			a |= SS_LEFTNOWORDWRAP;
			break;
	}
	
	SetWindowLong( widget->native, GWL_STYLE, a );
}
