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


#include "win32.h"

HFONT nicerfont;
int cg_isxplater = 0;

/* called at initialisation */
void cgraphics_init( )
{
	INITCOMMONCONTROLSEX icc;
	OSVERSIONINFO osv;
	
	icc.dwSize = sizeof( INITCOMMONCONTROLSEX );
	icc.dwICC = ICC_WIN95_CLASSES | ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx( &icc );
	
	CoInitialize( NULL );
	/* FIXME: call CoUninitialize(); when we finish! */
	
	nicerfont = CreateFont( 16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 
							OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
							ANTIALIASED_QUALITY | CLEARTYPE_QUALITY, 
							DEFAULT_PITCH | FF_DONTCARE, "Tahoma" );
	
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &osv );
	
	if ( osv.dwMajorVersion > 5 || (osv.dwMajorVersion == 5 && osv.dwMinorVersion >= 1) )
	{
		cg_isxplater = 1;
	}
	
	cg_isxplater = 0; // eh, for now ignore this because listview doesn't like compositing
}

widget_t *cgraphics_get_widget_window( widget_t *w )
{
	object_t *o ;
	
	o = (object_t *)w;
	
	if ( !strcmp( o->type, "claro.graphics.widgets.window" ) )
		return w;
	
	if ( o->parent == 0 )
		return 0;
	
	return cgraphics_get_widget_window( (widget_t *)o->parent );
}

int dialog_stack_check( widget_t *r );

/* intercept window procedure */
LRESULT CALLBACK cg_intercept_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w = GetWindowLong( hWnd, GWL_USERDATA );
	widget_t *win;
	PAINTSTRUCT ps;
	HDC hdc, hdcbuf;
	HBITMAP hbmp, nhbmp;
	RECT r;
	int y;
	SCROLLINFO si;
	DWORD s;
	char *tmp = "";
	
	if ( w == 0 )
		return 1;
	
	switch ( uMsg )
	{
		case WM_SETFOCUS:
			event_send( OBJECT(w), "focus", "" );
			break;
		case WM_DESTROY:
			SetWindowLong( hWnd, GWL_USERDATA, 0 );
			
			/* pop if we're dialog :) */
			dialog_stack_pop( w );
			
			/* only destroy the claro widget if it's the real widget,
			   NOT the container (avoids duplicate destroy) */
			if ( w->native == hWnd )
				widget_destroy( w );
			
			return 1;
		case WM_SIZE:
			w->visible = 1;
			break;
		case WM_SHOWWINDOW:
			w->visible = wParam;
			
			if ( wParam == true )
				event_send( w, "show", "" );
			else
				event_send( w, "hide", "" );
			
			break;
		case WM_VSCROLL:
		case WM_HSCROLL:
			if ( lParam != 0 )
			{
				w = GetWindowLong( lParam, GWL_USERDATA );
			}
			
			if ( !strcmp( w->object.type, "claro.graphics.widgets.scrollbar" ) )
			{
				s = SB_CTL;
				
				tmp = "scroll";
			}
			else if ( uMsg == WM_VSCROLL )
			{
				s = SB_VERT;
				tmp = "vscroll";
			}
			else
			{
				s = SB_HORZ;
				tmp = "hscroll";
			}
			
			si.cbSize = sizeof( si );
			si.fMask  = SIF_ALL;
			GetScrollInfo( w->native, s, &si );
			y = si.nPos;
			
			switch ( LOWORD( wParam ) ) {
				case SB_TOP:
					si.nPos = si.nMin;
					break;
				case SB_BOTTOM:
					si.nPos = si.nMax;
					break;
				case SB_LINEUP:
					si.nPos -= 1;
					break;
				case SB_LINEDOWN:
					si.nPos += 1;
					break;
				case SB_PAGEUP:
					si.nPos -= si.nPage;
					break;
				case SB_PAGEDOWN:
					si.nPos += si.nPage;
					break;
				case SB_THUMBTRACK:
					si.nPos = si.nTrackPos;
					break;
				default:
					break; 
			}
		
			si.fMask = SIF_POS;
			SetScrollInfo( w->native, s, &si, true );
			
			if ( si.nPos != y ) {
				//printf( "%d --> %d (%s,%s)\n",y,si.nPos,w->object.type, tmp );
				event_send( w, tmp, "" );
			}
			
			return 0;
	}
	
	return CallWindowProc( w->naddress[0], hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK cg_msgloop_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w = GetWindowLong( hWnd, GWL_USERDATA );
	widget_t *win;
	PAINTSTRUCT ps;
	HDC hdc, hdcbuf;
	HBITMAP hbmp, nhbmp;
	RECT r;
	
	if ( w == 0 )
		return 1;
	
	switch ( uMsg )
	{
		case WM_SETFOCUS:
			printf( "%s GOT FOCUS!\n", w->object.type );
			
			if ( dialog_stack_check( w ) == 1 )
				return 0;
			
			return 1;
		case WM_KEYUP:
			if ( w->notify_flags & cNotifyKey )
			{
				if ( event_send( OBJECT(w), "key_up", "i", wParam ) == 1 )
				{
					return 0;
				}
			}
			
			break;
		case WM_KEYDOWN:
			win = cgraphics_get_widget_window( w );
			
			if ( w->notify_flags & cNotifyKey )
			{
				if ( event_send( OBJECT(w), "key_down", "i", wParam ) == 1 )
				{
					return 0;
				}
			}
			
			if ( win != 0 && win->flags & cWindowModalDialog )
			{
				if ( wParam == VK_ESCAPE )
				{
					DestroyWindow( win->native );
					return 0;
				}
				else if ( wParam == VK_RETURN )
				{
					event_send( win, "dialog_ok", "" );
					return 0;
				}
			}
			
			return 1;
	}
	
#if 0
	switch ( uMsg )
	{
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT:
			win = cgraphics_get_widget_window( w );
			InvalidateRgn( hWnd, 0, RDW_VALIDATE ); // but say it's clean.. right?
			win->naddress[3] = 1; // dirty :)
			return 0;
		case WM_MOVE:
			widget_set_position( w, LOWORD(lParam), HIWORD(lParam), 0 );
			break;
		case WM_CREATE:
		case WM_SIZE:
			GetClientRect( hWnd, &r );
			if ( w->naddress[2] != 0 )
				DeleteObject( w->naddress[2] );
			hbmp = CreateCompatibleBitmap( hdc, r.right, r.bottom );
			w->naddress[2] = hbmp;
			ReleaseDC( hWnd, hdc );
			InvalidateRgn( hWnd, 0, 0 );
			break;
	}
#endif
#if 0
	switch ( uMsg )
	{
		case WM_SIZE:
			hdc = GetDC( hWnd );
			GetClientRect( hWnd, &r );
			if ( w->naddress[2] != 0 )
				DeleteObject( w->naddress[2] );
			hbmp = CreateCompatibleBitmap( hdc, r.right, r.bottom );
			w->naddress[2] = hbmp;
			ReleaseDC( hWnd, hdc );
			break;
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			
			hdcbuf = CreateCompatibleDC( hdc );
			
			GetClientRect( hWnd, &r );
			hbmp = w->naddress[2];
			
			if ( hbmp == 0 )
				hbmp = CreateCompatibleBitmap( hdc, r.right, r.bottom );
			
			w->naddress[2] = hbmp;
			
			nhbmp = SelectObject( hdcbuf, hbmp );
			
			SendMessage( hWnd, WM_PRINTCLIENT, hdcbuf, PRF_ERASEBKGND|PRF_CHILDREN|PRF_CLIENT );
			BitBlt( hdc, 0, 0, r.right, r.bottom, hdcbuf, 0, 0, SRCCOPY );
			
			SelectObject( hdcbuf, nhbmp );
			
			DeleteObject( hdcbuf );
			
			EndPaint( hWnd, &ps ); 
			
			return 0; /* do NOT paint normally :) */
	}
#endif
	
	return 1; //CallWindowProc( w->naddress[0], hWnd, uMsg, wParam, lParam );
}

extern widget_t *claro_current_dialog;

/* called each loop, use to process events from the platform */
void cgraphics_check_events( )
{
	MSG msg;
	node_t *n, *n2;
	widget_t *w;
	
	while ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
	{
		w = (widget_t *)GetWindowLong( msg.hwnd, GWL_USERDATA );
		
		if ( w != 0 )
		{
			if ( cg_msgloop_proc( msg.hwnd, msg.message, msg.wParam, msg.lParam ) == 0 )
				continue;
			
			if ( !strcmp( w->object.type, "claro.graphics.widgets.window" ) )
			{
				window_widget_t *ww = (window_widget_t *)w;
				
				if ( ww->workspace != 0 && TranslateMDISysAccel( ww->workspace->native, &msg ) != 0 )
					continue;
			}
		}
		
		if ( claro_current_dialog != 0 )
		{
			/* check for dialog messages */
			if ( IsDialogMessage( claro_current_dialog->native, &msg ) != 0 )
				continue;
			
			/* persistantly force the active dialog to the foreground of the app */
			if ( GetActiveWindow( ) != claro_current_dialog->native )
			{
				SetActiveWindow( claro_current_dialog->native );
			}
		}
		
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	/*
	LIST_FOREACH_SAFE( n, n2, object_list.head )
	{
		w = (widget_t *)n->data;
		
		if ( strcmp( w->object.type, "claro.graphics.widgets.window" ) )
			continue;
		
		if ( w->naddress[3] == 1 )
		{
			InvalidateRgn( w->container, 0, 0 );
			w->naddress[3] = 0;
		}
	}
	*/
}

extern list_t object_list;

widget_t *cg_find_by_native( HWND hwnd )
{
	widget_t *tw;
	node_t *n;
	
	if ( hwnd == 0 )
		return 0;
	
	LIST_FOREACH( n, object_list.head )
	{
		tw = n->data;
		
		if ( strncmp( tw->object.type, "claro.graphics.widgets.", 23 ) )
			continue;
		
		if ( tw->native == hwnd || tw->container == hwnd )
		{
			return tw;
		}
	}
	
	return 0;
}

int curr_classname_id = 1;

/* General widgets */

void cgraphics_widget_show( widget_t *widget )
{
	HWND hwnd = widget->native;
	ShowWindow( hwnd, SW_SHOW );
}

void cgraphics_widget_hide( widget_t *widget )
{
	HWND hwnd = widget->native;
	ShowWindow( hwnd, SW_HIDE );
}

void cgraphics_widget_focus( widget_t *widget )
{
	HWND hwnd = widget->native;
	SetFocus( hwnd );
}

void cgraphics_widget_close( widget_t *widget )
{
	HWND hwnd = widget->native;
	SendMessage( hwnd, WM_CLOSE, 0, 0 );
}

void cgraphics_post_init( widget_t *widget )
{
	SendMessage( widget->native, WM_SETFONT, nicerfont, true );
	widget->font.native = nicerfont;
	
	SetWindowLong( widget->native, GWL_USERDATA, widget );
	widget->naddress[0] = SetWindowLong( widget->native, GWL_WNDPROC, cg_intercept_proc );
	
	if ( widget->container != 0 )
	{
		SetWindowLong( widget->container, GWL_USERDATA, widget );
		widget->naddress[1] = SetWindowLong( widget->container, GWL_WNDPROC, cg_intercept_proc );
	}
	
	widget->naddress[2] = 0;
	widget->naddress[3] = 0;
}

void cgraphics_widget_enable( widget_t *widget )
{
	HWND hwnd = widget->native;
	EnableWindow( hwnd, true );
}

void cgraphics_widget_disable( widget_t *widget )
{
	HWND hwnd = widget->native;
	EnableWindow( hwnd, false );
}

/* update bounds (global widget function */

void cgraphics_update_bounds( object_t *obj )
{
	widget_t *widget = (widget_t *)obj;
	
	if ( widget->size_req == 0 )
		return;
	
	if ( !( widget->size_flags & cSizeRequestChanged ) )
		return;
	
	if ( !strcmp( widget->object.type, "claro.graphics.widgets.window" ) ||
		 !strcmp( widget->object.type, "claro.graphics.widgets.workspace.window" ) )
		return;
	
	if ( !strcmp( widget->object.type, "claro.graphics.widgets.workspace" ) )
	{
		widget_t *wp = cgraphics_get_widget_window( WIDGET(obj) );
		SetWindowPos( widget->native, HWND_TOP, widget->size_req->x+wp->size_ct.x, widget->size_req->y+wp->size_ct.y, widget->size_req->w, widget->size_req->h, 0 );
		return;
	}
	
	MoveWindow( widget->native, widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h, 1 );
}

void cgraphics_widget_screen_offset( widget_t *object, int *dx, int *dy )
{
	RECT cr;
	
	GetWindowRect( object->native, &cr );
	
	*dx = cr.left;
	*dy = cr.top;
}

void cgraphics_widget_set_cursor( widget_t *widget, int cursor )
{

}

/* end of platform functions */
