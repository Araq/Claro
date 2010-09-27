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


#ifndef _CLARO_GRAPHICS_PLATFORMS_MACOSX_CARBON_H
#define _CLARO_GRAPHICS_PLATFORMS_MACOSX_CARBON_H

#ifdef _PLATFORM_INC
#include <Cocoa/Cocoa.h>

/* A sane NSView: flipped to behave like everyone else
 *   - being different is bad in this case :) */
@interface SaneView : NSClipView
- (BOOL)isFlipped;
@end

NSView *cgraphics_get_native_parent( widget_t *widget );
widget_t *cgraphics_get_widget_window( widget_t *w );

#include "cocoa/window.h"

#endif

#endif
