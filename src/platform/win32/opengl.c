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

#ifdef CLARO_OPENGL

#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

LRESULT CALLBACK cg_opengl_win32_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w;

	w = cg_find_by_native( hWnd );
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	switch ( uMsg )
	{
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT:
		case WM_PRINTCLIENT:
			break;
		
		case WM_SIZE:
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

void cgraphics_opengl_setup( opengl_widget_t *w )
{
	// size viewport
	glViewport( w->widget.size.x, w->widget.size.y, w->widget.size.w, w->widget.size.h );
	
	// set matrix mode to projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	
	// perspective
	gluPerspective( 50.0f, (GLfloat)w->widget.size.w / (GLfloat)w->widget.size.h, 0.1f, 1000.0f ); 
	
	// modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	
	// clear depth
	glClearDepth( 1.0f );
	
	// enable some GL features
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glEnable( GL_TEXTURE_2D );
	
	glEnable( GL_ALPHA_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glAlphaFunc( GL_GEQUAL, 0.01f );
	
	//glShadeModel( GL_SMOOTH );
	
	glDisable( GL_CULL_FACE );
	/*glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );*/
	
	// smoothing
	//glEnable( GL_POLYGON_SMOOTH );
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
	
	// create fake texture as #1
	GLuint textmp;
	glGenTextures( 1, &textmp );
}

void cgraphics_opengl_flip( widget_t *widget )
{
	SwapBuffers( widget->ndata );
}

void cgraphics_opengl_activate( widget_t *widget )
{
	opengl_widget_t *ogl = (opengl_widget_t *)widget;
	wglMakeCurrent( widget->ndata, ogl->gldata );
}

event_handler( handle_resize )
{
	cgraphics_opengl_setup( (opengl_widget_t *)object );
}

void cgraphics_opengl_widget_create( widget_t *widget )
{
	opengl_widget_t *ogl = (opengl_widget_t *)widget;
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	
	WNDCLASSEX wc;
	static int curr_gl_classname_id = 0;
	char clname[1024];
	GLuint pixelformat;
	
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof( PIXELFORMATDESCRIPTOR ),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		0, // -1
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,
		0, //
		0, //
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};
	
	/* FIXME: allow setting of these? */
	pfd.cColorBits = 32;//colourbits;
	pfd.cDepthBits = 32;//depthbits;
	pfd.cStencilBits = 8;//stencilbits;
	
	sprintf( clname, "claro_gl_%d", ++curr_gl_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_opengl_win32_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) GetModuleHandle( NULL );
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)CreateSolidBrush( RGB( 0, 0, 0 ) );
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= clname;
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );
	

	if ( !RegisterClassEx( &wc ) )
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	
	if ( !( hwnd = CreateWindowEx( ( widget->flags & cWidgetNoBorder ? 0 : WS_EX_CLIENTEDGE ), clname, 
	                               "OpenGL",
	                               WS_CHILD | WS_VISIBLE,
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create opengl HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	
	if( !( widget->ndata = GetDC( hwnd ) ) )
	{
		MessageBox( 0, "Can't create a GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;
	}
	
	pixelformat = ChoosePixelFormat( widget->ndata, &pfd );

	if( !( pixelformat = ChoosePixelFormat( widget->ndata, &pfd ) ) )
	{
		MessageBox( 0, "Can't find a suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;
	}

	if( !( SetPixelFormat( widget->ndata, pixelformat, &pfd ) ) )
	{
		MessageBox( 0, "Can't set the PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;
	}
	
	if( !( ogl->gldata = wglCreateContext( widget->ndata ) ) )
	{
		MessageBox( 0, "Can't create a GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;
	}
	
	if( !( wglMakeCurrent( widget->ndata, ogl->gldata ) ) )
	{
		MessageBox( 0, "Can't activate the GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;
	}
	
	ShowWindow( hwnd, SW_SHOW );
	SetForegroundWindow( hwnd );
	SetFocus( hwnd );
	
	cgraphics_opengl_setup( widget );
	object_addhandler( widget, "resized", handle_resize );
}

#endif
