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

/* Container */

LRESULT CALLBACK cg_container_win32_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w;
	container_widget_t *sw;
	native_container_widget_t *nw;
	//bounds_t b;
	
	w = cg_find_by_native( hWnd );
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	sw = (container_widget_t *)w;
	nw = (native_container_widget_t *)w->ndata;
	
	switch ( uMsg )
	{
		case WM_MOVE:
			widget_set_position( w, LOWORD(lParam), HIWORD(lParam), 1 );
			break;
		case WM_SIZE:
			widget_set_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
			widget_set_content_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
			widget_resized_handle( OBJECT(w), 0 );
			break;
		case WM_COMMAND:
		case WM_NOTIFY:
			return cg_win32_proc( hWnd, uMsg, wParam, lParam );
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void cgraphics_container_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	//container_widget_t *sw = (container_widget_t *)widget;
	//native_container_widget_t *nw = (native_container_widget_t *)sw->widget.ndata;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	WNDCLASSEX wc;
	char clname[1024];
	
	sprintf( clname, "claro_window_%d", ++curr_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_container_win32_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) GetModuleHandle( NULL );
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground    = (HBRUSH)COLOR_APPWORKSPACE + 1;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= clname;
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if ( !RegisterClassEx( &wc ) )
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	
	if ( !( hwnd = CreateWindowEx( 0, clname, 
									0,
									WS_CHILD,
									widget->size_req->x, widget->size_req->y,
									widget->size_req->w, widget->size_req->h,
									hwnd_parent,
									NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create container HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	widget->container = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}
