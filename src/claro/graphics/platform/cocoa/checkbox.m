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

/* Checkbox */

/* ClaroCheckButton (subclassed from NSButton) */
@interface ClaroCheckButton : NSButton
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

@implementation ClaroCheckButton

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

- (void)mouseDown:(NSEvent *)event
{
	[super mouseDown:event];
	[self mouseUp:event];
	
	checkbox_widget_t *w = (checkbox_widget_t *)cw;
	
	w->checked = ([self state] == NSOnState);
	
	event_send( OBJECT(cw), "changed", "i", w->checked );
}

- (void)mouseUp:(NSEvent *)event
{
	/* nothing */
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

void cgraphics_checkbox_widget_create(widget_t *widget)
{
	ClaroCheckButton *pb;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	
	pb = [[ClaroCheckButton alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[pb setClaroWidget:widget];
	[pb setButtonType:NSSwitchButton];
	[parent addSubview: pb];
	
	widget->native = (NSControl *)pb;
}

void cgraphics_checkbox_set_text( checkbox_widget_t *widget )
{
	ClaroCheckButton *pb = (ClaroCheckButton *)widget->widget.native;
	NSString *str;

	str = [[NSString alloc] initWithCString:widget->text encoding:NSUTF8StringEncoding];
		
	[pb setTitle:str];
		
	[str release];
}


void cgraphics_checkbox_update_checked( widget_t *widget )
{
	ClaroCheckButton *pb = (ClaroCheckButton *)widget->native;
	checkbox_widget_t *w = (checkbox_widget_t *)widget;
	[pb setState: w->checked ? NSOnState : NSOffState ];
}

