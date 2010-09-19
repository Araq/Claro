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
// //#include <cairo-quartz.h>
#include "macosx_cocoa.h"

NSAutoreleasePool *arpool = NULL;

@interface ClaroApplication : NSObject
{
}

- (void)applicationWillFinishLaunching:(NSNotification *) not;
- (void)applicationDidFinishLaunching:(NSNotification *) not;

- (void)runOnce;

@end

@implementation ClaroApplication
- (void) applicationWillFinishLaunching: (NSNotification *) not
{
	/* */
}

- (void) applicationDidFinishLaunching: (NSNotification *) not
{
	/* */
}

- (void) dealloc
{
	[super dealloc];
}

- (void)runOnce
{
	for ( ; ; )
	{
		NSEvent *event = [NSApp nextEventMatchingMask: NSAnyEventMask
						untilDate: NULL
						inMode: NSDefaultRunLoopMode
						dequeue: YES ];
	
		if ( event == nil )
			break;
		
		[NSApp sendEvent: event];
		
		if ( ![NSApp isRunning] )
			break;
	}
}

@end

ClaroApplication *capp = NULL;

void CPSEnableForegroundOperation( ProcessSerialNumber* psn );

/* called at initialisation */
void cgraphics_init( )
{
	NSApplicationLoad( );
	
	arpool = [[NSAutoreleasePool alloc] init];
	
	[NSApplication sharedApplication];
	
	[NSApp setMainMenu:[[NSMenu alloc] init]];
	
	capp = [[ClaroApplication alloc] init];
	[NSApp setDelegate: capp];
	
	
	ProcessSerialNumber myProc, frProc;
	Boolean sameProc;

	if ( GetFrontProcess( &frProc ) == noErr && GetCurrentProcess( &myProc ) == noErr )
	{
		if ( SameProcess( &frProc, &myProc, &sameProc ) == noErr && !sameProc )
		{
			CPSEnableForegroundOperation( &myProc );
		}
		
		SetFrontProcess( &myProc );
	}
}

/* called each loop, use to process events from the platform */
void cgraphics_check_events( )
{
	/* with cocoa we crash the system without sleeping, it seems.. not enough events?:) */
	usleep( 5000 );
	
	[capp runOnce];
#if 0
	EventRef theEvent;
	EventTargetRef theTarget;
	
	/* run the event loop iteration */
	
	theTarget = GetEventDispatcherTarget();

	while ( ReceiveNextEvent( 0, 0, kEventDurationNoWait, true, &theEvent ) == noErr )
	{
		SendEventToEventTarget( theEvent, theTarget );
		ReleaseEvent(theEvent);
	}
#endif
}

/* A sane NSView: flipped to behave like everyone else
 *   - being different is bad in this case :) */
@implementation SaneView
- (BOOL)isFlipped
{
	return YES;
}
@end

NSView *cgraphics_get_native_parent( widget_t *widget )
{
	widget_t *wp = (widget_t *)widget->object.parent;
	NSWindow *parent;
	
	if ( !strcmp( wp->object.type, "claro.graphics.widgets.window" ) )
	{
		parent = (NSWindow *)widget_get_container( wp );
		return [parent contentView];
	}
	else
	{
		return (NSView *)widget_get_container( wp );
	}
}

/* General widgets */

#include "cocoa/RBSplitView/RBSplitView.h"
#include "cocoa/RBSplitView/RBSplitSubview.h"

void cgraphics_widget_show( widget_t *widget )
{
	NSControl *ctl = (NSControl *)widget->native;
	NSView *osparent;
	//NSView *parent = cgraphics_get_native_parent( widget );
	
	if ( !strcmp( widget->object.type, "claro.graphics.widgets.toolbar" ) )
		return;
	
	if ( !strcmp( widget->object.type, "claro.graphics.widgets.menubar" ) )
		return;
	
	if ( !strcmp( widget->object.type, "claro.graphics.widgets.menu" ) )
		return;
	
	osparent = [ctl superview];
	
	if ( [osparent isKindOfClass:[RBSplitSubview class]] )
		ctl = osparent;
	
	//[parent addSubview: ctl];
	[ctl setHidden: NO];
}

void cgraphics_widget_hide( widget_t *widget )
{
	NSControl *ctl = (NSControl *)widget->native;
	NSView *parent = [ctl superview];
	
	if ( [parent isKindOfClass:[RBSplitSubview class]] )
		ctl = parent;
	
	/*[ctl retain];
	[ctl removeFromSuperview];*/
	[ctl setHidden: YES];
}

void cgraphics_widget_focus( widget_t *widget )
{
	NSControl *ctl = (NSControl *)widget->native;
	NSView *parent = cgraphics_get_native_parent( widget );
	NSWindow *window = [parent window];
	
	[window makeFirstResponder: ctl];
}

void cgraphics_widget_enable( widget_t *widget )
{
	NSControl *ctl = (NSControl *)widget->native;
	
	[ctl setEnabled:YES];
}

void cgraphics_widget_disable( widget_t *widget )
{
	NSControl *ctl = (NSControl *)widget->native;
	
	[ctl setEnabled:NO];
}

void cgraphics_widget_close( widget_t *widget )
{
	NSWindow *ctl = (NSControl *)widget->native;
	
	if ( !strcmp( OBJECT(widget)->type, "claro.graphics.widgets.window" ) )
	{
		cgraphics_window_close( widget );
		return;
	}
}

widget_t *cgraphics_get_widget_window( widget_t *w )
{
	object_t *o ;
	
	o = (object_t *)w;
	
	if ( !strcmp( o->type, "claro.graphics.widgets.window" ) )
		return w;
	
	if ( o->parent == 0 )
		return 0;
	
	return cgraphics_get_widget_window( (widget_t *)o->parent );
}

void cgraphics_post_init( widget_t *widget )
{
	
}

/* update bounds (global widget function */

void cgraphics_update_bounds( object_t *obj )
{
	widget_t *widget = (widget_t *)obj;
	object_t *parent = obj->parent;
	NSView *ctl = (NSView *)widget->native;
	//NSRect oldframe = [ctl frame];
	NSRect frame;
	
	if ( widget->size_req == NULL )
		return;
	
	if ( !( widget->size_flags & cSizeRequestChanged ) )
		return;
	
	if ( !strcmp( widget->object.type, "claro.graphics.widgets.window" ) ||
		 !strcmp( widget->object.type, "claro.graphics.widgets.workspace.window" ) ||
		 !strcmp( widget->object.type, "claro.graphics.widgets.menu" ) )
		return;
	
	if ( parent && !strcmp( parent->type, "claro.graphics.widgets.splitter" ) )
		return;
	
	frame.origin.x = widget->size_req->x;
	frame.origin.y = widget->size_req->y;
	frame.size.width = widget->size_req->w;
	frame.size.height = widget->size_req->h;
	
	[[ctl superview] setNeedsDisplayInRect:[ctl frame]];
	[ctl setFrame:frame];
	[ctl claroResize:frame];
	[ctl setNeedsDisplay:YES];
}

void cgraphics_widget_screen_offset( widget_t *object, int *dx, int *dy )
{
	
}

void cgraphics_widget_set_cursor( widget_t *widget, int cursor )
{

}

/* end of platform functions */
