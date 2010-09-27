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
//#include <cairo-quartz.h>
#include "platform/macosx_cocoa.h"

/* ClaroPushButton (subclassed from NSButton) */
@interface ClaroPushButton : NSButton
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

@implementation ClaroPushButton

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
	if ( ![self isEnabled] )
		return;
	
	event_send( OBJECT(cw), "pushed", "" );
	
	[super mouseDown:event];
	[self mouseUp:event];
}

- (void)mouseUp:(NSEvent *)event
{
	/* nothing */
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = widget;
	
	[self setButtonType:NSMomentaryPushInButton];
	[self setBezelStyle:NSRoundedBezelStyle];
	
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

void cgraphics_button_widget_create( widget_t *widget )
{
	ClaroPushButton *pb;
	NSView *parent = cgraphics_get_native_parent( widget );
	int type = NSRoundedBezelStyle;
	
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;
	else
		type = NSSmallSquareBezelStyle;
	
	pb = [[ClaroPushButton alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[pb setClaroWidget:widget];
	[pb setBezelStyle:type];
	[parent addSubview: pb];
	
	widget->native = (NSControl *)pb;
}

void cgraphics_button_update_text( button_widget_t *widget )
{
	ClaroPushButton *pb = (ClaroPushButton *)widget->widget.native;
	NSString *str;

	str = [[NSString alloc] initWithCString:widget->text encoding:NSUTF8StringEncoding];
		
	[pb setTitle:str];
		
	[str release];
}

void cgraphics_button_update_image( button_widget_t *widget, const char *filename)
{
	
}
