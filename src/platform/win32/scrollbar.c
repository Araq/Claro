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

void cgraphics_scrollbar_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(OBJECT(parent));
	SCROLLINFO si;
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 21;
	
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 21;
	
	if ( !( hwnd = CreateWindowEx( 0, "SCROLLBAR", 
	                               "",
	                               WS_CHILD | WS_TABSTOP | (widget->flags&cScrollbarVertical?SBS_VERT:SBS_HORZ),
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create scrollbar HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	
	ZeroMemory( &si, sizeof(si) );
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0;
	si.nMax   = 0;
	si.nPage  = 10;
	si.nPos   = 0;
	SetScrollInfo( hwnd, SB_CTL, &si, true );
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

int cgraphics_scrollbar_get_sys_width( )
{
	return GetSystemMetrics( SM_CXVSCROLL );
}

void cgraphics_scrollbar_set_range( widget_t *w )
{
	scrollbar_widget_t *sb = (scrollbar_widget_t *)w;
	SCROLLINFO si;
	
	ZeroMemory( &si, sizeof(si) );
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE;
	si.nMin   = sb->min;
	si.nMax   = sb->max;
	si.nPage  = sb->pagesize;
	
	if ( si.nMin == si.nMax || (si.nMax - si.nMin) < si.nPage )
	{
		//si.nMax = 1;
		si.fMask |= SIF_DISABLENOSCROLL;
		EnableScrollBar( w->native, SB_CTL, ESB_DISABLE_BOTH );
	}
	else
	{
		EnableScrollBar( w->native, SB_CTL, ESB_ENABLE_BOTH );
	}
	
	SetScrollInfo( w->native, SB_CTL, &si, true );
}

void cgraphics_scrollbar_set_pos( widget_t *w, int pos )
{
	SCROLLINFO si;
	
	ZeroMemory( &si, sizeof(si) );
	si.cbSize = sizeof(si);
	si.fMask  = SIF_POS;
	si.nPos   = pos;
	
	SetScrollInfo( w->native, SB_CTL, &si, true );
	
	event_send( OBJECT(w), "scroll", "" );
}

int cgraphics_scrollbar_get_pos( widget_t *w )
{
	SCROLLINFO si;
	
	ZeroMemory( &si, sizeof(si) );
	si.cbSize = sizeof(si);
	si.fMask  = SIF_POS;
	
	GetScrollInfo( w->native, SB_CTL, &si );
	
	return si.nPos;
}
