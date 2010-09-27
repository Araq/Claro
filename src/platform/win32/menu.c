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

/* Menu */

extern list_t mblist;
extern int mblist_cmdid;

void cgraphics_menu_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	window_widget_t *wp = (window_widget_t *)parent;
	HWND hwnd_parent = parent->native; // menu is NOT inside the container widget :)
	HMENU menu;
	RECT r;
	
	menu = CreatePopupMenu( );
	
	widget->native = menu;
}

HMENU cgraphics_menu_get_native_parent( widget_t *widget, list_item_t *item )
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
			ppmenu = cgraphics_menu_get_native_parent( widget, pitem );
			
			pitem->native = CreatePopupMenu( );
			mii.fMask = MIIM_SUBMENU;
			mii.hSubMenu = pitem->native;
			SetMenuItemInfo( ppmenu, pitem->row, true, &mii );
		}
		
		return pitem->native;
	}
}

void cgraphics_menu_new_item( widget_t *widget, list_item_t *item )
{
	HMENU parent;
	MENUITEMINFO mii;
	node_t *n;
	
	mii.cbSize = sizeof( MENUITEMINFO );
	
	parent = cgraphics_menu_get_native_parent( widget, item );
	
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
}

void cgraphics_menu_remove_item( widget_t *widget, list_item_t *item )
{
	HMENU parent;
	
	parent = cgraphics_menu_get_native_parent( widget, item );
	
	RemoveMenu( parent, item->row, MF_BYPOSITION );
}

void cgraphics_menu_disable_item( widget_t *menu, list_item_t *item )
{
	MENUITEMINFO mii;
	HMENU parent = cgraphics_menu_get_native_parent( menu, item );
	
	mii.cbSize = sizeof( MENUITEMINFO );

	mii.fMask = MIIM_STATE;
	GetMenuItemInfo( parent, item->row, true, &mii );
	mii.fState = MFS_DISABLED;
	SetMenuItemInfo( parent, item->row, true, &mii );
}

void cgraphics_menu_enable_item( widget_t *menu, list_item_t *item )
{
	
}

void cgraphics_menu_popup( widget_t *menu, int x, int y, int flags )
{
	TrackPopupMenu( menu->native, 0, x, y, 0, WIDGET(OBJECT(menu)->parent)->native, 0 );
}
