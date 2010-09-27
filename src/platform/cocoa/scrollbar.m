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

/* ClaroScrollBar (subclassed from NSScroller) */
@interface ClaroScrollBar : NSScroller
{
    widget_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

- (void)setHorizontal: (bool)val;

- (void)trackKnob:(NSEvent *)theEvent;
- (void)trackScrollButtons:(NSEvent *)theEvent;
@end

@implementation ClaroScrollBar

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( OBJECT(cw), frame.size.width, frame.size.height, 1 );
	widget_set_position( OBJECT(cw), frame.origin.x, frame.origin.y, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( OBJECT(cw) );
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = widget;
	
	[self setControlTint:NSBlueControlTint];
	[self setEnabled:YES];
	[self setHorizontal: YES];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSViewFrameDidChangeNotification
		object:self];
}

- (void)setHorizontal: (bool)val
{
	sFlags.isHoriz = val?1:0;
}

- (void)trackKnob:(NSEvent *)theEvent
{
	[super trackKnob: theEvent];
	event_send( OBJECT(cw), "scroll", "" );
}

- (void)trackScrollButtons:(NSEvent *)theEvent
{
	
}

@end

void cgraphics_scrollbar_widget_create( widget_t *w )
{
	ClaroScrollBar *pb;
	NSView *parent = cgraphics_get_native_parent( w );
	
	pb = [[ClaroScrollBar alloc]
		initWithFrame: NSMakeRect( w->size_req->x, w->size_req->y, w->size_req->w, w->size_req->h )
		];
	
	[pb setClaroWidget:w];
	[pb setHorizontal:!(w->flags&cScrollbarVertical)];
	[parent addSubview: pb];
	
	w->native = (NSControl *)pb;
}

int cgraphics_scrollbar_get_sys_width( )
{
	return [NSScroller scrollerWidth];
}

void cgraphics_scrollbar_set_range( widget_t *w )
{
	ClaroScrollBar *csb = (ClaroScrollBar *)w->native;
	scrollbar_widget_t *sbw = (scrollbar_widget_t *)w;
	float prop = 1;
	int rmax = sbw->max - sbw->min;
	
	if ( rmax > 0 ) {
		prop = 0.9-(rmax/10);
	}
	
	[csb setFloatValue:[csb floatValue] knobProportion: prop];
}

void cgraphics_scrollbar_set_pos( widget_t *w, int pos )
{
	ClaroScrollBar *csb = (ClaroScrollBar *)w->native;
	scrollbar_widget_t *sbw = (scrollbar_widget_t *)w;
	int rmax = sbw->max - sbw->min;
	
	pos -= sbw->min;
	
	if ( rmax == 0 )
		rmax = 1;
	
	[csb setFloatValue: 1.0f / rmax * pos];
}

int cgraphics_scrollbar_get_pos( widget_t *w )
{
	ClaroScrollBar *csb = (ClaroScrollBar *)w->native;
	scrollbar_widget_t *sbw = (scrollbar_widget_t *)w;
	
	return sbw->min + ((sbw->max - sbw->min) * [csb floatValue]);
}
