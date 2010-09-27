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

/* Toolbar */

list_item_t *cgraphics_find_list_item_by_id( list_t *list, NSString *id );

/* ClaroToolBar (subclassed from NSToolbar) */
@interface ClaroToolBar : NSToolbar
{
	widget_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;
	
- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar
	itemForItemIdentifier:(NSString *)itemIdentifier
	willBeInsertedIntoToolbar:(BOOL)flag;

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar;
@end

@implementation ClaroToolBar

- (void)claroResize:(NSNotification *)aNotification
{
	
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = widget;
	
	[self setDelegate:self];
	[self setAllowsUserCustomization:NO];
	[self setAutosavesConfiguration: NO]; 
	
	if ( cw->flags & (cToolbarShowImages|cToolbarShowText) )
		[self setDisplayMode: NSToolbarDisplayModeIconAndLabel];
	else if ( cw->flags & cToolbarShowImages )
		[self setDisplayMode: NSToolbarDisplayModeIconOnly];
	else
		[self setDisplayMode: NSToolbarDisplayModeLabelOnly];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSViewFrameDidChangeNotification
		object:self];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar
	itemForItemIdentifier:(NSString *)itemIdentifier
	willBeInsertedIntoToolbar:(BOOL)flag
{
	NSToolbarItem *item = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];
	list_widget_t *lw = (list_widget_t *)cw;
	list_item_t *litem = cgraphics_find_list_item_by_id( &lw->items, itemIdentifier );
	
	if ( litem != NULL )
	{
		image_t *img = litem->data[0];
		const char *txt = litem->data[1];
		const char *ttip = litem->data[2];
		
		if ( txt != 0 )
			[item setLabel:[[NSString alloc] initWithCString:txt encoding:NSUTF8StringEncoding]];
		
		if ( ttip != 0 )
			[item setToolTip:[[NSString alloc] initWithCString:ttip encoding:NSUTF8StringEncoding]];
		
		if ( img != 0 )
			[item setImage:img->native];
	}
	
	return [item autorelease];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar
{
	return [NSArray arrayWithObjects:NSToolbarSeparatorItemIdentifier, nil];
}

@end

list_item_t *cgraphics_find_list_item_by_id( list_t *list, NSString *id )
{
	node_t *n;
	list_item_t *item;
	
	LIST_FOREACH( n, list->head )
	{
		item = (list_item_t *)n->data;
		
		if ( [id isEqualToString:(NSString *)item->nativeid] )
			return item;
		
		item = cgraphics_find_list_item_by_id( &item->children, id );
		
		if ( item != NULL )
			return item;
	}
	
	return NULL;
}

void cgraphics_toolbar_widget_create( widget_t *widget )
{
	ClaroToolBar *tb;
	NSView *parent = cgraphics_get_native_parent( widget );
	NSWindow *win = [parent window];
	
	tb = [[ClaroToolBar alloc]
		initWithIdentifier:@"claroToolbar"
		];
	
	[tb setClaroWidget:widget];
	[win setToolbar:[tb autorelease]];
	//[parent addSubview: tb];
	
	widget->native = (NSControl *)tb;
}

void cgraphics_toolbar_new_icon( widget_t *widget, list_item_t *item )
{
	ClaroToolBar *tb = widget->native;
	//NSToolbarItem *titem;
	NSString *str;
	image_t *img = item->data[0];
	const char *txt = item->data[1];
	//const char *ttip = item->data[2];
	char tmp[512];
	
	if ( img == 0 && txt == 0 )
	{
		/* regardless of style, this is a separator :) */
		
		//item = [[[NSToolbarItem alloc] initWithItemIdentifier:NSToolbarSeparatorItemIdentifier] autorelease];
		[tb insertItemWithItemIdentifier:NSToolbarSeparatorItemIdentifier atIndex:item->row ];
		return;
	}
	
	sprintf( tmp, "clarotoolbaritem%p", item );

	str = [[NSString alloc] initWithCString:tmp encoding:NSUTF8StringEncoding];
	item->nativeid = (int)str;
	
	[tb insertItemWithItemIdentifier:str atIndex:item->row ];
}

void cgraphics_toolbar_remove_icon( widget_t *widget, list_item_t *item )
{
	
}

void cgraphics_toolbar_set_menu( widget_t *widget, list_item_t *item )
{
	
}
