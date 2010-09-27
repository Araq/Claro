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

#define IsALeafNode ((id)-1)

@interface ClaroTableColumn : NSTableColumn
{
	int column;
}

- (void)setColumnId:(int)col;
- (int)columnId;

@end

@interface ClaroTreeItem : NSObject
{
    ClaroTreeItem *parent;
	
	object_t *tv;
	
	list_item_t *me;
	
	NSMutableArray *children;
	
	NSString *title;
}

- (int)numberOfChildren;			// Returns -1 for leaf nodes
- (ClaroTreeItem *)childAtIndex:(int)n;	// Invalid to call on leaf nodes
- (NSString *)stringName;
- (void)addChild:(ClaroTreeItem *)c atRow:(int)row;

- (list_item_t *)me;
@end

@implementation ClaroTreeItem

- (list_item_t *)me
{
	return me;
}

- (NSString *)stringName
{
	if ( me == NULL )
		return @"ERROR";
	
	return title;
}

- (id)initWithTreeView:(object_t *)treeview item:(list_item_t *)item parent:(ClaroTreeItem *)obj {
    if ((self = [super init])) {
        parent = obj;
    }
	children = [[NSMutableArray alloc] initWithCapacity: 1];
	self->tv = treeview;
	self->me = item;
	title = @"WTF";
	if ( item != NULL ) {
		item->native = self;
		//printf( "*********NEWROW***** %s->%s\n", (obj->me?obj->me->data[1]:"ROOT"), item->data[1] );
		[obj addChild:self atRow:item->row];
		
		title = [[NSString alloc] initWithCString:item->data[1] encoding:NSUTF8StringEncoding];
	}
    return self;
}

- (void)addChild:(ClaroTreeItem *)c atRow:(int)row
{
	//printf( "**** [%s addChild:%s atRow:%d]\n", (me?me->data[1]:"ROOT"), (c->me?c->me->data[1]:"ROOT"), row );
	[children insertObject:c atIndex:row];
}

- (void)dealloc {
    [children release];
    [super dealloc];
}

- (ClaroTreeItem *)childAtIndex:(int)n {
	//printf( "**** [%s childAtIndex:%d]\n", (me?me->data[1]:"ROOT"), n );
    return [children objectAtIndex:n];
}

- (int)numberOfChildren {
	//printf( "**** [%s numberOfChildren] -> %d\n", (me?me->data[1]:"ROOT"), [children count] );
	return [children count];
}

@end

@interface ClaroOutlineView : NSOutlineView
{
	object_t *obj;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

- (NSMenu *)menuForEvent:(NSEvent *)theEvent;
@end

@implementation ClaroOutlineView

/* http://www.cocoadev.com/index.pl?RightClickSelectInTableView */
- (NSMenu *)menuForEvent:(NSEvent *)theEvent
{
	NSPoint mousePoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	int row = [self rowAtPoint:mousePoint];
	list_item_t *sel = NULL;
	
	if (row >= 0)
	{
		[self selectRow:row byExtendingSelection:NO];
		
		ClaroTreeItem *crow = [self itemAtRow: row];
		sel = [crow me];
		
		[[self delegate] selected: self];
	}
	else
	{
		// you can disable this if you don't want clicking on an empty space to deselect all rows
		[self deselectAll:self];
	}
	
	/* right clicked is a bad name for this */
	event_send( OBJECT(obj), "right_clicked", "" );
	
	// make sure you return something!
	return [super menuForEvent:theEvent];
}

- (void)setClaroWidget:(widget_t *)widget
{
	obj = OBJECT(widget);
}

@end


/* ClaroListView (subclassed from NSScrollView) */
@interface ClaroTreeView : NSScrollView
{
    widget_t *cw;
    
    ClaroOutlineView *tbl;
    //NSTableColumn *tblc;

	ClaroTreeItem *root;
    
    int cnt;
}

- (NSOutlineView *)getTable;

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

@implementation ClaroTreeView

- (NSOutlineView *)getTable;
{
	return tbl;
}

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( OBJECT(cw), frame.size.width, frame.size.height, 1 );
}

- (void)claroMove:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_position( OBJECT(cw), frame.origin.x, frame.origin.y, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( OBJECT(cw) );
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
	
	/* REAL TREEVIEW STUFF */
	root = [[ClaroTreeItem alloc] initWithTreeView:OBJECT(cw) item:NULL parent:nil];
	/*
	tblc = [[NSTableColumn alloc] initWithIdentifier: @"Name"];
	[tblc setEditable: NO];
	*/
	tbl = [[ClaroOutlineView alloc] initWithFrame: NSMakeRect(5,40,335,185)];
	[tbl setClaroWidget: cw];
	[tbl setDrawsGrid: NO];
	[tbl setAllowsColumnSelection: NO];
	[tbl setAllowsColumnReordering: NO];
	[tbl setAllowsEmptySelection: NO];
	[tbl setAllowsMultipleSelection: NO];
	[tbl setColumnAutoresizingStyle: NSTableViewUniformColumnAutoresizingStyle];
	/*//[tbl addTableColumn: tblc];*/
	
	ClaroTableColumn *tc = [[ClaroTableColumn alloc] initWithIdentifier: @""];
	[tc setColumnId:0];
	[[tc headerCell] setStringValue: [[NSString alloc] initWithCString:"TEST" encoding:NSUTF8StringEncoding]];
	[tc setEditable: NO];
	[tc setDataCell:[[NSBrowserCell alloc] init]];
	[tbl addTableColumn: tc];
	[tbl setOutlineTableColumn: tc];
	
	[tbl setDataSource: self]; 
	[tbl setDelegate: self];
	[tbl setTarget:self];
	[tbl setDoubleAction:@selector(doubleclicked:)];
	[tbl setAction:@selector(selected:)];
	
	[self setHasHorizontalScroller: NO];
	[self setHasVerticalScroller: YES];
	[self setDocumentView: tbl];
	//[self setAutoresizingMask: NSViewWidthSizable | NSViewHeightSizable];
	
	[tbl setHeaderView:nil];
	
//	cnt = 0;
}

- (void)doubleclicked:(id)sender
{
	//int rowIndex = [sender selectedRow];
}

- (int)numberOfColumns
{
	return 1;
}

- (void)selected:(id)sender
{
	//list_widget_t *list = (list_widget_t *)cw;
	treeview_widget_t *lw = (treeview_widget_t *)cw;
	list_item_t /**item,*/ *sel=0;
	//node_t *n;
	int rowIndex = [sender selectedRow];
	ClaroTreeItem *row = [sender itemAtRow: rowIndex];
	
	sel = [row me];
	
	lw->selected = sel;
	event_send( OBJECT(cw), "selected", "p", sel );
}

- (void)insertRow:(list_item_t *)item
{
	treeview_widget_t *lvw = (treeview_widget_t *)cw;
	ClaroTreeItem *p = root;
	
	if ( item->parent_item != NULL )
		p = item->parent_item->native;
	
	[[ClaroTreeItem alloc] initWithTreeView:OBJECT(lvw) item:item parent:p];
	
	if ( item->parent_item != NULL )
		[tbl reloadItem:item->parent_item->native reloadChildren:YES];
	else
		[tbl reloadData];
}

- (void)deleteRow:(list_item_t *)item
{
	//[tbl reloadItem];
}

- (void)selectRow:(list_item_t *)item
{
	listview_widget_t *lw = (listview_widget_t *)cw;
	
	[tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:item->row] byExtendingSelection:NO];
		
	lw->selected = item;
	event_send( OBJECT(cw), "selected", "p", item );
}

- (int)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item {
	if ( item == nil ) item = root;
	return [item numberOfChildren];
    //return (item == nil) ? [root numberOfChildren] : [item numberOfChildren];
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item {
	if ( item == nil ) item = root;
	return [item numberOfChildren] > 0;
//    return (item == nil) ? ([root numberofChildren] != 0) : ([item numberOfChildren] != 0);
}

- (id)outlineView:(NSOutlineView *)outlineView child:(int)index ofItem:(id)item {
	if ( item == nil ) item = root;
	return [item childAtIndex:index];
//    return [(ClaroTreeItem *)(item==nil?self->root:item) childAtIndex:index];
}

- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item {
    return (item == nil) ? @"UNKNOWN" : (id)[item stringName];
}

- (void)outlineView:(NSOutlineView *)outlineView willDisplayCell:(id)cell forTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	ClaroTreeItem *i = (ClaroTreeItem *)item;
	image_t *im = (image_t *)([i me])->data[0];
	[cell setImage:(NSImage *)im->native];
}


// Delegate methods

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(NSTableColumn *)tableColumn item:(id)item {
    return NO;
}
@end

void cgraphics_treeview_widget_create( widget_t *widget )
{
	ClaroTreeView *lv;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	lv = [[ClaroTreeView alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[lv setClaroWidget:widget];
	[parent addSubview: lv];
	
	widget->native = (NSControl *)lv;
}

void cgraphics_treeview_new_row( widget_t *widget, list_item_t *item )
{
	ClaroTreeView *lv = (ClaroTreeView *)widget->native;
	
	// insert at item->row item->data[0]
	
	[lv insertRow:item];
}

void cgraphics_treeview_remove_row( widget_t *widget, list_item_t *item )
{
	
}

void cgraphics_treeview_select_row( widget_t *widget, list_item_t *item )
{
	ClaroTreeView *tv = (ClaroTreeView *)widget->native;
	
	[tv selectRow: item];
}

void cgraphics_treeview_expand( widget_t *widget, list_item_t *item )
{
	ClaroTreeView *tv = (ClaroTreeView *)widget->native;
	ClaroTreeItem *ti = (ClaroTreeItem *)item->native;

	[[tv getTable] expandItem:ti];
}

void cgraphics_treeview_collapse( widget_t *widget, list_item_t *item )
{
	
}
void cgraphics_treeview_edit_row( widget_t *widget, list_item_t *item )
{
	
}
