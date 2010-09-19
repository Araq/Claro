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

void *cgraphics_font_from_font( font_t *font )
{
	
}

void cgraphics_widget_set_font( widget_t *widget, font_t *font )
{

}

int cgraphics_widget_font_string_width( widget_t *w, char *text, int chars )
{
	NSControl *ctl = (NSControl *)w->native;
	NSFont *fnt;
	
	if ( !strcmp( OBJECT(w)->type, "claro.graphics.widgets.window" ) )
		fnt = [NSFont systemFontOfSize:[NSFont systemFontSizeForControlSize: NSRegularControlSize]];
	else
		fnt = [ctl font];
	
	char tmp[chars+1];
	
	strncpy( tmp, text, chars );
	tmp[chars] = 0;
		
	NSString *str = [NSString stringWithCString:tmp];
	NSDictionary *attrs = [NSDictionary dictionaryWithObjectsAndKeys:
					fnt, NSFontAttributeName, nil];
	NSSize sz = [str sizeWithAttributes:attrs];
	
	[attrs release];
	
	return (int)sz.width;
}
