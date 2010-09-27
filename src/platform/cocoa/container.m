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

/* Container */

/* ClaroContainer (subclassed from NSView) */
@interface ClaroContainer : NSView
{
	widget_t *cw;
	container_widget_t *sw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

/* subclass some functions so we do as expected */
- (BOOL)isFlipped;

@end

@implementation ClaroContainer

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );
	widget_set_position( cw, frame.origin.x, frame.origin.y, 1 );
	
	widget_set_content_size( cw, frame.size.width, frame.size.height, 1 );
	widget_set_content_position( cw, 0, 0, 1 );
	
	widget_resized_handle( OBJECT(cw), 0 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = widget;
	sw = (container_widget_t *)cw;
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSViewFrameDidChangeNotification
		object:self];
}

- (BOOL)isFlipped
{
	return YES;
}

@end

void cgraphics_container_widget_create( widget_t *widget )
{
	ClaroContainer *co;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	co = [[ClaroContainer alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[parent addSubview: co];
	[co setClaroWidget:widget];
	
	widget->native = (NSControl *)co;
	
	widget_set_content_size( widget, widget->size_req->w, widget->size_req->h, 1 );
}

