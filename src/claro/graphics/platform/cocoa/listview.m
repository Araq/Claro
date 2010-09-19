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

@interface ClaroTableColumn : NSTableColumn
{
	int column;
}

- (void)setColumnId:(int)col;
- (int)columnId;

@end

@implementation ClaroTableColumn
- (void)setColumnId:(int)col
{
	column = col;
}

- (int)columnId
{
	return column;
}

@end

/* ClaroListView (subclassed from NSScrollView) */
@interface ClaroListView : NSScrollView
{
    widget_t *cw;
    
    NSTableView *tbl;
    //NSTableColumn *tblc;
    
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

- (NSTableView *)getTable;

- (void)setEnabled:(bool)e;
@end

@implementation ClaroListView

- (void)setEnabled:(bool)e
{
	[tbl setEnabled: e];
}

- (NSTableView *)getTable;
{
	return tbl;
}

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
	
	/* REAL LISTVIEW STUFF */
	/*
	tblc = [[NSTableColumn alloc] initWithIdentifier: @"Name"];
	[tblc setEditable: NO];
	*/
	tbl = [[NSTableView alloc] initWithFrame: NSMakeRect(5,40,335,185)];
	[tbl setDrawsGrid: NO];
	[tbl setAllowsColumnSelection: NO];
	[tbl setAllowsColumnReordering: NO];
	[tbl setAllowsEmptySelection: NO];
	[tbl setAllowsMultipleSelection: NO];
	[tbl setColumnAutoresizingStyle: NSTableViewUniformColumnAutoresizingStyle];
	//[tbl addTableColumn: tblc];
	[tbl setDataSource: self]; 
	[tbl setDelegate: self];
	[tbl setTarget:self];
	[tbl setDoubleAction:@selector(doubleclicked:)];
	[tbl setAction:@selector(selected:)];
	
	[self setHasHorizontalScroller: NO];
	[self setHasVerticalScroller: YES];
	[self setDocumentView: tbl];
	[self setAutoresizingMask: NSViewWidthSizable | NSViewHeightSizable];
	
	cnt = 0;
}

- (void)doubleclicked:(id)sender
{
	int rowIndex = [sender selectedRow];
}

- (void)selected:(id)sender
{
	list_widget_t *list = (list_widget_t *)cw;
	listview_widget_t *lw = (listview_widget_t *)cw;
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
	listview_widget_t *lvw = (listview_widget_t *)cw;
	
	item->native = (void *)malloc( sizeof(NSObject *) * lvw->widget.columns );
	memset( item->native, 0, sizeof(NSObject *) * lvw->widget.columns );
	
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
	listview_widget_t *lw = (listview_widget_t *)cw;
	
	[tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:item->row] byExtendingSelection:NO];
		
	lw->selected = item;
	event_send( OBJECT(cw), "selected", "p", item );
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return cnt;
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(ClaroTableColumn *)aTableColumn row:(int)rowIndex
{
	list_widget_t *list = (list_widget_t *)cw;
	list_item_t *item;
	node_t *n;
	NSObject **in;
	int a;
	
	LIST_FOREACH( n, list->items.head )
	{
		item = (list_item_t *)n->data;
		
		if ( item->row == rowIndex )
		{
			in = item->native;
			
			a = [aTableColumn columnId];
			
			if ( list->coltypes[a] == CLIST_TYPE_UINT )
			{
				unsigned int *uid = item->data[a];
				
				uid[0] = [anObject boolValue];
				
				event_send( OBJECT(item), "changed", "ii", a, uid[0] );
			}
			
			[in[a] release];
			in[a] = 0;
		}
	}
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(ClaroTableColumn *)aTableColumn row:(int)rowIndex
{
	//listview_widget_t *lw = (listview_widget_t *)cw;
	list_widget_t *list = (list_widget_t *)cw;
	list_item_t *item;
	node_t *n;
	NSString *str;
	NSNumber *num;
	char *cstr;
	NSObject **in;
	int a;
	
	LIST_FOREACH( n, list->items.head )
	{
		item = (list_item_t *)n->data;
		
		if ( item->row == rowIndex )
		{
			in = item->native;
			
			a = [aTableColumn columnId];
			
			if ( in[a] == 0 )
			{
				if ( list->coltypes[a] == CLIST_TYPE_STRING )
				{
					cstr = item->data[a];
					str = [[NSString alloc] initWithCString:cstr encoding:NSUTF8StringEncoding];
					in[a] = str;
				}
				else if ( list->coltypes[a] == CLIST_TYPE_UINT )
				{
					unsigned int chk = *((int *)item->data[a]);
					num = [[NSNumber alloc] initWithUnsignedInt:chk];
					in[a] = num;
				}
				else if ( list->coltypes[a] == CLIST_TYPE_DOUBLE )
				{
					double dbl = *((double *)item->data[a]);
					num = [[NSNumber alloc] initWithDouble:dbl];
					in[a] = num;
				}
			}
			
			return in[a];
		}
	}
	
	return 0;
}
@end

void cgraphics_listview_widget_create( widget_t *widget )
{
	ClaroListView *lv;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	lv = [[ClaroListView alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[lv setClaroWidget:widget];
	[parent addSubview: lv];
	
	widget->native = (NSControl *)lv;
}

void cgraphics_listview_widget_create_columns( widget_t *widget )
{
	ClaroListView *lv = (ClaroListView *)widget->native;
	listview_widget_t *lvw = (listview_widget_t *)widget;
	list_widget_t *list = (list_widget_t *)widget;
	ClaroTableColumn *tc;
	int a;
	
	for ( a = 0; a < lvw->widget.columns; a++ )
	{
		tc = [[ClaroTableColumn alloc] initWithIdentifier: @""];
		[tc setColumnId:a];
		[[tc headerCell] setStringValue: [[NSString alloc] initWithCString:lvw->titles[a] encoding:NSUTF8StringEncoding]];
		[tc setEditable: NO];
		if ( list->coltypes[a] == CLIST_TYPE_UINT )
		{
			NSButtonCell *bc;
			bc = [[NSButtonCell alloc] init];
			[bc setButtonType:NSSwitchButton];
			[bc setTitle: @""];
			[tc setDataCell: bc];
		}
		else if ( list->coltypes[a] == CLIST_TYPE_DOUBLE )
		{
			NSLevelIndicatorCell *nlic;
			nlic = [[NSLevelIndicatorCell alloc] init];
			[nlic setLevelIndicatorStyle:NSContinuousCapacityLevelIndicatorStyle];
			[nlic setMaxValue:1.0];
			[nlic setEnabled:NO];
			[tc setDataCell: nlic];
		}
		[[lv getTable] addTableColumn: tc];
	}
}

void cgraphics_listview_new_row( widget_t *widget, list_item_t *item )
{
	ClaroListView *lv = (ClaroListView *)widget->native;
	
	// insert at item->row item->data[0]
	
	[lv insertRow:item];
}

void cgraphics_listview_remove_row( widget_t *widget, list_item_t *item )
{
	ClaroListView *lv = (ClaroListView *)widget->native;
	
	// delete item->row
	
	[lv deleteRow:item];
}

void cgraphics_listview_select_row( widget_t *widget, list_item_t *item )
{
	ClaroListView *lv = (ClaroListView *)widget->native;
	
	// select item->row
	
	[lv selectRow:item];
}
