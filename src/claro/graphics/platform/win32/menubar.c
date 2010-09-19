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

/* Menubar */

list_t mblist;
int mblist_cmdid = 0;

void cgraphics_window_adjust_size( window_widget_t *wp );

void cgraphics_menubar_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	window_widget_t *wp = (window_widget_t *)parent;
	HWND hwnd_parent = parent->native; // menubar is NOT inside the container widget :)
	HMENU menu;
	
	menu = CreateMenu( );
	
	widget->native = menu;
	wp->menubar = widget;
	
	cgraphics_window_adjust_size( wp );
	
	SetMenu( hwnd_parent, menu );
	
	/* incase workspace has already been created */
	if ( wp->workspace != 0 )
		SetMenu( wp->workspace->native, menu );
	
	DrawMenuBar( hwnd_parent );
}

HMENU cgraphics_menubar_get_native_parent( widget_t *widget, list_item_t *item )
{
	MENUITEMINFO mii;
	HMENU ppmenu;
	list_item_t *pitem = 0;
	
	mii.cbSize = sizeof( MENUITEMINFO );
	
	if ( strcmp( item->object.parent->type, "claro.graphics.list_item" ) ) // parent isn't an item
		return widget->native;
	else
	{
		pitem = item->object.parent;
		
		if ( pitem->native == 0 )
		{
			ppmenu = cgraphics_menubar_get_native_parent( widget, pitem );
			
			pitem->native = CreatePopupMenu( );
			mii.fMask = MIIM_SUBMENU;
			mii.hSubMenu = pitem->native;
			SetMenuItemInfo( ppmenu, pitem->row, true, &mii );
		}
		
		return pitem->native;
	}
}

void cgraphics_menubar_new_item( widget_t *widget, list_item_t *item )
{
	HMENU parent;
	MENUITEMINFO mii;
	node_t *n;
	
	mii.cbSize = sizeof( MENUITEMINFO );
	
	parent = cgraphics_menubar_get_native_parent( widget, item );
	
	/* if none so far, just for safely clean the list :) */
	if ( mblist_cmdid == 0 ) {
		mblist_cmdid++;
		list_create( &mblist );
	}
	
	/* that's our cmdid! prepare the next one, too.. */
	item->nativeid = mblist_cmdid;
	mblist_cmdid++;
	
	/* save into cmdid mapping list */
	n = node_create( );
	node_add( item, n, &mblist );
	
	if ( item->data[1] == 0 )
	{
		mii.fMask = MIIM_FTYPE;
		mii.fType = MFT_SEPARATOR;
		InsertMenu( parent, item->row, MF_BYPOSITION, item->nativeid, "" );
		SetMenuItemInfo( parent, item->row, true, &mii );
	}
	else
	{
		InsertMenu( parent, item->row, MF_STRING | MF_BYPOSITION, item->nativeid, item->data[1] );
		
		if ( item->data[0] != 0 )
		{
			// image
			mii.fMask = MIIM_BITMAP;
			mii.hbmpItem = HBMMENU_CALLBACK;
			SetMenuItemInfo( parent, item->row, true, &mii );
		}
	}
	
	DrawMenuBar( widget->native );
}

void cgraphics_menubar_remove_item( widget_t *widget, list_item_t *item )
{
	HMENU parent;
	
	parent = cgraphics_menubar_get_native_parent( widget, item );
	
	RemoveMenu( parent, item->row, MF_BYPOSITION );
}

void cgraphics_menubar_disable_item( widget_t *menubar, list_item_t *item )
{
	MENUITEMINFO mii;
	HMENU parent = cgraphics_menubar_get_native_parent( menubar, item );
	
	mii.cbSize = sizeof( MENUITEMINFO );

	mii.fMask = MIIM_STATE;
	GetMenuItemInfo( parent, item->row, true, &mii );
	mii.fState = MFS_DISABLED;
	SetMenuItemInfo( parent, item->row, true, &mii );
}

void cgraphics_menubar_enable_item( widget_t *menubar, list_item_t *item )
{
	
}

void cgraphics_menubar_add_key_binding(widget_t * widget, list_item_t *item, const char * utf8_key,int modifier)
{

}