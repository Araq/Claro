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

void cgraphics_workspace_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = cgraphics_get_widget_window( WIDGET(object->parent) );
	window_widget_t *wp = (window_widget_t *)parent;
	HWND hwnd, hwnd_parent = parent->native;//widget_get_container(parent);
	CLIENTCREATESTRUCT ccs;
	
	ccs.hWindowMenu  = 0;
	ccs.idFirstChild = 0xF0000;
	
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"mdiclient",
		NULL,
		WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
		widget->size_req->x, widget->size_req->y,
		widget->size_req->w, widget->size_req->h, 
		hwnd_parent, NULL, (HINSTANCE) GetModuleHandle( NULL ), (LPVOID)&ccs
	);
	
	if ( hwnd == NULL )
	{
		MessageBox(NULL, "Workspace (MDIClient) Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	
	widget->native = hwnd;
	
	/* incase menu has already been created */
	if ( wp->menubar != 0 )
	{
		SetMenu( hwnd, wp->menubar->native );
		DrawMenuBar( hwnd );
	}
}

void cgraphics_workspace_set_active( widget_t *widget, object_t *child )
{
	
}

object_t *cgraphics_workspace_get_active( widget_t *widget )
{
	HWND win;
	
	win = (HWND)SendMessage( widget->native, WM_MDIGETACTIVE, 0, 0 );
	
	if ( win == 0 )
		return 0;
	
	return GetWindowLong( win, GWL_USERDATA );;
}

void cgraphics_workspace_cascade( widget_t *widget )
{
	SendMessage( widget->native, WM_MDICASCADE, MDITILE_SKIPDISABLED, 0 );
}

void cgraphics_workspace_tile( widget_t *widget, int dir )
{
	SendMessage( widget->native, WM_MDITILE, ( dir == cWorkspaceTileHorizontally ? MDITILE_HORIZONTAL : MDITILE_VERTICAL ), 0 );
}

/* workspace window */

LRESULT CALLBACK cg_win32_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

LRESULT CALLBACK cg_mdiclient_win32_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w;
	
	w = cg_find_by_native( hWnd );
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	if ( hWnd == w->container )
	{
		switch ( uMsg )
		{
			
		}
	}
	
	return cg_win32_proc( hWnd, uMsg, wParam, lParam );
}

void cgraphics_workspace_window_widget_create( widget_t *widget )
{
	WNDCLASSEX wc;
	HWND hwnd, rhwnd, wparent;
	int wflags;
	char clname[1024];
	REBARINFO rbi;
	RECT rect;
	
	sprintf( clname, "claro_window_%d", ++curr_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_mdiclient_win32_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) GetModuleHandle( NULL );
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE + 1;
	
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
	
	wflags = WS_OVERLAPPEDWINDOW;
	
	wparent = WIDGET(OBJECT(widget)->parent)->native;
	
	rect.left = widget->size_req->x;
	rect.top = widget->size_req->y;
	rect.right = widget->size_req->x + widget->size_req->w;
	rect.bottom = widget->size_req->y + widget->size_req->h;
	
	AdjustWindowRectEx( &rect, wflags, 0, 0 );
	
	/*hwnd = CreateWindowEx(
			wexflags,
			clname,
			"Default Workspace Window - Claro",
			wflags,
			rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			wparent, NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL
		);*/
	
	widget->size_req->x = rect.left;
	widget->size_req->y = rect.top;
	widget->size_req->w = rect.right - rect.left;
	widget->size_req->h = rect.bottom - rect.top;
	
	int rx, ry;
	
	rx = widget->size_req->x;
	ry = widget->size_req->y;
	
	if ( rx < 0 )
		rx = CW_USEDEFAULT;
	
	if ( ry < 0 )
		ry = CW_USEDEFAULT;
	
	hwnd = CreateMDIWindow(
				clname,
				"Default Workspace Window - Claro",
				0,
				rx, ry,
				widget->size_req->w, widget->size_req->h,
				wparent,
				(HINSTANCE) GetModuleHandle( NULL ),
				MAKELPARAM( 0, 0 )
			);
	
	ShowWindow( hwnd, SW_HIDE );
	
	if ( !( rhwnd = CreateWindowEx( 0, clname, 
	                               "",
	                               WS_CHILD,
	                               0, 0,
	                               rect.right - rect.left, rect.bottom - rect.top,
	                               hwnd,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create window internal HWND.", "Claro error", 0 );
	
	ShowWindow( rhwnd, SW_SHOW );
	
	/**** REBAR CREATION ****/
	
	widget->ndata = CreateWindowEx( WS_EX_TOOLWINDOW,
							REBARCLASSNAME,
							NULL,
							WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBS_VARHEIGHT | 
							RBS_BANDBORDERS | CCS_NODIVIDER | CCS_NOPARENTALIGN,
							0, 0, CW_USEDEFAULT, 0,
							hwnd,
							NULL,
							(HINSTANCE) GetModuleHandle( NULL ),
							NULL );
	
	rbi.cbSize = sizeof( REBARINFO );
	rbi.fMask  = 0;
	rbi.himl   = (HIMAGELIST)NULL;
	
	if ( !SendMessage( widget->ndata, RB_SETBARINFO, 0, (LPARAM)&rbi ) )
	{
		MessageBox( 0, "Could not create window: RB_SETBARINFO", 0, 0 );
	}
	
	/**** END REBAR CREATION ****/
	
	widget->native = hwnd;
	widget->container = rhwnd;
	
	SetWindowLong( widget->native, GWL_USERDATA, widget );
	SetWindowLong( widget->container, GWL_USERDATA, widget );
}

void cgraphics_workspace_window_update_title( widget_t *widget )
{
	workspace_window_widget_t *ww = (workspace_window_widget_t *)widget;
	HWND hwnd = widget->native;
	SetWindowText( hwnd, ww->title );
}

void cgraphics_workspace_window_show( widget_t *widget )
{
	ShowWindow( widget->native, SW_SHOW );
}

void cgraphics_workspace_window_hide( widget_t *widget )
{
	ShowWindow( widget->native, SW_HIDE );
}

void cgraphics_workspace_window_restore( widget_t *widget )
{
	ShowWindow( widget->native, SW_RESTORE );
}

void cgraphics_workspace_window_minimise( widget_t *widget )
{
	ShowWindow( widget->native, SW_MINIMIZE );
}

void cgraphics_workspace_window_maximise( widget_t *widget )
{
	ShowWindow( widget->native, SW_MAXIMIZE );
}

void cgraphics_workspace_window_update_icon( widget_t *w )
{
	workspace_window_widget_t *ww = (workspace_window_widget_t *)w;
	
	if ( ww->icon != 0 )
	{
		SetClassLong( w->native, GCL_HICON, (long)ww->icon->icon );
		SetClassLong( w->native, GCL_HICONSM, (long)ww->icon->icon );
	}
}
