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

/* ClaroComboBox (subclassed from NSComboBox) */
@interface ClaroComboBox : NSComboBox
{
    widget_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroMove:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

- (void)doubleclicked:(id)sender;
- (void)comboBoxSelectionDidChange:(NSNotification *)notification;

/* custom stuff */
- (void)selectRow:(list_item_t *)item;
@end

@implementation ClaroComboBox

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
	
	[self setEditable: NO];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSWindowDidResizeNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroMove:) name:NSWindowDidMoveNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(comboBoxSelectionDidChange:) name:NSComboBoxSelectionDidChangeNotification
		object:self];
}

- (void)doubleclicked:(id)sender
{
	int rowIndex = [sender selectedRow];
}

- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
	list_widget_t *list = (list_widget_t *)cw;
	combo_widget_t *lw = (combo_widget_t *)cw;
	list_item_t *item, *sel=0;
	node_t *n;
	int rowIndex = [self indexOfSelectedItem];
	
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

- (void)selectRow:(list_item_t *)item
{
	combo_widget_t *lw = (combo_widget_t *)cw;
	
	[self selectItemAtIndex:item->row];
	[self setObjectValue:[self objectValueOfSelectedItem]];
		
	lw->selected = item;
	event_send( OBJECT(cw), "selected", "p", item );
}
@end

void cgraphics_combo_widget_create( widget_t *widget )
{
	ClaroComboBox *lv;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	lv = [[ClaroComboBox alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[lv setClaroWidget:widget];
	[parent addSubview: lv];
	
	widget->native = (NSControl *)lv;
}

void cgraphics_combo_new_row( widget_t *widget, list_item_t *item )
{
	ClaroComboBox *lv = (ClaroComboBox *)widget->native;
	
	// insert at item->row item->data[0]
	
	[lv insertItemWithObjectValue:[[NSString alloc] initWithCString:item->data[0] encoding:NSUTF8StringEncoding] atIndex:item->row];
}

void cgraphics_combo_remove_row( widget_t *widget, list_item_t *item )
{
	ClaroComboBox *lv = (ClaroComboBox *)widget->native;
	
	// delete item->row
	
	//[lv deleteRow:item];
}

void cgraphics_combo_select_row( widget_t *widget, list_item_t *item )
{
	ClaroComboBox *lv = (ClaroComboBox *)widget->native;
	
	// select item->row
	
	[lv selectRow:item];
}
