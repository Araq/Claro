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

widget_t *cg_find_by_native( HWND hwnd );

LRESULT CALLBACK cg_win32_frame_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w;
	WNDPROC p;
	RECT r;
	
	w = cg_find_by_native( hWnd );
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	/* are we the frame or the container? */
	if ( w->native == hWnd )
	{
		/* frame. */
		p = w->ndata;
		
		if ( uMsg == WM_SIZE )
		{
			MoveWindow( w->container, 10, 20, LOWORD(lParam)-20, HIWORD(lParam)-30, 1 );
			widget_set_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
		}
		
		return CallWindowProc( p, hWnd, uMsg, wParam, lParam );
	}
	
	switch ( uMsg )
	{
		case WM_QUIT:
			return 1;
		case WM_CLOSE:
			DestroyWindow( hWnd );
			return 0;
		case WM_MOVE:
			widget_set_position( w, LOWORD(lParam), HIWORD(lParam), 1 );
			break;
		case WM_SIZE:
			widget_set_content_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
			break;
		case WM_COMMAND:
			return cg_win32_proc( hWnd, uMsg, wParam, lParam );
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/* Frame */
void cgraphics_frame_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent), rhwnd;
	
	WNDCLASSEX wc;
	int wflags, wexflags;
	char clname[1024];
	RECT rect;
	
	sprintf( clname, "claro_frame_%d", ++curr_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_win32_frame_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) GetModuleHandle( NULL );
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground    = NULL; //COLOR_BTNFACE; //(HBRUSH)COLOR_APPWORKSPACE + 1;
	
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= clname;
	
	/* FIXME: use icons when they exist */
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );
	
	if ( !RegisterClassEx( &wc ) )
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	if ( !( hwnd = CreateWindowEx( 0, "BUTTON", 
	                               "",
	                               WS_CHILD | BS_GROUPBOX | WS_GROUP | WS_TABSTOP,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create frame HWND.", "Claro error", 0 );
	
	if ( !( rhwnd = CreateWindowEx( 0, clname, 
	                               "",
	                               WS_CHILD,
	                               10, 20,
	                               widget->size_req->w-20, widget->size_req->h-30,
	                               hwnd,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create frame HWND(2).", "Claro error", 0 );
	
	widget->ndata = SetWindowLongPtr( hwnd, GWLP_WNDPROC, (long)&cg_win32_frame_proc );
	
	widget->native = hwnd;
	widget->container = rhwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	
	ShowWindow( rhwnd, SW_SHOW );
	UpdateWindow( hwnd );
	UpdateWindow( rhwnd );
	
	// set the size of the widget to the size of the container :)
	widget_set_size( widget, widget->size_req->w-20, widget->size_req->h-30, 1 );
	
}

void cgraphics_frame_set_text( frame_widget_t *widget )
{
	HWND hwnd = widget->widget.native;
	SetWindowText( hwnd, widget->text );
}
