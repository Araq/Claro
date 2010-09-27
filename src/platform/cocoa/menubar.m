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
#include "graphics.h"
#include "platform/macosx_cocoa.h"

@interface ClaroMenuItem : NSMenuItem
{
	list_item_t *myitem;
}

/* internal init function */
- (void)menuItemHit: (id)sender;

- (void)claroInit: (list_item_t *)item;

@end

@implementation ClaroMenuItem : NSMenuItem

- (void)claroInit: (list_item_t *)item
{
	myitem = item;
}

- (void) menuItemHit: (id)sender
{
	if ( myitem == 0 )
		return;
	
	event_send( OBJECT(myitem), "pushed", "" );
}

@end

void cgraphics_menubar_widget_create( widget_t *widget )
{
	widget->native = [NSApp mainMenu];
}

void cgraphics_menubar_add_key_binding(widget_t * widget, list_item_t *item, const char * utf8_key,int modifier)
{
	NSMenuItem *mitem = item->native;
	NSString *s = [[[NSString alloc] initWithCString:utf8_key encoding:NSUTF8StringEncoding] autorelease];
	if (modifier & cModifierShift)
		[mitem setKeyEquivalent: [[s uppercaseString] autorelease]];
	else
		[mitem setKeyEquivalent: [[s lowercaseString] autorelease]];
	int mask = 0;
	if(modifier & cModifierCommand)
		mask |= NSCommandKeyMask;
	if(modifier & cModifierShift)
		mask |= NSShiftKeyMask;
	[mitem setKeyEquivalentModifierMask:mask];
}

NSMenu *cgraphics_menubar_get_native_parent( widget_t *widget, list_item_t *item )
{
	list_item_t *pitem = 0;
	NSMenuItem *mitem;
	NSMenu *tmp;
	
	if ( strcmp( item->object.parent->type, "claro.graphics.list_item" ) ) // parent isn't an item
		return widget->native;
	else
	{
		pitem = (list_item_t *)item->object.parent;
		
		if ( pitem->native != 0 )
		{
			mitem = pitem->native;
			
			if ( pitem->data[1] == 0 )
			{
				printf( "Attempt to create menu item under an item with no title!\n" );
				exit( 0 );
			}
			
			tmp = [mitem submenu];
			
			if ( tmp == nil )
			{
				tmp = [[NSMenu alloc] initWithTitle:[[NSString alloc] initWithCString:pitem->data[1] encoding:NSUTF8StringEncoding]];
				[mitem setSubmenu:tmp];
			}
			
			return tmp;
		}
	}
	
	return 0;
}

void cgraphics_menubar_new_item( widget_t *widget, list_item_t *item )
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

void cgraphics_menubar_remove_item( widget_t *widget, list_item_t *item )
{
	NSMenu *parent = cgraphics_menubar_get_native_parent( widget, item );
	
	[parent removeItemAtIndex:item->row];
	/* FIXME: need to release item too? */
}

void cgraphics_menubar_disable_item( widget_t *menubar, list_item_t *item )
{
	
}

void cgraphics_menubar_enable_item( widget_t *menubar, list_item_t *item )
{
	
}
