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

/* ClaroListBox (subclassed from NSScrollView) */
@interface ClaroListBox : NSScrollView
{
    widget_t *cw;
    
    NSTableView *tbl;
    NSTableColumn *tblc;
    
    int cnt;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroMove:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

- (void)doubleclicked:(id)sender;
- (void)selected:(id)sender;

/* custom stuff */
- (void)insertRow:(list_item_t *)item;
- (void)deleteRow:(list_item_t *)item;
- (void)selectRow:(list_item_t *)item;
@end

@implementation ClaroListBox

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );
}

- (void)claroMove:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_position( cw, frame.origin.x, frame.origin.y, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = widget;
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSWindowDidResizeNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroMove:) name:NSWindowDidMoveNotification
		object:self];
	
	/* REAL LISTBOX STUFF */
	
	tblc = [[NSTableColumn alloc] initWithIdentifier: @"Name"];
	[tblc setEditable: NO];
	
	tbl = [[NSTableView alloc] initWithFrame: NSMakeRect(5,40,335,185)];
	[tbl setDrawsGrid: NO];
	[tbl setAllowsColumnSelection: NO];
	[tbl setAllowsColumnReordering: NO];
	[tbl setAllowsEmptySelection: NO];
	[tbl setAllowsMultipleSelection: NO];
	[tbl setColumnAutoresizingStyle: NSTableViewUniformColumnAutoresizingStyle];
	[tbl addTableColumn: tblc];
	[tbl setDataSource: self]; 
	[tbl setDelegate: self];
	[tbl setTarget:self];
	[tbl setDoubleAction:@selector(doubleclicked:)];
	[tbl setAction:@selector(selected:)];
	
	[tbl setHeaderView:nil];
	
	[self setHasHorizontalScroller: NO];
	[self setHasVerticalScroller: YES];
	[self setDocumentView: tbl];
	[self setAutoresizingMask: NSViewWidthSizable | NSViewHeightSizable];
	
	[tblc setResizingMask: NSTableColumnAutoresizingMask];
	[tblc setWidth:10000];
	
	cnt = 0;
}

- (void)doubleclicked:(id)sender
{
	int rowIndex = [sender selectedRow];
}

- (void)selected:(id)sender
{
	list_widget_t *list = (list_widget_t *)cw;
	listbox_widget_t *lw = (listbox_widget_t *)cw;
	list_item_t *item, *sel=0;
	node_t *n;
	int rowIndex = [sender selectedRow];
	
	LIST_FOREACH( n, list->items.head )
	{
		item = (list_item_t *)n->data;
		
		if ( item->row == rowIndex )
		{
			sel = item;
		}
	}
	
	lw->selected = sel;
	event_send( OBJECT(cw), "selected", "p", sel );
}

- (void)insertRow:(list_item_t *)item
{
	cnt++;
	[tbl reloadData];
}

- (void)deleteRow:(list_item_t *)item
{
	cnt--;
	[tbl reloadData];
}

- (void)selectRow:(list_item_t *)item
{
	listbox_widget_t *lw = (listbox_widget_t *)cw;
	
	[tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:item->row] byExtendingSelection:NO];
		
	lw->selected = item;
	event_send( OBJECT(cw), "selected", "p", item );
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return cnt;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex
{
	list_widget_t *list = (list_widget_t *)cw;
	list_item_t *item;
	node_t *n;
	CFStringRef str;
	char *cstr;
	
	LIST_FOREACH( n, list->items.head )
	{
		item = (list_item_t *)n->data;
		
		if ( item->row == rowIndex )
		{
			if ( item->native == 0 )
			{
				cstr = item->data[0];
				str = CFStringCreateWithCString( NULL, cstr, strlen( cstr ) );
				item->native = str;
			}
			
			return item->native;
		}
	}
	
	return 0;
}
@end

void cgraphics_listbox_widget_create( widget_t *widget )
{
	ClaroListBox *lv;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	lv = [[ClaroListBox alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[lv setClaroWidget:widget];
	[parent addSubview: lv];
	
	widget->native = (NSControl *)lv;
}

void cgraphics_listbox_new_row( widget_t *widget, list_item_t *item )
{
	ClaroListBox *lv = (ClaroListBox *)widget->native;
	
	// insert at item->row item->data[0]
	
	[lv insertRow:item];
}

void cgraphics_listbox_remove_row( widget_t *widget, list_item_t *item )
{
	ClaroListBox *lv = (ClaroListBox *)widget->native;
	
	// delete item->row
	
	[lv deleteRow:item];
}

void cgraphics_listbox_select_row( widget_t *widget, list_item_t *item )
{
	ClaroListBox *lv = (ClaroListBox *)widget->native;
	
	// select item->row
	
	[lv selectRow:item];
}
