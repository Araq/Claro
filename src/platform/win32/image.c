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

LRESULT CALLBACK cg_win32_image_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	image_widget_t *iw;
	image_t *im;
	widget_t *w;
	PAINTSTRUCT ps;
	HDC hdc, oldbmp;
	
	w = cg_find_by_native( hWnd );
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	iw = w;
	im = iw->src;
	
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
			widget_set_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
			break;
		case WM_PAINT:
			if ( im == NULL )
				break;
			
			hdc = BeginPaint( hWnd, &ps );
			
			/*RECT rect;
			rect.left = rect.top = 0;
			rect.right = rect.bottom = 100;
			FillRect( hdc, &rect, RGB(255,0,0));*/
			
#ifdef IMAGES_USE_LIBPNG_FIXME
			if ( im->native3 == 0 )
			{
				/* Use alphablend, it's a PNG */
				oldbmp = SelectObject( im->native, im->native2 );
				BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
				AlphaBlend( hdc, 0, 0, im->width, im->height, im->native, 0, 0, im->width, im->height, pixelblend );
				SelectObject( im->native, oldbmp );
			}
			else
#endif
			{
				oldbmp = SelectObject( im->native, im->native2 );
				BitBlt( hdc, 0, 0, im->width, im->height, im->native, 0, 0, SRCCOPY );
				SelectObject( im->native, oldbmp );
			}
			
			EndPaint( hWnd, &ps );
			return 0;
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/* Image */
void cgraphics_image_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	
	WNDCLASSEX wc;
	int wflags, wexflags;
	char clname[1024];
	RECT rect;
	
	sprintf( clname, "claro_image_%d", ++curr_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_win32_image_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) GetModuleHandle( NULL );
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground    = NULL;
	
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

	if ( !( hwnd = CreateWindowEx( 0, clname, 
	                               "",
	                               WS_CHILD,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create image HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
	
}

void cgraphics_image_set_image( widget_t *widget )
{
	UpdateWindow( widget->native );
}
