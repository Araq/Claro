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
#include <initguid.h>
#include "IImgCtx.h"

/* Win32 header file issue */
#ifndef LPNMLVDISPINFO
#define LPNMLVDISPINFO LPNMLVDISPINFOA
#endif

#ifndef CDRF_NOTIFYSUBITEMDRAW
#define CDRF_NOTIFYSUBITEMDRAW 0x00000020
#endif

#ifndef WM_NCMOUSELEAVE
#define WM_NCMOUSELEAVE 0x02A2
#endif

BYTE ANDmaskIcon[] = {
	0xFF, 0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF, 0xFF,
};
BYTE XORmaskIcon[] = {
	0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
};
HICON cnull_icon = 0;

widget_t *cg_find_by_native( HWND hwnd );

extern list_t tblist;
extern list_t mblist;

list_t claro_dialog_list;
widget_t *claro_current_dialog=0;

void cgraphics_window_adjust_size( window_widget_t *wp )
{
	RECT r, rr;

	GetWindowRect( wp->widget.native, &rr );
	
	r.left = rr.left;
	r.top = rr.top;
	r.right = r.left + wp->widget.size_ct.w;
	r.bottom = r.top + wp->widget.size_ct.h;
	
	if ( wp->exsp_tools >= 0 )
	{
		r.bottom += wp->exsp_tools;
		r.bottom += wp->exsp_status;
	}
	
	AdjustWindowRect( &r, GetWindowLong( wp->widget.native, GWL_STYLE ), wp->menubar != 0 );
	MoveWindow( wp->widget.native, wp->widget.size.x, wp->widget.size.y, r.right-r.left, r.bottom-r.top, true );
}

void dialog_stack_push( widget_t *w )
{
	node_t *n;
	widget_t *p;
	
	if ( claro_current_dialog == 0 )
	{
		list_create( &claro_dialog_list );
	}
	
	n = node_create( );
	node_add( w, n, &claro_dialog_list );
	
	claro_current_dialog = w;
	
	p = w->object.parent;
	
	if ( p != 0 )
	{
		EnableWindow( p->native, false );
	}
}

void dialog_stack_pop( widget_t *w )
{
	node_t *n, *nn;
	
	if ( claro_current_dialog != w )
		return;
	
	claro_current_dialog = 0;
	
	LIST_FOREACH_SAFE( n, nn, claro_dialog_list.head )
	{
		if ( nn == 0 )
		{
			/* last node! we're the gonner. */
			
			node_del( n, &claro_dialog_list );
			node_free( n );
			
			break;
		}
		
		/* current dialog could be us, because we're not the last */
		claro_current_dialog = n->data;
	}
	
	if ( claro_current_dialog == 0 )
	{
		widget_t *p;
		
		// that was the last one :)
		p = w->object.parent;
		
		if ( p != 0 )
			EnableWindow( p->native, true );
	}
}

int dialog_stack_check( widget_t *r )
{
	if ( !strcmp( r->object.type, "claro.graphics.widgets.window" ) && 
		  claro_current_dialog != 0 && claro_current_dialog != r )
	{
		SetFocus( claro_current_dialog->native );
	}
}

#if 0
typedef struct
{
	HDC hdc;
	HWND parent;
} CGW32ENUMPARM;

BOOL CALLBACK cg_win32_enum_proc( HWND hwnd, CGW32ENUMPARM *inp )
{
	widget_t *w = (widget_t *)GetWindowLong( hwnd, GWL_USERDATA );
	HDC hdcbuf;
	HBITMAP hbm, hbmold;
	RECT r, rs, rp;
	CGW32ENUMPARM ep;
	HDC hdc = inp->hdc;
	HWND parent = inp->parent;
	
	if ( w == 0 )
		return 1;
	
	ep.hdc = hdc;
	ep.parent = hwnd;
	
	GetClientRect( hwnd, &r );
	
	hdcbuf = CreateCompatibleDC( hdc );
	hbm = CreateCompatibleBitmap( hdc, r.right, r.bottom );
	
	//RedrawWindow( hwnd, NULL, NULL, 0 );
	
	//hbm = w->naddress[2];
	
	hbmold = (HBITMAP)SelectObject( hdcbuf, hbm );
	
	SendMessage( hwnd, WM_PRINTCLIENT, (WPARAM)hdcbuf, (LPARAM)PRF_CHILDREN|PRF_CLIENT|PRF_CHILDREN );
	//UpdateWindow( hwnd );
	
	GetWindowRect( hwnd, &rs );
	GetWindowRect( parent, &rp );
	
	BitBlt( hdc, rs.left - rp.left, rs.top - rp.top, r.right, r.bottom, hdcbuf, 0, 0, SRCCOPY );
	
	SelectObject( hdcbuf, hbmold );
	
	DeleteObject( hdcbuf );
	DeleteObject( hbm );
	
	EnumChildWindows( hwnd, cg_win32_enum_proc, inp );
	
	return 1;
}
#endif
LRESULT CALLBACK cg_win32_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	RECT clientr, statusr, toolr;
	LPNMHDR hdr;
	widget_t *w, *rw;
	window_widget_t *windoww;
	RECT contr;
	//PAINTSTRUCT ps;
	//HDC hdc, hdcbuf;
	//HBITMAP hbm, hbmold;
	//RECT r;
	//CGW32ENUMPARM ep;
	LPMEASUREITEMSTRUCT measure;
	DRAWITEMSTRUCT *draws;
	
	w = cg_find_by_native( hWnd );
	rw = w;
	windoww = (window_widget_t *)w;
	
	if ( w == 0 )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
	if ( hWnd == w->container )
	{
		switch ( uMsg )
		{
#if 0
			case WM_ERASEBKGND:
				return 1;
			case WM_PAINT:
				GetClientRect( hWnd, &r );
				
				hdcbuf = CreateCompatibleDC( NULL );
				hbm = CreateCompatibleBitmap( NULL, r.right, r.bottom );
				
				hbmold = (HBITMAP)SelectObject( hdcbuf, hbm );
				
				BitBlt( hdcbuf, 0, 0, r.right, r.bottom, NULL, 0, 0, WHITENESS );
				/*ep.hdc = hdcbuf;
				ep.parent = hWnd;
				EnumChildWindows( hWnd, cg_win32_enum_proc, &ep );*/
				
				SendMessage( hWnd, WM_PRINTCLIENT, hdcbuf, PRF_CHILDREN|PRF_CLIENT|PRF_NONCLIENT );
				hdc = BeginPaint( hWnd, &ps );
				BitBlt( hdc, 0, 0, r.right, r.bottom, hdcbuf, 0, 0, SRCCOPY );
				EndPaint( hWnd, &ps );
				SelectObject( hdcbuf, hbmold );
				
				DeleteObject( hbm );
				DeleteObject( hdcbuf );
				
				break;
#endif
			case WM_MOVE:
				widget_set_content_position( w, LOWORD(lParam), HIWORD(lParam), 1 );
			case WM_SIZE:
				//InvalidateRgn( hWnd, 0, 0 );
				widget_set_content_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
				
				break;
			case WM_CTLCOLORSTATIC:
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORDLG:
				SetBkMode( (HDC)wParam, TRANSPARENT );
				return (LRESULT)((HBRUSH)COLOR_APPWORKSPACE + 1);
			case WM_NOTIFY:
				hdr = (LPNMHDR)lParam;
				
				switch ( hdr->code )
				{
					LPNMTTDISPINFO ttdi;
					LPNMLVDISPINFO lvdi;
					LPNMTVDISPINFO tvdi;
					LVITEM *litem;
					TVITEM *titem;
					list_item_t *li;
					list_widget_t *lw;
					
					LPNMCUSTOMDRAW cd;
					LPNMTVCUSTOMDRAW tcd;
					
					case TVN_SELCHANGED:
						{
							LPNMTREEVIEW nmtv = (LPNMTREEVIEW)lParam;
							list_item_t *item = (list_item_t *)nmtv->itemNew.lParam;
							treeview_widget_t *tw = (treeview_widget_t *)cg_find_by_native( hdr->hwndFrom );
							
							tw->selected = item;
							
							event_send( OBJECT(tw), "selected", "p", item );
						}
						break;
					case NM_RCLICK:
						w = cg_find_by_native( hdr->hwndFrom );
						
						if ( !strcmp( OBJECT(w)->type, "claro.graphics.widgets.treeview" ) )
						{
							TVHITTESTINFO hti;
							TVITEM titem;
							DWORD cpos = GetMessagePos( );
							HTREEITEM i;
							
							hti.pt.x = LOWORD( cpos );
							hti.pt.y = HIWORD( cpos );
							
							ScreenToClient( w->native, &hti.pt );
							
							i = TreeView_HitTest( w->native, &hti );
							
							titem.hItem = i;
							titem.mask = TVIF_PARAM;
							
							if ( i != 0 )
							{
								list_item_t *item;
								
								TreeView_GetItem( w->native, &titem );
								
								item = (list_item_t *)titem.lParam;
								
								event_send( OBJECT(w), "right_clicked", "pii", item, hti.pt.x, hti.pt.y );
							}
						}
						break;
					
					case TTN_GETDISPINFO:
						ttdi = (LPNMTTDISPINFO)lParam;
						
						ttdi->lpszText = (LPTSTR)"Bleh.";
						break;
					case NM_CLICK:
						w = cg_find_by_native( hdr->hwndFrom );
						
						if ( !strcmp( OBJECT(w)->type, "claro.graphics.widgets.listview" ) )
						{
							int a = SendMessage( w->native, LVM_GETHOTITEM, 0, 0 );
							list_item_t *li = list_widget_get_row( w, 0, a );
							listview_widget_t *lvw = (listview_widget_t *)w;
							
							lvw->selected = li;
							
							event_send( OBJECT(w), "selected", "p", li );
						}
						break;
					/*case TVN_GETDISPINFO:
						w = cg_find_by_native( hdr->hwndFrom );
						
						if ( w == NULL )
							break;
						
						tvdi = (LPNMTVDISPINFO)lParam;
						titem = (TVITEM *)&(tvdi->item);
						
						if ( titem->mask & TVIF_IMAGE )
						{
							titem->iImage = -1;
						}
							
						break;*/
					case LVN_GETDISPINFO:
						w = cg_find_by_native( hdr->hwndFrom );
						
						if ( w == NULL )
							break;
						
						lvdi = (LPNMLVDISPINFO)lParam;
						litem = (LVITEM *)&(lvdi->item);
						
						li = list_widget_get_row( w, 0, litem->iItem );
						
						if ( li )
						{
							HWND *hwnds;
							
							hwnds = li->native;
							
							litem->mask = LVIF_TEXT;
							litem->pszText = "";
							
							lw = (list_widget_t *)w;
							
							/* FIXME: this should use LISTVIEW_TYPEs instead */
							if ( lw->coltypes[litem->iSubItem] == CLIST_TYPE_STRING )
							{
								/* text */
								litem->pszText = li->data[litem->iSubItem];
							}
							else if ( lw->coltypes[litem->iSubItem] == CLIST_TYPE_UINT )
							{
								/* checkbox */
								litem->pszText = "";
								
								if ( hwnds[litem->iSubItem] == 0 )
								{
									hwnds[litem->iSubItem] = CreateWindowEx( WS_EX_TRANSPARENT, "BUTTON", "",
										WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX,
										0, 0, 0, 0,
										w->native,
										NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL );
									SendMessage( hwnds[litem->iSubItem], BM_SETCHECK, (*((int *)li->data[litem->iSubItem])?BST_CHECKED:0), 0 );
									SetWindowLong( hwnds[litem->iSubItem], GWL_USERDATA, li );
									ShowWindow( hwnds[litem->iSubItem], SW_SHOW );
									UpdateWindow( hwnds[litem->iSubItem] );
								}
							}
							else if ( lw->coltypes[litem->iSubItem] == CLIST_TYPE_DOUBLE )
							{
								/* progress bar */
								litem->pszText = "";
								
								if ( hwnds[litem->iSubItem] == 0 )
								{
									double **v = li->data;
									double rv = *(v[litem->iSubItem]);
									int intpos = (int)((float)rv * 65535.0);
									
									hwnds[litem->iSubItem] = CreateWindowEx(0, PROGRESS_CLASS, "",
										WS_CHILD,
										0, 0, 0, 0,
										w->native,
										NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL );
									
									SendMessage( hwnds[litem->iSubItem], PBM_SETRANGE, 0, MAKELPARAM( 0, 65535 ) );
									SendMessage( hwnds[litem->iSubItem], PBM_SETPOS, intpos, 0 );
									
									SetWindowLong( hwnds[litem->iSubItem], GWL_USERDATA, li );
									
									ShowWindow( hwnds[litem->iSubItem], SW_SHOW );
									UpdateWindow( hwnds[litem->iSubItem] );
								}
							}
						}
						
						break;
					case NM_CUSTOMDRAW:
						w = cg_find_by_native( hdr->hwndFrom );
						
						if ( w == NULL )
							break;
						
						if ( !strcmp( OBJECT(w)->type, "claro.graphics.widgets.treeview" ) )
						{
							tcd = (LPNMTVCUSTOMDRAW)hdr;
							cd = (LPNMCUSTOMDRAW)hdr;
							
							switch ( cd->dwDrawStage )
							{
								case CDDS_PREPAINT:
									return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYSUBITEMDRAW;
								case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
									return CDRF_SKIPDEFAULT;
								case CDDS_ITEMPREPAINT:
								{
									list_item_t *litem = (list_item_t *)cd->lItemlParam;
									color_t *tcol, *bcol;
									
									if ( cd->uItemState == (CDIS_FOCUS | CDIS_SELECTED) )
									{
										// selected
										
										tcol = &litem->sel_text_color;
										bcol = &litem->sel_back_color;
									}
									else
									{
										tcol = &litem->text_color;
										bcol = &litem->back_color;
									}
									
									if ( tcol->used )
										tcd->clrText = RGB( (tcol->r*255.0f), (tcol->g*255.0f), (tcol->b*255.0f) );
									
									if ( bcol->used )
										tcd->clrTextBk = RGB( (bcol->r*255.0f), (bcol->g*255.0f), (bcol->b*255.0f) );
									
									if ( litem->font.used == 2 )
									{
										TVITEM tvi;
										
										memset( &tvi, 0, sizeof(TVITEM) );
										
										tvi.mask = TVIF_STATE;
										
										tvi.hItem = litem->native;
										
										tvi.stateMask = TVIS_BOLD | TVIS_CUT;
										tvi.state = (litem->font.weight==cFontWeightBold?TVIS_BOLD:0);
										
										SendMessage( w->native, TVM_SETITEM, 0, (LPARAM)(LPTVITEM)&tvi );
										
										litem->font.used = 1;
										/*
										HFONT f = (HFONT)cgraphics_font_from_font( &litem->font );
										
										SelectObject( cd->hdc, f );
										
										if ( f != litem->font.native && litem->font.native != 0 )
											DeleteObject( litem->font.native );
										
										litem->font.used = 1;
										litem->font.native = f;
										*/
									}
									
									return CDRF_NEWFONT | CDRF_NOTIFYSUBITEMDRAW;
								}
							}
							
							return CDRF_DODEFAULT;
						}
						else if ( !strcmp( OBJECT(w)->type, "claro.graphics.widgets.listview" ) )
						{
							cd = (LPNMCUSTOMDRAW)hdr;
							
							switch ( cd->dwDrawStage )
							{
								int i;
								LPNMLVCUSTOMDRAW lvcd;
								HWND *hwnds;
								
								case CDDS_PREPAINT:
									return CDRF_NOTIFYITEMDRAW;
								case CDDS_ITEMPREPAINT:
									return CDRF_NOTIFYSUBITEMDRAW;
								case CDDS_SUBITEM | CDDS_ITEMPREPAINT: 
									return CDRF_NOTIFYPOSTPAINT;
								case CDDS_SUBITEM | CDDS_ITEMPOSTPAINT: 
									lvcd = (LPNMLVCUSTOMDRAW)cd;
									
									i = lvcd->iSubItem;
									lw = (list_widget_t *)w;
									
									li = list_widget_get_row( w, 0, cd->dwItemSpec );
									
									if ( li == 0 )
										return CDRF_DODEFAULT;
									
									hwnds = li->native;
									
									//printf( "Woof draw sub %d->%d\n", cd->dwItemSpec, i );
									
									/* FIXME: this should use LISTVIEW_TYPEs instead */
									if ( lw->coltypes[i] == CLIST_TYPE_UINT )
									{
										HWND hwndi = hwnds[i];
										RECT r;
										
										if ( hwndi )
										{
											r.left = LVIR_BOUNDS;
											SendMessage( w->native, LVM_GETITEMRECT, cd->dwItemSpec, &r );
											MoveWindow( hwndi, (cd->rc.left+r.left), r.top, (cd->rc.right+r.left)-(cd->rc.left+r.left), r.bottom-r.top, true );
											InvalidateRect( hwndi, NULL, true );
										}
										
										return CDRF_DODEFAULT;
									}
									else if ( lw->coltypes[i] == CLIST_TYPE_DOUBLE )
									{
										HWND hwndi = hwnds[i];
										RECT r;
										
										if ( hwndi )
										{
											r.left = LVIR_BOUNDS;
											SendMessage( w->native, LVM_GETITEMRECT, cd->dwItemSpec, &r );
											MoveWindow( hwndi, (cd->rc.left+r.left), r.top, (cd->rc.right+r.left)-(cd->rc.left+r.left), r.bottom-r.top, true );
											InvalidateRect( hwndi, NULL, true );
										}
										
										return CDRF_DODEFAULT;
									}
									
									return CDRF_NOTIFYSUBITEMDRAW;
								default:
									return CDRF_DODEFAULT;
							}
							
							return CDRF_DODEFAULT;
						}
						
						break;
				}
				
				break;
			case WM_COMMAND:
				w = cg_find_by_native( (HWND)lParam );
				
				switch ( HIWORD( wParam ) )
				{
					case BN_CLICKED:
						if ( !strcmp( w->object.type, "claro.graphics.widgets.button" )  )
							event_send( OBJECT(w), "pushed", "" );
						else if ( !strcmp( w->object.type, "claro.graphics.widgets.radiobutton" )  )
						{
							/* Yes. That's right. I didn't like the way Windows wanted
							 *  to do everything it's own way, so we'll do radio buttons
							 *  by HAND.. *winge, winge, winge*
							 */
							
							radiobutton_widget_t *rb = (radiobutton_widget_t *)w;
							radiogroup_t *rg = rb->group;
							node_t *n;
							widget_t *cw;
							
							LIST_FOREACH( n, rg->buttons.head )
							{
								cw = (widget_t *)n->data;
								
								if ( cw != w )
									SendMessage( cw->native, BM_SETCHECK, BST_UNCHECKED, 0 );
							}
							
							SendMessage( w->native, BM_SETCHECK, BST_CHECKED, 0 );
							
							rg->selected = OBJECT(w);
							
							event_send( OBJECT(w), "selected", "" );
							event_send( OBJECT(rg), "changed", "p", w );
						}
						else if ( !strcmp( w->object.type, "claro.graphics.widgets.checkbox" )  )
						{
							checkbox_widget_t *cb = (checkbox_widget_t *)w;
							
							cb->checked = ( SendMessage( w->native, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
							
							event_send( OBJECT(w), "changed", "i", cb->checked );
						}
						break;
					case EN_CHANGE:
						if ( !strcmp( w->object.type, "claro.graphics.widgets.textbox" ) )
						{
							textbox_widget_t *tbw = (textbox_widget_t *)w;
							SendMessage( w->native, WM_GETTEXT, CLARO_TEXTBOX_MAXIMUM-1, (LPARAM)tbw->text );
							event_send( OBJECT(w), "changed", "" );
						}
						else if ( !strcmp( w->object.type, "claro.graphics.widgets.textarea" ) )
						{
							textarea_widget_t *tbw = (textarea_widget_t *)w;
							SendMessage( w->native, WM_GETTEXT, CLARO_TEXTAREA_MAXIMUM-1, (LPARAM)tbw->text );
							event_send( OBJECT(w), "changed", "" );
						}
						break;
					case LBN_SELCHANGE: /* also CBN_SELCHANGE, they are aparently the same */
						if ( !strcmp( w->object.type, "claro.graphics.widgets.listbox" ) )
						{
							listbox_widget_t *lw = (listbox_widget_t *)w;
							list_item_t *i;
							int a;
							
							a = SendMessage( w->native, LB_GETCURSEL, 0, 0 );
							i = (list_item_t *)SendMessage( w->native, LB_GETITEMDATA, a, 0);
							
							lw->selected = i;
							
							event_send( OBJECT(w), "selected", "p", i );
						}
						else if ( !strcmp( w->object.type, "claro.graphics.widgets.combo" ) )
						{
							combo_widget_t *lw = (combo_widget_t *)w;
							list_item_t *i;
							int a;
							
							a = SendMessage( w->native, CB_GETCURSEL, 0, 0 );
							i = (list_item_t *)SendMessage( w->native, CB_GETITEMDATA, a, 0);
							
							lw->selected = i;
							
							event_send( OBJECT(w), "selected", "p", i );
						}
						
						break;
					case LBN_DBLCLK:
						if ( !strcmp( w->object.type, "claro.graphics.widgets.listbox" ) )
						{
							event_send( OBJECT(w), "double_clicked", "" );
						}
						
						break;
				}
				
				break;
		}
		
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	RECT r;
	switch ( uMsg )
	{
		case WM_QUIT:
			return 1;
		case WM_CLOSE:
			event_send( OBJECT(w), "closing", "" );
			
			dialog_stack_pop( w );
			
			if ( !strcmp( w->object.type, "claro.graphics.widgets.workspace.window" ) )
			{
				SendMessage( WIDGET(w->object.parent)->native, WM_MDIDESTROY, (WPARAM)hWnd, 0 );
			}
			
			DestroyWindow( hWnd );
			
			return 0;
		case WM_MOVE:
			GetWindowRect( w->native, &r );
			widget_set_position( w, r.left, r.top, 1 );
			//widget_set_position( w, LOWORD(lParam), HIWORD(lParam), 1 );
			break;
		case WM_MEASUREITEM:
			measure = (LPMEASUREITEMSTRUCT)lParam;
			
			if ( measure->CtlType == ODT_MENU )
			{
				measure->itemWidth = 16;
				measure->itemHeight = 16;
				return 1;
			}
			
			return 0;
		case WM_DRAWITEM:
			if ( wParam == 0 )
			{
				draws = (DRAWITEMSTRUCT *)lParam;
				if ( draws->CtlType == ODT_MENU )
				{
					list_item_t *li = NULL;
					list_item_t *tli;
					node_t *n;
					
					LIST_FOREACH( n, mblist.head )
					{
						tli = (list_item_t *)n->data;
						
						if ( tli->nativeid == draws->itemID )
						{
							li = tli;
							break;
						}
					}
					
					if ( li != 0 && li->data[0] != 0 )
					{
						image_t *im = li->data[0];
						HBITMAP oldbmp;
						
#ifdef IMAGES_USE_LIBPNG
						if ( im->native3 == 0 )
						{
							/* Use alphablend, it's a PNG */
							oldbmp = SelectObject( im->native, im->native2 );
							BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
							AlphaBlend( draws->hDC, 8, draws->rcItem.top, im->width, im->height, im->native, 0, 0, im->width, im->height, pixelblend );
							SelectObject( im->native, oldbmp );
						}
						else
#endif
						{
							oldbmp = SelectObject( im->native, im->native2 );
							BitBlt( draws->hDC, 8, draws->rcItem.top, im->width, im->height, im->native, 0, 0, SRCCOPY );
							SelectObject( im->native, oldbmp );
						}
					}
				}
			}
			
			break;
		case WM_NOTIFY:
			hdr = (LPNMHDR)lParam;
			
			switch ( hdr->code )
			{
				LPNMTBCUSTOMDRAW tbcd;
				LPNMCUSTOMDRAW cd;
				
				case NM_CUSTOMDRAW:
					w = cg_find_by_native( hdr->hwndFrom );
					
					if ( w == NULL )
						break;
					
					if ( !strcmp( OBJECT(w)->type, "claro.graphics.widgets.toolbar" ) )
					{
						tbcd = (LPNMTBCUSTOMDRAW)hdr;
						cd = (LPNMCUSTOMDRAW)hdr;

						switch ( cd->dwDrawStage )
						{
							//IImgCtx *m_pImgCtx;
							//RECT imgrect;
							image_t *img;
							list_item_t *item;
							list_item_t *tli;
							node_t *n;
							
							case CDDS_PREPAINT:
								return CDRF_NOTIFYITEMDRAW;
							case CDDS_ITEMPREPAINT:
								return CDRF_NOTIFYPOSTPAINT;
							case CDDS_ITEMPOSTPAINT:
								item = NULL;
								
								LIST_FOREACH( n, tblist.head )
								{
									tli = (list_item_t *)n->data;
									
									if ( tli->nativeid == cd->dwItemSpec )
									{
										item = tli;
										break;
									}
								}
								
								img = NULL;
								
								if ( item != 0 )
									img = item->data[0];
								
								if ( img != 0 )
								{
									HBITMAP oldbmp;
									int x, y;
									
									x = cd->rc.left + ( (cd->rc.right-cd->rc.left) / 2 ) - ( img->width / 2 );
									y = cd->rc.top + 4;
									
#ifdef IMAGES_USE_LIBPNG
									if ( img->native3 == 0 )
									{
										/* Use alphablend, it's a PNG */
										oldbmp = SelectObject( img->native, img->native2 );
										BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
										AlphaBlend( cd->hdc, x, y, img->width, img->height, img->native, 0, 0, img->width, img->height, pixelblend );
										SelectObject( img->native, oldbmp );
									}
									else
#endif
									{
										oldbmp = SelectObject( img->native, img->native2 );
										BitBlt( cd->hdc, x, y, img->width, img->height, img->native, 0, 0, SRCCOPY );
										SelectObject( img->native, oldbmp );
									}
								}
								
								return CDRF_SKIPDEFAULT;
						}
						
						//return CDRF_NOTIFYITEMDRAW;
					}
					
					break;
			}
			
			break;
		case WM_SHOWWINDOW:
			if ( wParam == true )
			{
				/* hack, makes sure windows get a WM_SIZE event to start :/ */
				GetWindowRect( w->native, &clientr );
				SetWindowPos( w->native, 0, 0, 0, clientr.right - clientr.left, clientr.bottom - clientr.top + 1, SWP_NOMOVE | SWP_NOZORDER );
				SetWindowPos( w->native, 0, 0, 0, clientr.right - clientr.left, clientr.bottom - clientr.top, SWP_NOMOVE | SWP_NOZORDER );
			}
			/*
			if ( wParam == true )
				event_send( w, "show", "" );
			else
				event_send( w, "hide", "" );*/
			
			break;
		case WM_SIZE:
			//InvalidateRgn( hWnd, 0, 0 );
			
			widget_set_size( w, LOWORD(lParam), HIWORD(lParam), 1 );
			
			if ( wParam == SIZE_MAXIMIZED )
				event_send( OBJECT(w), "maximized", "" );
			else if ( wParam == SIZE_MINIMIZED )
				event_send( OBJECT(w), "minimized", "" );
			else if ( wParam == SIZE_RESTORED )
				event_send( OBJECT(w), "restored", "" );	
			
			GetClientRect( w->native, &clientr );
			
			contr.left = contr.top = 0;
			contr.right = clientr.right - clientr.left;
			contr.bottom = clientr.bottom - clientr.top;
			int tool_size = 0;
			
			if ( !strcmp( rw->object.type, "claro.graphics.widgets.workspace.window" ) )
			{
				MoveWindow( w->container, contr.left, contr.top, contr.right, contr.bottom, 1 );
				break;
			}
			
			memset( &toolr, 0, sizeof( RECT ) );
			if ( w->ndata != 0 )
			{
				//SendMessage( w->ndata, WM_SIZE, 0, 0 );
				MoveWindow( w->ndata, clientr.left, clientr.top, clientr.right - clientr.left, clientr.bottom - clientr.top, 1 );
				
				if ( GetClientRect( (HWND)w->ndata, &toolr ) == 0 )
				{
					printf( "ERROR GETTING WINDOW RECT FOR REBAR %p: %d!\n", w->ndata, (int)GetLastError( ) );
					exit( 0 );
				}
				
				tool_size += toolr.bottom - toolr.top;
				
				contr.top += toolr.bottom - toolr.top;
				contr.bottom -= toolr.bottom - toolr.top;
			}
			
			node_t *n;
			object_t *o;
			int status_size = 0;
			
			LIST_FOREACH( n, OBJECT(w)->children.head )
			{
				o = (object_t *)n->data;
				
				if ( !strcmp( o->type, "claro.graphics.widgets.statusbar" ) )
				{
					SendMessage( WIDGET(o)->native, WM_SIZE, 0, 0 );
					
					GetWindowRect( WIDGET(o)->native, &statusr );
					
					status_size += statusr.bottom - statusr.top;
					contr.bottom -= statusr.bottom - statusr.top;
					
					break;
				}
			}
			
			//MoveWindow( w->container, contr.left, contr.top, contr.right, contr.bottom, 1 );
			
			if ( !strcmp( w->object.type, "claro.graphics.widgets.window" ) )
			{
				window_widget_t *win = (window_widget_t *)w;
				if ( status_size != win->exsp_status || tool_size != win->exsp_tools )
				{
					win->exsp_status = status_size;
					win->exsp_tools = tool_size;
					
					cgraphics_window_adjust_size( win );
					win->exsp_init = 1;
				}
				else if ( win->exsp_init == 1 )
				{
					MoveWindow( w->container, contr.left, contr.top, contr.right, contr.bottom, 1 );
				}
			}
			else
			{
				MoveWindow( w->container, contr.left, contr.top, contr.right, contr.bottom, 1 );
			}
			
			if ( !strcmp( rw->object.type, "claro.graphics.widgets.window" ) && windoww->workspace != 0 )
				SetWindowPos( windoww->workspace->native, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
			
			return 0;
		case WM_NCMOUSELEAVE:
		case WM_EXITMENULOOP:
			DrawMenuBar( hWnd );
			break;
		case WM_COMMAND:
			w = cg_find_by_native( (HWND)lParam );
			
			if ( w == 0 )
			{
				w = windoww->menubar;
			}
			
			switch ( HIWORD( wParam ) )
			{
				case BN_CLICKED:
					if ( w != NULL && !strcmp( w->object.type, "claro.graphics.widgets.toolbar" )  )
					{
						list_item_t *li = NULL;
						list_item_t *tli;
						node_t *n;
						
						LIST_FOREACH( n, tblist.head )
						{
							tli = (list_item_t *)n->data;
							
							if ( tli->nativeid == LOWORD( wParam ) )
							{
								li = tli;
								break;
							}
						}
						
						if ( li != NULL )
							event_send( OBJECT(li), "pushed", "" );
					}
					else if ( w == NULL ||
						      !strcmp( w->object.type, "claro.graphics.widgets.menubar" ) ||
						      !strcmp( w->object.type, "claro.graphics.widgets.menu" ) )
					{
						list_item_t *li = NULL;
						list_item_t *tli;
						node_t *n;
						
						LIST_FOREACH( n, mblist.head )
						{
							tli = (list_item_t *)n->data;
							
							if ( tli->nativeid == LOWORD( wParam ) )
							{
								li = tli;
								break;
							}
						}
						
						if ( li != NULL )
							event_send( OBJECT(li), "pushed", "" );
						
						DrawMenuBar( hWnd );
					}
					break;
				default:
					if ( !strcmp( rw->object.type, "claro.graphics.widgets.window" ) && windoww->workspace != 0 )
						return DefFrameProc( hWnd, windoww->workspace->native, uMsg, wParam, lParam );
			}
			
			break;
	}
	
	if ( !strcmp( rw->object.type, "claro.graphics.widgets.workspace.window" ) )
		return DefMDIChildProc( hWnd, uMsg, wParam, lParam );
	else if ( !strcmp( rw->object.type, "claro.graphics.widgets.window" ) && windoww->workspace != 0 )
		return DefFrameProc( hWnd, (HWND)windoww->workspace->native, uMsg, wParam, lParam );
	else
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/* called when a window widget needs creating */
void cgraphics_window_widget_create( widget_t *widget )
{
	WNDCLASSEX wc;
	HWND hwnd, rhwnd, wparent;
	int wflags, wexflags;
	char clname[1024];
	REBARINFO rbi;
	RECT rect;
	window_widget_t *wwin = (window_widget_t *)widget;
	
	wwin->exsp_status = -1;
	wwin->exsp_tools = -1;
	
	sprintf( clname, "claro_window_%d", ++curr_classname_id );
	
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= cg_win32_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) GetModuleHandle( NULL );
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE + 1;
	
	/* FIXME: fix this when we have MDI
	if ( flags & C_MDIPARENT )
		wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE + 1;
	else
		wc.hbrBackground = (HBRUSH)CreateSolidBrush( RGB( (C_NATIVE_BACKGROUND >> 24) & 0xFF, (C_NATIVE_BACKGROUND >> 16) & 0xFF, (C_NATIVE_BACKGROUND >> 8) & 0xFF ) );*/
	
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= clname;
	
	if ( cnull_icon == 0 )
	{
		cnull_icon = CreateIcon( (HINSTANCE) GetModuleHandle( NULL ),
             16, 16, 1, 1, ANDmaskIcon, XORmaskIcon );
	}
	
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );
	
	if ( !RegisterClassEx( &wc ) )
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	
	wflags = WS_OVERLAPPEDWINDOW;// | WS_CLIPCHILDREN;
	wexflags = 0;//WS_EX_COMPOSITED; // DAMMIT, this doesn't work with listview :{
	
	wparent = 0;
	
	int rx, ry;
	
	rx = widget->size_req->x;
	ry = widget->size_req->y;
	
	if ( rx < 0 )
		rx = CW_USEDEFAULT;
	
	if ( ry < 0 )
		ry = CW_USEDEFAULT;
	
	if ( widget->flags & cWindowNoResizing )
		wflags &= ~WS_THICKFRAME;
	
	if ( widget->flags & cWindowModalDialog && widget->object.parent != 0 && !strcmp( widget->object.parent->type, "claro.graphics.widgets.window" ) )
	{
		wflags = WS_POPUPWINDOW | WS_DLGFRAME | WS_OVERLAPPED | WS_CLIPSIBLINGS | DS_MODALFRAME;
		wexflags = WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CONTROLPARENT;
		
		dialog_stack_push( widget );
		
		// create a dummy invisible parent window for our window, hiding us from the taskbar.
		wparent = CreateWindowEx( 0, clname, "", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL );
	}
	
	rect.left = widget->size_req->x;
	rect.top = widget->size_req->y;
	rect.right = widget->size_req->x + widget->size_req->w;
	rect.bottom = widget->size_req->y + widget->size_req->h;
	
	AdjustWindowRectEx( &rect, wflags, 0, wexflags );
	
	if ( widget->flags & cWindowCenterParent )
	{
		// center the window in the center of the parent (Claro) window.
		int px, py, pw, ph, mw, mh;
		object_t *parent = widget->object.parent;
		widget_t *wp = (widget_t *)parent;
		
		if ( parent != 0 && !strcmp( parent->type, "claro.graphics.widgets.window" ) )
		{
			px = wp->size.x;
			py = wp->size.y;
			pw = wp->size.w;
			ph = wp->size.h;
		}
		else
		{
			// FIXME: screen 
			px = 0;
			py = 0;
			RECT sr;
			GetClientRect( GetDesktopWindow( ), &sr );
			pw = sr.right;
			ph = sr.bottom;
		}
		
		mw = widget->size.w;
		mh = widget->size.h;
		
		rx = px + ( pw / 2 ) - ( mw / 2 );
		ry = py + ( ph / 2 ) - ( mh / 2 );
	}
	/*
	widget->size.x = rx;
	widget->size.y = ry;
	*/
	if ( widget->size_req )
	{
		widget->size_req->x = rx;
		widget->size_req->y = ry;
	}
	
	hwnd = CreateWindowEx(
			wexflags,
			clname,
			"Default Window - Claro",
			wflags,
			rx, ry, 
			rect.right - rect.left, rect.bottom - rect.top,
			wparent, NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL
		);
	
	if ( !( rhwnd = CreateWindowEx( WS_EX_CONTROLPARENT, clname, 
	                               "",
	                               WS_CHILD | WS_CLIPSIBLINGS,
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
							RBS_BANDBORDERS | CCS_NODIVIDER | CCS_NOPARENTALIGN | RBS_FIXEDORDER,
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

void cgraphics_window_show( widget_t *w )
{
	HWND hwnd = w->native;
	ShowWindow( hwnd, SW_SHOW );
}

void cgraphics_window_hide( widget_t *w )
{
	HWND hwnd = w->native;
	ShowWindow( hwnd, SW_HIDE );
}

void cgraphics_window_focus( widget_t *w )
{
	HWND hwnd = w->native;
	SetFocus( hwnd );
}

void cgraphics_window_update_title( widget_t *w )
{
	window_widget_t *ww = (window_widget_t *)w;
	HWND hwnd = w->native;
	SetWindowText( hwnd, ww->title );
}

void cgraphics_window_minimise( widget_t *w )
{
	HWND hwnd = w->native;
	ShowWindow( hwnd, SW_SHOWMINIMIZED );
}

void cgraphics_window_maximise( widget_t *w )
{
	HWND hwnd = w->native;
	ShowWindow( hwnd, SW_SHOWMAXIMIZED );
}

void cgraphics_window_restore( widget_t *w )
{
	HWND hwnd = w->native;
	ShowWindow( hwnd, SW_SHOW );
}

void cgraphics_window_update_icon( widget_t *w )
{
	window_widget_t *ww = (window_widget_t *)w;
	
	if ( ww->icon != 0 )
	{
		SetClassLong( w->native, GCL_HICON, (long)ww->icon->icon );
		SetClassLong( w->native, GCL_HICONSM, (long)ww->icon->icon );
	}
}
