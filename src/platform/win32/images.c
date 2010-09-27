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
#include <windows.h>

#ifdef IMAGES_USE_LIBPNG

#include "platform/win32/pngdib.h"

void cgraphics_png_image_load( image_t *img, const char *file )
{
	PNGDIB *pngdib;
	HDC hdc;
	HBITMAP bmp, obmp;
	LPBITMAPINFOHEADER lpdib;
	BITMAPINFO bmi;
	int lpdib_size, lpdib_bits_offs;
	void *lpbits, *lpbits2;
	char buf[1000];
	
	pngdib = pngdib_p2d_init( );
	if ( !pngdib ) return;

	pngdib_set_dibalpha32( pngdib, 1 );
	pngdib_p2d_set_png_filename( pngdib, file );
	pngdib_p2d_set_gamma_correction( pngdib, 1, PNGDIB_DEFAULT_SCREEN_GAMMA );
	
	if ( pngdib_p2d_run( pngdib ) ) {
		pngdib_done( pngdib );
		return;
	}
	
	pngdib_p2d_get_dib( pngdib, &lpdib, &lpdib_size );
	pngdib_p2d_get_dibbits( pngdib, &lpbits, &lpdib_bits_offs, NULL );

	pngdib_done( pngdib );
	
	img->width = lpdib->biWidth;
	img->height = lpdib->biHeight;
	img->native = lpdib;
	img->native2 = lpbits;
	
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	memcpy( &bmi, lpdib, sizeof(BITMAPINFOHEADER) );
	
	bmi.bmiHeader.biCompression = BI_RGB;
	
	hdc = CreateCompatibleDC( NULL );
	bmp = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, &lpbits2, NULL, 0x0 );
	
	obmp = SelectObject( hdc, bmp );
	memcpy( lpbits2, lpbits, bmi.bmiHeader.biSizeImage );
	
	SelectObject( hdc, obmp );
	
	img->native = hdc;
	img->native2 = bmp;
	img->native3 = 0;
	
	HBITMAP hMonoBitmap = CreateBitmap( img->width, img->height, 1, 1, NULL );
	
	ICONINFO ii;
    ii.fIcon = true;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = hMonoBitmap;
	ii.hbmColor = bmp;
	
	img->icon = CreateIconIndirect( &ii );

	return;
}

#endif

#include <initguid.h>
#include "IImgCtx.h"
#include <wchar.h>

DEFINE_GUID(IID_IImgCtx, 0x3050f3d7, 0x98b5, 0x11cf, 0xbb, 0x82, 0x00, 0xaa, 0x00, 0xbd, 0xce, 0x0b);
DEFINE_GUID(CLSID_IImgCtx, 0x3050f3d6, 0x98b5, 0x11cf, 0xbb, 0x82, 0x00, 0xaa, 0x00, 0xbd, 0xce, 0x0b);

/* Heavily based off http://www.codeproject.com/win32/win32image.asp */

void ErrorDescription(HRESULT hr) {
	char* szErrMsg;
	if(FACILITY_WINDOWS == HRESULT_FACILITY(hr))
		hr = HRESULT_CODE(hr);
	if(FormatMessage(
	  FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, 
	  NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
	  (LPTSTR)&szErrMsg, 0, NULL) != 0) {
		MessageBox( NULL, szErrMsg, "ErrorDescription", MB_OK|MB_ICONINFORMATION );
		LocalFree(szErrMsg);
	} else
		MessageBox( NULL, "Could not find a description for error", "ErrorDescription", MB_OK|MB_ICONINFORMATION );
}

void cgraphics_image_load( image_t *img, const char *file )
{
	IImgCtx *m_pImgCtx;
	HPALETTE m_hPal;
	SIZE sizeImage;
	ULONG ulState;
	HRESULT hr;
	HBITMAP bmp, oldbmp;
	RECT imgrect;
	HDC sdc, dc;
	wchar_t fullname[512];
	wchar_t widedir[512];
	wchar_t widefile[512];
	char dir[512];
	char *thedir = &dir;
	char *thefile = file;
	char *tmp;
	
#ifdef IMAGES_USE_LIBPNG
	/* are we a PNG? if so, send to the PNG loader instead */
	tmp = file + strlen( file ) - 4;
	if ( tmp >= file && !strcasecmp( tmp, ".png" ) )
	{
		cgraphics_png_image_load( img, file );
		return;
	}
#endif
	
	m_pImgCtx = NULL;
	hr = CoCreateInstance( &CLSID_IImgCtx, NULL, CLSCTX_INPROC_SERVER, &IID_IImgCtx, (LPVOID*)&m_pImgCtx );
	
	if ( FAILED(hr) ) {
		ErrorDescription( hr );
		PostQuitMessage( 0 );
	}
	
	getcwd( dir, 512 );
	mbsrtowcs( &widedir, &thedir, 512, NULL );
	mbsrtowcs( &widefile, &thefile, 512, NULL );
	swprintf( fullname, L"file://%s/%s", widedir, widefile );
	
	hr = m_pImgCtx->lpVtbl->Load( m_pImgCtx, fullname, 0 );
	
	if ( FAILED(hr) ) {
		ErrorDescription( hr );
		PostQuitMessage( 0 );
	}
	
	do {
		m_pImgCtx->lpVtbl->GetStateInfo( m_pImgCtx, &ulState, &sizeImage, 0 );
		
		// This image should load/decode instantly, but everyone deserves some sleep
		Sleep(5);
	} while( (ulState & IMGLOAD_LOADING) );
	
	m_pImgCtx->lpVtbl->GetPalette( m_pImgCtx, &m_hPal );
	
	/*** Save to HBITMAP/HDC ***/
	
	// get screen DC
	sdc = GetDC( NULL );
	
	// create a device context compatible with the screen
	dc = CreateCompatibleDC( sdc );
	
	// create a bitmap compatible with the screen
	bmp = CreateCompatibleBitmap( sdc, sizeImage.cx, sizeImage.cy );

	// release the DC
	ReleaseDC( NULL, sdc );
	
	// select the new bitmap
	oldbmp = SelectObject( dc, bmp );
	
	// paint the image to the DC
	SelectPalette( dc, m_hPal, 1 );
	RealizePalette( dc );
	imgrect.left = imgrect.top = 0;
	imgrect.bottom = sizeImage.cy;
	imgrect.right = sizeImage.cx;
	m_pImgCtx->lpVtbl->Draw( m_pImgCtx, dc, &imgrect );
	
	SelectObject( dc, oldbmp );
	
	img->width = sizeImage.cx;
	img->height = sizeImage.cy;
	img->native = dc;
	img->native2 = bmp;
	img->native3 = m_pImgCtx;
	
	//m_pImgCtx->lpVtbl->Release( m_pImgCtx );
}

void cgraphics_image_load_inline_png( image_t *img, const unsigned char * data, int len )
{
	// FIXME
}


