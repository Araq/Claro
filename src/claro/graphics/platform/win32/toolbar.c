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

/* Toolbar */

#ifndef RBS_AUTOSIZE
#define RBS_AUTOSIZE 0x2000
#endif

#define MAX_TB_ICONS 4096

list_t tblist;
int tblist_cmdid = 0;

void cgraphics_toolbar_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = parent->native; // toolbar is NOT inside the container widget :)
	HWND rebar = parent->ndata;
	REBARBANDINFO ri;
	DWORD dwBtnSize;
	
	//CCS_ADJUSTABLE | 
	if ( !( hwnd = CreateWindowEx( TBSTYLE_EX_DRAWDDARROWS, TOOLBARCLASSNAME, (LPSTR) NULL,
				WS_VISIBLE | WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, rebar,
				(HMENU)0, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create button HWND.", "Claro error", 0 );
	
	SendMessage( hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0 ); 
	
	SendMessage( hwnd, TB_AUTOSIZE, 0, 0 );
	
	widget->native = hwnd;
	
	ShowWindow( hwnd, SW_SHOW );
	
	// Get the height of the toolbar.
	dwBtnSize = SendMessage( hwnd, TB_GETBUTTONSIZE, 0, 0 );
	
	ZeroMemory( &ri, sizeof(ri) );
	ri.cbSize = sizeof(REBARBANDINFO);
	ri.fMask  = RBBIM_SIZE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_ID | 
				RBBIM_STYLE | RBBIM_TEXT | RBBIM_BACKGROUND;
	ri.fStyle = RBBS_CHILDEDGE | RBBS_FIXEDBMP | RBS_AUTOSIZE;
	
	ri.lpText     = "Claro Toolbar";
	ri.hwndChild  = hwnd;
	ri.cxMinChild = 0;
	ri.cyMinChild = HIWORD(dwBtnSize);
	ri.cx         = 10;

	SendMessage( rebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&ri );
	
	UpdateWindow( hwnd );
}

void cgraphics_toolbar_new_icon( widget_t *widget, list_item_t *item )
{
	widget_t *parent = (widget_t *)OBJECT(widget)->parent;
	HWND rebar = parent->ndata;
	REBARBANDINFO ri;
	TBADDBITMAP tbb;
	TBBUTTON tbut;
	DWORD dwBtnSize;
	node_t *n;
	int a;
	image_t *img = item->data[0];
	const char *txt = item->data[1];
	
	if ( img == 0 && txt == 0 )
	{
		/* regardless of style, this is a separator :) */
		tbut.iBitmap = 10;
		tbut.fsStyle = BTNS_SEP;
		tbut.iString = 0; /* JIC */
	}
	else
	{
		/* it has SOMETHING filled in, so let's tell Windows */
		
		tbut.fsStyle = BTNS_BUTTON;
		tbb.hInst = NULL;
		tbb.nID = 0;
		
		if ( widget->flags & cToolbarAutoSizeButtons )
			tbut.fsStyle |= BTNS_AUTOSIZE;
		
		if ( LIST_LENGTH( &item->children ) > 0 )
			tbut.fsStyle |= BTNS_DROPDOWN;
		
		if ( widget->flags & cToolbarShowImages )
		{
			tbb.hInst = NULL;
#ifdef IMAGES_USE_LIBPNG
			tbb.nID = 0;
			tbut.iBitmap = 0;
#else
			tbb.nID = (UINT_PTR)img->native2; // BMP
			
			a = SendMessage( widget->native, TB_ADDBITMAP, 1, (LPARAM)&tbb );
			
			tbut.iBitmap = a;
#endif
		}
		
		if ( widget->flags & cToolbarShowText )
		{
			tbut.iString = txt;
			tbut.fsStyle |= BTNS_SHOWTEXT;
		}
		else
			tbut.iString = 0;
	}
	
	/* if none so far, just for safely clean the list :) */
	if ( tblist_cmdid == 0 ) {
		tblist_cmdid++;
		list_create( &tblist );
	}
	
	/* that's our cmdid! prepare the next one, too.. */
	item->nativeid = tblist_cmdid;
	tblist_cmdid++;
	
	/* save into cmdid mapping list */
	n = node_create( );
	node_add( item, n, &tblist );
	
	tbut.idCommand = item->nativeid;
	tbut.fsState = TBSTATE_ENABLED;
	tbut.dwData = 0;
	
	SendMessage( widget->native, TB_INSERTBUTTON, item->row, (LPARAM)&tbut );
	
	/* Fix the rebar :) */
	
	SendMessage( widget->native, TB_AUTOSIZE, 0, 0 );
	
	dwBtnSize = SendMessage( widget->native, TB_GETBUTTONSIZE, 0, 0 );
	
	ZeroMemory( &ri, sizeof(ri) );
	ri.cbSize = sizeof(REBARBANDINFO);
	ri.fMask  = RBBIM_CHILDSIZE;
	
	ri.cxMinChild = 0;
	ri.cyMinChild = HIWORD(dwBtnSize);
	ri.cx         = 10;

	SendMessage( rebar, RB_SETBANDINFO, (WPARAM)0, (LPARAM)&ri );
}

void cgraphics_toolbar_remove_icon( widget_t *widget, list_item_t *item )
{
	
}

void cgraphics_toolbar_new_popup_item( widget_t *widget, list_item_t *item )
{
	
}

void cgraphics_toolbar_remove_popup_item( widget_t *widget, list_item_t *item )
{
	
}

void cgraphics_toolbar_set_menu( widget_t *widget, list_item_t *item )
{
	
}
