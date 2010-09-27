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

@interface ClaroWorkspace : NSTabView
{
	widget_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroMove:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;
@end

@interface ClaroWorkspaceWindow : SaneView
{
@public
	NSTabViewItem *tab;
	widget_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget withParent:(ClaroWorkspace *)parent;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroMove:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

- (void)updateTitle;
@end

object_t *cgraphics_workspace_get_active( widget_t *widget );

@implementation ClaroWorkspace

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

- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem
{
	object_t *o = cgraphics_workspace_get_active( cw );
	event_send( OBJECT(o), "focus", "" );
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
	
	[self setDelegate: self];
}

@end

void cgraphics_workspace_widget_create( widget_t *widget )
{
	ClaroWorkspace *v;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	v = [[ClaroWorkspace alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[v setClaroWidget:widget];
	[parent addSubview: v];
	
	widget->native = (NSControl *)v;
}

void cgraphics_workspace_set_active( widget_t *widget, object_t *child )
{
	ClaroWorkspace *ws = (ClaroWorkspace *)widget->native;
	ClaroWorkspaceWindow *wsw = (ClaroWorkspaceWindow *)WIDGET(child)->native;
	[ws selectTabViewItem: wsw->tab];
}

object_t *cgraphics_workspace_get_active( widget_t *widget )
{
	ClaroWorkspace *ws = (ClaroWorkspace *)widget->native;
	ClaroWorkspaceWindow *wsw;
	NSTabViewItem *tvi;
	
	tvi = [ws selectedTabViewItem];
	
	if ( tvi == nil )
		return NULL;
	
	wsw = (ClaroWorkspaceWindow *)[tvi view];
	
	return OBJECT(wsw->cw);
}

void cgraphics_workspace_cascade( widget_t *widget )
{
	
}

void cgraphics_workspace_tile( widget_t *widget, int dir )
{
	
}

@implementation ClaroWorkspaceWindow

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( OBJECT(cw), frame.size.width, frame.size.height, 1 );
	widget_set_content_size( OBJECT(cw), frame.size.width, frame.size.height, 1 );
}

- (void)claroMove:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_position( OBJECT(cw), frame.origin.x, frame.origin.y, 1 );
	widget_set_content_position( OBJECT(cw), 0, 0, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( OBJECT(cw) );
}

- (void)setClaroWidget:(widget_t *)widget withParent:(ClaroWorkspace *)parent
{
	cw = widget;
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSViewFrameDidChangeNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroMove:) name:NSViewFrameDidChangeNotification
		object:self];
	
	[self setBackgroundColor: [NSColor windowBackgroundColor]];
	
	tab = [[NSTabViewItem alloc] initWithIdentifier: 0];
	
	[self updateTitle];
	[tab setView:self];
	
	[parent addTabViewItem:tab];
}

- (void)updateTitle
{
	workspace_window_widget_t *ww = (workspace_window_widget_t *)cw;
	[tab setLabel:[[NSString alloc] initWithCString:ww->title encoding:NSUTF8StringEncoding]];
}

@end

void cgraphics_workspace_window_widget_create( widget_t *widget )
{
	ClaroWorkspaceWindow *v;
	ClaroWorkspace *parent = (ClaroWorkspace *)cgraphics_get_native_parent( widget );
	
	v = [[ClaroWorkspaceWindow alloc]
		initWithFrame: NSMakeRect( 0, 0, 100, 100 )
		];
	
	[v setClaroWidget:widget withParent:parent];
	//[parent addSubview: v];
	//	[parent insertTabViewItem:v atIndex: 0];
	
	widget->native = (NSControl *)v;
//	widget->container = [v view];
}

void cgraphics_workspace_window_update_title( widget_t *widget )
{
	ClaroWorkspaceWindow *v = (ClaroWorkspaceWindow *)widget->native;
	[v updateTitle];
}

void cgraphics_workspace_window_show( widget_t *widget )
{
	//ClaroWorkspaceWindow *v = (ClaroWorkspaceWindow *)widget->native;
	
}

void cgraphics_workspace_window_hide( widget_t *widget )
{
	//ClaroWorkspaceWindow *v = (ClaroWorkspaceWindow *)widget->native;
	
}

void cgraphics_workspace_window_restore( widget_t *widget )
{
	
}

void cgraphics_workspace_window_minimise( widget_t *widget )
{
	
}

void cgraphics_workspace_window_maximise( widget_t *widget )
{
	
}

void cgraphics_workspace_window_update_icon( widget_t *w )
{
	
}
