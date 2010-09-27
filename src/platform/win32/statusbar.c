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

/* Statusbar */
void cgraphics_statusbar_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = parent->native;
	
	if ( !( hwnd = CreateWindowEx( 0, STATUSCLASSNAME,
	                               "",
	                               WS_CHILD | SBARS_SIZEGRIP,
	                               0, 0, 
	                               0, 0,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create button HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_statusbar_widget_update_text( statusbar_widget_t *sb )
{
	SendMessage( sb->widget.native, SB_SETTEXT, 0, (LPARAM)sb->text );
}
