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

/* Canvas */

extern int cg_isxplater;

LRESULT CALLBACK cg_canvas_win32_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w;
	canvas_widget_t *cvsw;
	native_canvas_widget_t *nw;
	RECT r;
	PAINTSTRUCT ps;
	HDC hdc, mdc, odc;
	HBITMAP bmp, obmp;
	
	w = cg_find_by_native( hWnd );
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	cvsw = (canvas_widget_t *)w;
	nw = (native_canvas_widget_t *)w->ndata;
	
	switch ( uMsg )
	{
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT:
		case WM_PRINTCLIENT:
			if ( uMsg == WM_PAINT )
			{
				if ( !cg_isxplater )
				{
					odc = BeginPaint( hWnd, &ps );
					mdc = CreateCompatibleDC( odc );
					
					GetClientRect( hWnd, &r );
					
					if ( nw->bmp == 0 )
						nw->bmp = CreateCompatibleBitmap( odc, r.right, r.bottom );
					
					obmp = SelectObject( mdc, nw->bmp );
					
					hdc = mdc;
				}
				else
				{
					hdc = BeginPaint( hWnd, &ps );
				}
			}
			else
				hdc = wParam;
            
            GetClientRect( hWnd, &r );
            FillRect( hdc, &r, (HBRUSH) (COLOR_WINDOW) );
            
            cvsw->surfdata = hdc;
            
            if ( w->font.native != 0 )
				SelectObject( hdc, w->font.native );
            
#ifndef NO_CAIRO
			cairo_surface_destroy( cvsw->surface );
			cvsw->surface = 0;
			
			if ( cvsw->surface == 0 )
			{
				cvsw->surface = cairo_win32_surface_create( hdc );
			}
			
			cvsw->cr = cairo_create( cvsw->surface );
#endif
			/*
			cairo_rectangle( cvsw->cr, r.left, r.top, r.right - r.left, r.bottom - r.top );
			cairo_clip( cvsw->cr );
			*/
			event_send( OBJECT(w), "redraw", "p", cvsw->cr );

#ifndef NO_CAIRO
			cairo_destroy( cvsw->cr );
#endif
			
			if ( uMsg == WM_PAINT && !cg_isxplater )
			{
				BitBlt( odc, 0, 0, r.right, r.bottom, hdc, 0, 0, SRCCOPY );
				SelectObject( mdc, obmp );
				EndPaint( hWnd, &ps );
			} else if ( uMsg == WM_PAINT )
			{
				EndPaint( hWnd, &ps );
			}
			
			//cairo_surface_flush( cvsw->surface );
			
			break;
		
		case WM_SIZE:
			if ( !cg_isxplater )
			{
				if ( nw->bmp != 0 )
					DeleteObject( nw->bmp );
				hdc = GetDC( hWnd );
				GetClientRect( hWnd, &r );
				nw->bmp = CreateCompatibleBitmap( hdc, r.right, r.bottom );
				ReleaseDC( hWnd, hdc );
			}
			InvalidateRgn( hWnd, 0, 0 );
			break;
		
		case WM_LBUTTONDOWN:
			event_send( OBJECT(w), "clicked", "ii", LOWORD(lParam), HIWORD(lParam) );
			break;
		
		case WM_LBUTTONUP:
			event_send( OBJECT(w), "released", "ii", LOWORD(lParam), HIWORD(lParam) );
			break;
		
		case WM_RBUTTONDOWN:
			event_send( OBJECT(w), "right_clicked", "ii", LOWORD(lParam), HIWORD(lParam) );
			break;
		
		case WM_RBUTTONUP:
			event_send( OBJECT(w), "right_released", "ii", LOWORD(lParam), HIWORD(lParam) );
			break;
		
		case WM_MOUSEMOVE:
			if ( wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON) || w->notify_flags & cNotifyMouse )
			{
				event_send( OBJECT(w), "mouse_moved", "ii", LOWORD(lParam), HIWORD(lParam) );
				
				// will this work fine here? hmm.
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof( TRACKMOUSEEVENT );
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				tme.dwHoverTime = 0;
				TrackMouseEvent( &tme );
			}
			break;
		
		case WM_MOUSELEAVE:
			event_send( OBJECT(w), "mouse_leave", "" );
			break;
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

event_handler( cgraphics_canvas_cleanup )
{
	widget_t *w = (widget_t *)object;
	free( w->ndata );
}

void cgraphics_canvas_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	canvas_widget_t *cvsw = (canvas_widget_t *)widget;
	native_canvas_widget_t *nw = 0;//(native_canvas_widget_t *)&cvsw->ndata;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	WNDCLASSEX wc;
	char clname[1024];
	
	sprintf( clname, "claro_window_%d", ++curr_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_canvas_win32_proc;
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
	
#ifndef NO_CAIRO
	cvsw->surface = 0;
#endif
	
	if ( !( hwnd = CreateWindowEx( 0, clname, 
									0,
									WS_CHILD,
									widget->size_req->x, widget->size_req->y,
									widget->size_req->w, widget->size_req->h,
									hwnd_parent,
									NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create canvas HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	widget->ndata = (native_canvas_widget_t *)malloc(sizeof(native_canvas_widget_t));
	nw = widget->ndata;
	
	object_addhandler( widget, "destroy", cgraphics_canvas_cleanup );
	
	nw->bmp = 0;
	nw->font = 0;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_canvas_redraw( canvas_widget_t *w )
{
	//InvalidateRect( w->widget.native, NULL, 1 );
	RedrawWindow( w->widget.native, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW );
}

/* claro native text functions */

void cgraphics_canvas_set_text_color( widget_t *widget, double r, double g, double b, double a )
{
	native_canvas_widget_t *nw = (native_canvas_widget_t *)widget->ndata;
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	
	nw->fg = RGB(r*255,g*255,b*255);
	SetTextColor( cw->surfdata, RGB(r*255,g*255,b*255) );
}

void cgraphics_canvas_set_text_bgcolor( widget_t *widget, double r, double g, double b, double a )
{
	native_canvas_widget_t *nw = (native_canvas_widget_t *)widget->ndata;
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	
	nw->bg = RGB(r*255,g*255,b*255);
	SetBkColor( cw->surfdata, RGB(r*255,g*255,b*255) );
}

void cgraphics_canvas_set_text_font( widget_t *widget, char *face, int size, int weight, int slant, int decoration )
{
	native_canvas_widget_t *nw = (native_canvas_widget_t *)widget->ndata;
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	HFONT f, o;
	int w = FW_NORMAL;
	
	if ( weight == CTEXT_WEIGHT_BOLD )
		w = FW_BOLD;
	
	f = CreateFont( size, 0, 0, 0, w, (slant==CTEXT_SLANT_ITALIC), (decoration&CTEXT_EXTRA_UNDERLINE), 0, DEFAULT_CHARSET, 
							OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
							ANTIALIASED_QUALITY | CLEARTYPE_QUALITY, 
							DEFAULT_PITCH | FF_DONTCARE, face );
	
	if ( nw->font != 0 )
		DeleteObject( nw->font );
	
	nw->font = f;
	
	/*
	o = SelectObject( cw->surfdata, f );
	
	if ( o && o != widget->font.native )
		DeleteObject( o );
	*/
}

void cgraphics_canvas_selectfont( canvas_widget_t *canvas )
{
	native_canvas_widget_t *nw = (native_canvas_widget_t *)canvas->widget.ndata;
	
	if ( nw->font == 0 )
		SelectObject( canvas->surfdata, canvas->widget.font.native );
	else
		SelectObject( canvas->surfdata, nw->font );
}

int cgraphics_canvas_text_width( widget_t *widget, const char *text, int len )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	SIZE sz;
	int chars;
	
	cgraphics_canvas_selectfont( cw );
	
	//GetTextExtentPoint32( cw->surfdata, text, len, &sz );
	GetTextExtentExPoint( cw->surfdata, text, len, 0, NULL, NULL, &sz );
	
	return sz.cx;
}

int cgraphics_canvas_text_box_width( widget_t *widget, const char *text, int len )
{
	return cgraphics_canvas_text_width( widget, text, len );
}

int cgraphics_canvas_text_display_count( widget_t *widget, const char *text, int width )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	SIZE sz;
	int chars;
	
	cgraphics_canvas_selectfont( cw );
	
	GetTextExtentExPoint( cw->surfdata, text, strlen( text ), width, &chars, NULL, &sz );
	
	return chars;
}

void cgraphics_canvas_show_text( widget_t *widget, int x, int y, const char *text, int len )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;

	cgraphics_canvas_selectfont( cw );
	
	TextOut( cw->surfdata, x, y, text, len );
}

void cgraphics_canvas_fill_rect( widget_t *widget, int x, int y, int w, int h, double r, double g, double b, double a )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	RECT rr;
	HBRUSH bc;
	
	rr.left = x;
	rr.top = y;
	rr.right = x+w;
	rr.bottom = y+h;
	
	bc = CreateSolidBrush( RGB(r*255,g*255,b*255) );
	
	FillRect( cw->surfdata, &rr, bc );
	
	DeleteObject( bc );
}

void cgraphics_canvas_draw_image( widget_t *widget, image_t *image, int x, int y )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	HDC hdc = cw->surfdata;
	HBITMAP oldbmp;
	
#ifdef IMAGES_USE_LIBPNG
	if ( image->native3 == 0 )
	{
		/* Use alphablend, it's a PNG */
		oldbmp = SelectObject( image->native, image->native2 );
		BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		AlphaBlend( hdc, x, y, image->width, image->height, image->native, 0, 0, image->width, image->height, pixelblend );
		SelectObject( image->native, oldbmp );
	}
	else
#endif
	{
		oldbmp = SelectObject( image->native, image->native2 );
		BitBlt( hdc, x, y, image->width, image->height, image->native, 0, 0, SRCCOPY );
		SelectObject( image->native, oldbmp );
	}
}


