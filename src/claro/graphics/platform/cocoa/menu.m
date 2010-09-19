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


#define _PLATFORM_INC
#include <claro/graphics.h>
//#include <cairo-quartz.h>
#include "../macosx_cocoa.h"

@interface ClaroMenuItem : NSMenuItem
{
	list_item_t *myitem;
}

/* internal init function */
- (void)menuItemHit: (id)sender;

- (void)claroInit: (list_item_t *)item;

@end

void cgraphics_menu_widget_create( widget_t *widget )
{
	widget->native = [[[NSMenu alloc] initWithTitle:@"Contextual Menu"] autorelease];
}

void cgraphics_menu_new_item( widget_t *widget, list_item_t *item )
{
	NSMenu *parent = cgraphics_menubar_get_native_parent( widget, item );
	ClaroMenuItem *mitem;
	image_t *img = item->data[0];
	
	if ( item->data[1] == 0 )
		mitem = [NSMenuItem separatorItem];
	else
	{
		mitem = [[ClaroMenuItem alloc] initWithTitle:[[NSString alloc] initWithCString:item->data[1] encoding:NSUTF8StringEncoding] action:@selector(menuItemHit:) keyEquivalent:@""];
		[mitem claroInit:item];
		[mitem setTarget:mitem];
		
		if ( img != 0 )
			[mitem setImage:(NSImage *)img->native];
	}
	
	[parent addItem:mitem];
	item->native = mitem;
}

void cgraphics_menu_remove_item( widget_t *widget, list_item_t *item )
{
	NSMenu *parent = cgraphics_menubar_get_native_parent( widget, item );
	
	[parent removeItemAtIndex:item->row];
	/* FIXME: need to release item too? */
}

void cgraphics_menu_disable_item( widget_t *menu, list_item_t *item )
{
}

void cgraphics_menu_enable_item( widget_t *menu, list_item_t *item )
{
}

void cgraphics_menu_popup( object_t *menu, int x, int y, int flags )
{
	NSMenu *m = (NSMenu *)WIDGET(menu)->native;
	NSControl *c = (NSControl *)(WIDGET(menu->parent)->native);
	//mouseEventWithType:location:modifierFlags:timestamp:windowNumber:context:eventNumber:clickCount:pressure:
	[NSMenu popUpContextMenu:m withEvent:[NSEvent mouseEventWithType: NSRightMouseDown location:[[c window] mouseLocationOutsideOfEventStream]
													modifierFlags:0 timestamp:[[NSApp currentEvent] timestamp]
													windowNumber:[[((NSControl *)WIDGET(menu->parent)->native) window] windowNumber]
													context:NULL eventNumber:0
													clickCount:0 pressure:1.0 ] forView:nil];
}
