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

/* Progressbar */

/* ClaroProgressBar (subclassed from NSProgressIndicator) */
@interface ClaroProgressBar : NSProgressIndicator
{
    widget_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;
@end

@implementation ClaroProgressBar

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );
	widget_set_position( cw, frame.origin.x, frame.origin.y, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = widget;
	
	[self setIndeterminate:NO];
	[self setMinValue:0.0];
	[self setMaxValue:1.0];
	[self setDoubleValue:0.0];
	[self setControlTint:NSBlueControlTint];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSViewFrameDidChangeNotification
		object:self];
}

@end

void cgraphics_progress_widget_create(widget_t *widget)
{
    ClaroProgressBar *pb;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	if ( widget->size_req->h == -1 )
		widget->size_req->h = NSProgressIndicatorPreferredAquaThickness;
	
	pb = [[ClaroProgressBar alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[pb setClaroWidget:widget];
	[parent addSubview: pb];
	
	widget->native = (NSControl *)pb;
}

void cgraphics_progress_set_level( widget_t *widget, double percentage )
{
	ClaroProgressBar *bp = (ClaroProgressBar *)widget->native;
	
	[bp setDoubleValue:percentage];
}

void cgraphics_progress_set_orientation( widget_t *widget )
{
	
}

