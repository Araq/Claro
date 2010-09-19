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

/* Splitter */

LRESULT CALLBACK cg_splitter_win32_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	widget_t *w;
	splitter_widget_t *sw;
	native_splitter_widget_t *nw;
	bounds_t b;
	int mx, my;
	cell_t *cl;
	int es;
	POINT mouse;
	POINT ptClientUL;
    POINT ptClientLR;
    RECT rcClient;
	
	w = cg_find_by_native( hWnd );
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	sw = (splitter_widget_t *)w;
	nw = (native_splitter_widget_t *)w->ndata;
	
	switch ( uMsg )
	{
		case WM_MOVE:
			widget_set_position( w, LOWORD(lParam), HIWORD(lParam), 1 );
			break;
		case WM_SIZE:
			widget_set_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
			b = w->size;
			b.x = b.y = 0;
			layout_reparse( nw->lt, b, nw->lt->col.min, nw->lt->row.min );
			break;
		case WM_LBUTTONDOWN:
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			nw->ml_x = mx;
			nw->ml_y = my;
			
			SetCapture( hWnd );
			
			/* code here copied from MSDN mouse input example:
			 * http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/windowsuserinterface/userinput/mouseinput/usingmouseinput.asp
			 */
			
			GetClientRect( hWnd, &rcClient );
            ptClientUL.x = rcClient.left;
            ptClientUL.y = rcClient.top;
 
            ptClientLR.x = rcClient.right + 1;
            ptClientLR.y = rcClient.bottom + 1;
            ClientToScreen( hWnd, &ptClientUL );
            ClientToScreen( hWnd, &ptClientLR );
            
            SetRect(&rcClient, ptClientUL.x, ptClientUL.y, ptClientLR.x, ptClientLR.y);
            
            ClipCursor(&rcClient);
            
			break;
		case WM_LBUTTONUP:
			ClipCursor( NULL );
			ReleaseCapture( );
			break;
		case WM_MOUSEMOVE:
			mouse.x = LOWORD(lParam);
			mouse.y = HIWORD(lParam);
			
			mx = mouse.x - nw->ml_x;
			my = mouse.y - nw->ml_y;
			
			if ( wParam & MK_LBUTTON && !(nw->frozen) )
			{
				char *act;
				//printf( "moved: %d,%d\n", mx, my );
				
				// set b to our bounds
				b = *w->size_req;
				b.x = b.y = 0;
				
				if ( sw->children[0].flex )
				{
					act = "widget2";
					my = -my; // flip
					mx = -mx; // flip
				}
				else
					act = "widget1";
				
				if ( w->flags & cSplitterVertical )
				{
					cl = layout_cell_get( nw->lt, act );
					
					es = cl->bounds.h + my;
					
					if ( es < 0 )
						es = 0;
					
					if ( es > b.h - nw->splitter_gap )
						es = b.h - nw->splitter_gap;
					
					layout_cell_set_h( nw->lt, act, es );
				}
				else
				{
					cl = layout_cell_get( nw->lt, act );
					
					es = cl->bounds.w + mx;
					
					if ( es < 0 )
						es = 0;
					
					if ( es > b.w - nw->splitter_gap )
						es = b.w - nw->splitter_gap;
					
					layout_cell_set_w( nw->lt, act, es );
				}
				
				// reparse the layout
				layout_reparse( nw->lt, b, nw->lt->col.min, nw->lt->row.min );
				
				// update all the children
				widget_resized_handle( OBJECT(w), 0 );
				
				// save the mouse position
				nw->ml_x = mouse.x;
				nw->ml_y = mouse.y;
			}
			break;
		case WM_COMMAND:
		case WM_NOTIFY:
			return cg_win32_proc( hWnd, uMsg, wParam, lParam );
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/* update visibility status of child widgets */
event_handler( cgraphics_splitter_child_vis_updated_p )
{
	splitter_widget_t *sw = (splitter_widget_t *)object;
	native_splitter_widget_t *nw = (native_splitter_widget_t *)sw->widget.ndata;
	int va, vb;
	
	if ( nw->splitter_num != 2 )
		return;
	
	va = vb = 1;
	
	if ( nw->widgets[0] )
		va = nw->widgets[0]->visible;
	if ( nw->widgets[1] )
		vb = nw->widgets[1]->visible;
	
	if ( va && vb )
	{
		// both visible
		nw->frozen = 0;
		
		if ( sw->widget.flags & cSplitterVertical )
		{
			if ( sw->children[0].flex )
				layout_cell_flex_h( nw->lt, "widget1" );
			else
				layout_cell_set_h( nw->lt, "widget1", sw->children[0].size );
			
			if ( sw->children[1].flex )
				layout_cell_flex_h( nw->lt, "widget2" );
			else
				layout_cell_set_h( nw->lt, "widget2", sw->children[1].size );
				
			layout_cell_set_h( nw->lt, "gap", nw->splitter_gap );
		}
		else
		{
			if ( sw->children[0].flex )
				layout_cell_flex_w( nw->lt, "widget1" );
			else
				layout_cell_set_w( nw->lt, "widget1", sw->children[0].size );
			
			if ( sw->children[1].flex )
				layout_cell_flex_w( nw->lt, "widget2" );
			else
				layout_cell_set_w( nw->lt, "widget2", sw->children[1].size );
			
			layout_cell_set_w( nw->lt, "gap", nw->splitter_gap );
		}
	}
	else
	{
		// one hidden
		nw->frozen = 1;
		
		if ( sw->widget.flags & cSplitterVertical )
		{
			layout_cell_set_h( nw->lt, (va?"widget2":"widget1"), 0 );
			layout_cell_flex_h( nw->lt, (va?"widget1":"widget2") );
			layout_cell_set_h( nw->lt, "gap", 0 );
		}
		else
		{
			layout_cell_set_w( nw->lt, (va?"widget2":"widget1"), 0 );
			layout_cell_flex_w( nw->lt, (va?"widget1":"widget2") );
			layout_cell_set_w( nw->lt, "gap", 0 );
		}
	}
	
	layout_reparse( nw->lt, *sw->widget.size_req, nw->lt->col.min, nw->lt->row.min );
	
	// update all the children
	widget_resized_handle( OBJECT(sw), 0 );
}

event_handler( cgraphics_splitter_child_vis_updated )
{
	cgraphics_splitter_child_vis_updated_p( object->parent, event );
}

void cgraphics_splitter_child_create_handler( object_t *obj, event_t *event )
{
	object_t *child = (object_t *)event_get_arg_ptr( event, 0 );
	widget_t *cw = (widget_t *)child;
	splitter_widget_t *sw = (splitter_widget_t *)obj;
	native_splitter_widget_t *nw = (native_splitter_widget_t *)sw->widget.ndata;
	char ltw[16];
	
	// make sure we don't get more than 2 children!
	if ( nw->splitter_num == 2 )
		return;
	
	// free size_req if it was used
	if ( cw->size_req->owner == 0 )
		free( cw->size_req->owner );
	
	nw->widgets[nw->splitter_num] = child;
	sw->children[nw->splitter_num].w = child;
	
	nw->splitter_num++;
	sprintf( ltw, "widget%d", nw->splitter_num );
	
	// give the widget a layout's bound data for size_req
	cw->size_req = lt_bounds( nw->lt, ltw );
	
	object_addhandler( child, "hide", cgraphics_splitter_child_vis_updated );
	object_addhandler( child, "show", cgraphics_splitter_child_vis_updated );
	
	// tell the widget it's got new coords
	event_send( child, "update", "" );
	
	cgraphics_splitter_child_vis_updated_p( sw, 0 );
}

void cgraphics_splitter_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	splitter_widget_t *sw = (splitter_widget_t *)widget;
	native_splitter_widget_t *nw = (native_splitter_widget_t *)sw->widget.ndata;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	WNDCLASSEX wc;
	char clname[1024];
	char fmt[64];
	bounds_t b;
	
	widget->ndata = malloc( sizeof( native_splitter_widget_t ) );
	nw = widget->ndata;
	
	sprintf( clname, "claro_window_%d", ++curr_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_splitter_win32_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) GetModuleHandle( NULL );
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	if ( sw->widget.flags & cSplitterVertical )
		wc.hCursor			= LoadCursor(NULL, IDC_SIZENS);
	else
		wc.hCursor			= LoadCursor(NULL, IDC_SIZEWE);
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
		MessageBox( 0, "Could not create canvas HWND.", "Claro error", 0 );
	
	b = *widget->size_req;
	
	b.x = b.y = 0;
	nw->splitter_gap = 5;
	
	sprintf( fmt, "[_widget1|gap(%d)|widget2]", nw->splitter_gap );
	
	if ( sw->widget.flags & cSplitterVertical )
		sprintf( fmt, "[_widget1][{%d}gap][_widget2]", nw->splitter_gap );
	
	nw->lt = layout_create(NULL, fmt, b, 10, 10 );
	nw->splitter_num = 0;
	nw->frozen = 0;
	
	object_addhandler( OBJECT(widget), "child_create", cgraphics_splitter_child_create_handler );
	
	widget->native = hwnd;
	widget->container = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}

void cgraphics_splitter_updated( widget_t *widget, int child )
{
	splitter_widget_t *sw = (splitter_widget_t *)widget;
	cgraphics_splitter_child_vis_updated_p( widget, 0 );
}
