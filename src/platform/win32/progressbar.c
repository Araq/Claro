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

/* Progressbar */
void cgraphics_progress_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	if ( !( hwnd = CreateWindowEx( 0, PROGRESS_CLASS, 
	                               0,
	                               WS_CHILD | WS_VISIBLE,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create progressbar HWND.", "Claro error", 0 );
	
	SendMessage( hwnd, PBM_SETRANGE, 0, MAKELPARAM( 0, 65535 ) );
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_progress_set_level( widget_t *widget, double percentage )
{
	int intpos = (int)((float)percentage * 65535.0);
	
	SendMessage( widget->native, PBM_SETPOS, intpos, 0 );
}

void cgraphics_progress_set_orientation( widget_t *widget, int flags)
{
	
}
