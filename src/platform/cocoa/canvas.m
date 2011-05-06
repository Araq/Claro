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
#ifndef NO_CAIRO
#include <cairo-quartz.h>
#endif
#include "platform/macosx_cocoa.h"

/* Canvas */

/* ClaroCanvas (subclassed from NSView) */
@interface ClaroCanvas : NSView
{
	widget_t *cw;
	canvas_widget_t *cvsw;
	
	NSGraphicsContext *cg;
	CGContextRef cgr;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

- (void)mouseDown:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)rightMouseDown:(NSEvent *)event;
- (void)rightMouseUp:(NSEvent *)event;
- (void)otherMouseDown:(NSEvent *)event;
- (void)otherMouseUp:(NSEvent *)event;
- (void)otherMouseDragged:(NSEvent *)event;
- (void)mouseMoved:(NSEvent *)event;

- (NSPoint)getLocalMousePosition:(NSEvent *)event;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroMove:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

/* override this to make it normal */
- (BOOL)isFlipped;

/* sublcassed functions */
- (void)drawRect:(NSRect)aRect;
@end

@implementation ClaroCanvas

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );

#ifndef NO_CAIRO
	cairo_surface_destroy( cvsw->surface );
	cvsw->surface = cairo_quartz_surface_create( cgr, true, cw->size_req->w, cw->size_req->h );
#endif
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


#define macroMouseEvent(e) NSPoint pt = [self getLocalMousePosition: event]; \
event_send( OBJECT(cw), e, "ii", (int)pt.x, (int)pt.y );

- (void)rightMouseDown:(NSEvent *)event
{
	macroMouseEvent("right_clicked");
}

- (void)rightMouseUp:(NSEvent *)event
{
	macroMouseEvent("right_released");
}

- (void)otherMouseDown:(NSEvent *)event
{
	macroMouseEvent("middle_clicked");
}

- (void)otherMouseUp:(NSEvent *)event
{
	macroMouseEvent("middle_released");
}

- (void)mouseDown:(NSEvent *)event
{
	macroMouseEvent("clicked");
}

- (void)mouseUp:(NSEvent *)event
{
	macroMouseEvent("released");
}

- (NSPoint)getLocalMousePosition:(NSEvent *)event
{
	NSPoint event_location = [event locationInWindow];
	NSPoint local_point = [self convertPoint:event_location fromView:nil];
	NSRect frame = [self frame];
	
	local_point.y = frame.size.height - local_point.y;
	
	return local_point;
}

- (void)mouseMoved:(NSEvent *)event
{
	macroMouseEvent("mouse_moved");
	
	[super mouseMoved: event];
}

- (void)otherMouseDragged:(NSEvent *)event
{
	macroMouseEvent("mouse_moved");
	
	[super otherMouseDragged: event];
}

- (void)scrollWheel:(NSEvent *)event
{
	NSPoint pt = [self getLocalMousePosition: event];
	double dx=[event deltaX], dy=[event deltaY], dz=[event deltaZ];
	
	event_send( OBJECT(cw), "scroll_wheel", "iiddd", (int)pt.x, (int)pt.y, dx, dy, dz );
}

- (void)drawRect:(NSRect)aRect
{
	CGContextRef context = [[NSGraphicsContext currentContext] graphicsPort];
	widget_t *widget = cw;
	widget_t *window = cgraphics_get_widget_window( widget );
	int window_height = window->size.h;
	int widget_bottom, widget_height;
	
#ifndef NO_CAIRO
	if ( cvsw->surfdata != context || cvsw->surface == NULL )
	{
		// context has changed.
		
		// free existing surface, if set
		if ( cvsw->surface != NULL)
			cairo_surface_destroy( cvsw->surface );
		
		// create a new surface with this context
		cvsw->surface = cairo_quartz_surface_create( context, cw->size_req->w, cw->size_req->h, true );
	}
#endif
	
	cvsw->surfdata = context;
	widget_bottom = window_height - widget->size_req->y - widget->size_req->h;
	widget_height = widget->size_req->h;
	
	if ( widget_height + widget_bottom > window_height )
		widget_height = window_height - widget_bottom;
	
	/* FIXME: make this aRect :/ */
	//CGContextClipToRect( context, CGRectMake( widget->size_req->x, widget_bottom, widget->size_req->w, widget->size_req->h ) );
	//CGContextClipToRect( context, CGRectMake( widget->size_req->x, widget_bottom, widget->size_req->w, widget->size_req->h ) );
	
	//printf( "TranslateCTM %d,%d\n", widget->size_req->x, widget_bottom );
	//CGContextTranslateCTM( context, widget->size_req->x, widget_bottom );
	//CGContextTranslateCTM( context, widget->size_req->x, widget_bottom );
	
	/****/
#ifndef NO_CAIRO
	cvsw->cr = cairo_create( cvsw->surface );
	
	/*cairo_rectangle( cvsw->cr, aRect.origin.x, aRect.origin.y, aRect.size.width, aRect.size.height );
	cairo_clip( cvsw->cr );*/
	event_send( OBJECT(cw), "redraw", "p", cvsw->cr );	
	cairo_destroy( cvsw->cr );
	
	/****/
      	cairo_surface_flush( cvsw->surface );
#endif
	CGContextFlush( context );
}


#ifndef NO_CAIRO
cairo_surface_t *cairo_quartz_surface_create(CGContextRef context,
	cairo_bool_t flipped, int width, int height);
#endif


- (void)setClaroWidget:(widget_t *)widget
{
	cw = widget;
	cvsw = (canvas_widget_t *)cw;
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSWindowDidResizeNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroMove:) name:NSViewFrameDidChangeNotification
		object:self];
	
	cg = [NSGraphicsContext graphicsContextWithWindow:[self window]];
	
	cvsw->surfdata = 0;
	//cgr = (CGContextRef)[cg graphicsPort];
/*#ifndef NO_CAIRO
	cvsw->surface = cairo_quartz_surface_create( cgr, widget->size_req->w, widget->size_req->h );
#endif*/
}

- (BOOL)isFlipped
{
	return YES;
}

@end

void cgraphics_canvas_widget_create( widget_t *widget )
{
	ClaroCanvas *cv;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	cv = [[ClaroCanvas alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[parent addSubview: cv];
	[cv setClaroWidget:widget];
	
	widget->native = (NSControl *)cv;
}

void cgraphics_canvas_redraw( canvas_widget_t *w )
{
	ClaroCanvas *cv = (ClaroCanvas *)w->widget.native;
	
	[cv setNeedsDisplay:YES];
	[cv displayIfNeeded];
}

/* claro native text functions */

void cgraphics_canvas_set_text_color( widget_t *widget, double r, double g, double b, double a )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	CGContextSetRGBFillColor( cw->surfdata, r, g, b, a );
}

void cgraphics_canvas_set_text_bgcolor( widget_t *widget, double r, double g, double b, double a )
{
	/* FIXME: save this and draw the rect :) */
}

void cgraphics_canvas_set_text_font( widget_t *widget, char *face, int size, int weight, int slant )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	
	ATSUStyle style;
	OSStatus status = ATSUCreateStyle( &style );
	ATSUFontID fontID;
	Boolean isBold=false, isItalic=false;
	
	isBold = (weight==cFontWeightBold);
	isItalic = (slant==cFontSlantItalic);
	
	status = ATSUFindFontFromName( face, strlen(face),
							   kFontFullName,
							   kFontNoPlatformCode,
							   kFontRomanScript,
							   kFontNoLanguageCode, &fontID );

	ATSUAttributeTag styleTags[] = { kATSUQDItalicTag, kATSUQDBoldfaceTag, kATSUFontTag };
	ATSUAttributeValuePtr styleValues[] = { &isItalic, &isBold, &fontID };
	ByteCount styleSizes[] = { sizeof(Boolean), sizeof(Boolean), sizeof(ATSUFontID) };

	status = ATSUSetAttributes( style,
							sizeof(styleTags) / sizeof(styleTags[0]),
							styleTags, styleSizes, styleValues );
	
	if ( cw->fontdata )
		ATSUDisposeStyle( cw->fontdata );
	cw->fontdata = style;
	
	cw->font_height = size;
	
	//CGContextSelectFont( cw->surfdata, face, size, kCGEncodingMacRoman );
}

ATSUTextLayout cgraphics_canvas_internal_create_text_layout( canvas_widget_t *cw, const char *text, int len, UniChar *textu )
{
	ATSUTextLayout layout;
	ATSUStyle style = (ATSUStyle) cw->fontdata;
	int a;
	
	for ( a = 0; a < len + 1; a++ )
		textu[a] = text[a];
	
	ATSUCreateTextLayoutWithTextPtr(
		 textu,
		 0,		  // offset
		 len,	 // length
		 len,	 // total length
		 1,		  // styleRunCount
		 &len,	// length of style run
		 &style, 
		 &layout );
	
	ATSUAttributeTag theTags[] = { kATSUCGContextTag };
	ByteCount theSizes[] = { sizeof(CGContextRef) };
	ATSUAttributeValuePtr theValues[] = { &cw->surfdata };
	ATSUSetLayoutControls( layout, 1, theTags, theSizes, theValues );
	
	return layout;
}

int cgraphics_canvas_text_width( widget_t *widget, const char *text, int len )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	UniChar textu[len+1];
	ATSUTextLayout layout;
	Rect r;
	
	layout = cgraphics_canvas_internal_create_text_layout( cw, text, len, &textu );
	
	ATSUMeasureTextImage( layout, 0, len, 0, 0, &r );
	
	ATSUDisposeTextLayout( layout );
	
	return r.right;
}

int cgraphics_canvas_text_box_width( widget_t *widget, const char *text, int len )
{
	return cgraphics_canvas_text_width( widget, text, len );
}

int cgraphics_canvas_text_display_count( widget_t *widget, const char *text, int width )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	int c = strlen( text );
	UniChar textu[c+1];
	ATSUTextLayout layout;
	UniCharArrayOffset os;
	
	layout = cgraphics_canvas_internal_create_text_layout( cw, text, c, &textu );
	
	ATSUBreakLine( layout, 0, width<<16, false, &os );
	
	ATSUDisposeTextLayout( layout );
	
	return os;
}

#define STATE_SAVE CGContextSaveGState( (CGContextRef)((canvas_widget_t *)widget)->surfdata );
#define STATE_FLIP CGContextScaleCTM( (CGContextRef)((canvas_widget_t *)widget)->surfdata, 1, -1 );
#define STATE_TRANSLATE CGContextTranslateCTM( (CGContextRef)((canvas_widget_t *)widget)->surfdata, 0, widget->size_req->h + widget->size_req->y );
#define STATE_RESTORE CGContextRestoreGState( (CGContextRef)((canvas_widget_t *)widget)->surfdata );

void cgraphics_canvas_show_text( widget_t *widget, int x, int y, const char *text, int len )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	UniChar textu[len+1];
	ATSUTextLayout layout;
	
	STATE_SAVE
	
	layout = cgraphics_canvas_internal_create_text_layout( cw, text, len, &textu );
	
	STATE_FLIP
	
	MoveTo( x, -(y + cw->font_height) );
	ATSUDrawText( layout, 0, len, kATSUUseGrafPortPenLoc, kATSUUseGrafPortPenLoc );
	
	ATSUDisposeTextLayout( layout );
	
	STATE_RESTORE
}

void cgraphics_canvas_fill_rect( widget_t *widget, int x, int y, int w, int h, double r, double g, double b, double a )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	CGContextRef context = (CGContextRef)cw->surfdata;
	CGRect rect;
	
	rect.origin.x = x;
	rect.origin.y = y;
	rect.size.width = w;
	rect.size.height = h;
	
	STATE_SAVE
	
	CGContextSaveGState( context );
	CGContextSetRGBFillColor( context, r, g, b, a );
	CGContextFillRect( context, rect );
	CGContextRestoreGState( context );
	
	STATE_RESTORE
}

void cgraphics_canvas_draw_image( widget_t *widget, image_t *image, int x, int y )
{
	canvas_widget_t *cw = (canvas_widget_t *)widget;
	CGContextRef context = (CGContextRef)cw->surfdata;
	CGRect rect;
	NSImage *im = (NSImage *)image->native;
	NSPoint p;
	
	p.x = x;
	p.y = y;
	
	[im setFlipped: YES];
	[im drawAtPoint:p fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1.0];
	[im setFlipped: NO];
}


