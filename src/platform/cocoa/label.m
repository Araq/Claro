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

/* Label */

/* ClaroLabel (subclassed from NSTextField) */
@interface ClaroLabel : NSTextField
{
	widget_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroMove:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;
@end

@implementation ClaroLabel

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
}

@end

void cgraphics_label_widget_create(widget_t *widget)
{
	ClaroLabel *tb;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 21;
	
	tb = [[ClaroLabel alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[tb setEditable: NO];
	[tb setAlignment: NSLeftTextAlignment];
	[tb setDrawsBackground: NO];
	[tb setBordered: NO];
	
	[tb setClaroWidget:widget];
	[parent addSubview: tb];
	
	widget->native = (NSControl *)tb;
}

void cgraphics_label_update_text(textbox_widget_t *widget)
{
	ClaroLabel *tb = (ClaroLabel *)widget->widget.native;
	NSString *str = [[NSString alloc] initWithUTF8String: widget->text];
	[tb setStringValue:str];
}

void cgraphics_label_update_justify( widget_t *widget )
{
	ClaroLabel *tb = (ClaroLabel *)widget->native;
	
	switch ( widget->flags ) {
		case cLabelRight:
			[tb setAlignment: NSRightTextAlignment];
			break;
		case cLabelCenter:
			[tb setAlignment: NSCenterTextAlignment];
			break;
		case cLabelFill:
		case cLabelLeft:
		default:
			[tb setAlignment: NSLeftTextAlignment];
			break;
	}
}

