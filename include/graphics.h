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


#ifndef _CLARO_GRAPHICS_H
#define _CLARO_GRAPHICS_H

#ifndef __cplusplus
#ifdef _WIN32
typedef unsigned short bool;
#endif
#else
extern "C" {
#endif


/* These must come first or the widgets won't know about base types like object_t and layout_t */
#include "base.h"

/* Fonts */
#include "font.h"

#include "widget.h"
#include "layout.h"

/* Basic image handling */
#include "image.h"

/* Widgets must go here not above this */
#include "widgets/list.h"

#include "widgets/window.h"
#include "widgets/menubar.h"
#include "widgets/toolbar.h"
#include "widgets/textbox.h"
#include "widgets/button.h"
#include "widgets/canvas.h"
#include "widgets/splitter.h"
#include "widgets/container.h"
#include "widgets/textarea.h"
#include "widgets/checkbox.h"
#include "widgets/dialog.h"
#include "widgets/label.h"
#include "widgets/progress.h"
#include "widgets/radio.h"
#include "widgets/splitter.h"
#include "widgets/statusbar.h"
#include "widgets/frame.h"
#include "widgets/image.h"

#include "widgets/workspace.h"
#include "widgets/scrollbar.h"
#include "widgets/menu.h"

#include "widgets/listview.h"
#include "widgets/treeview.h"
#include "widgets/listbox.h"
#include "widgets/combo.h"

#include "widgets/opengl.h"

#include "widgets/font_dialog.h"

#include "system.h"

#include "notify.h"

//added by Cody,2/9/06
#include "widgets/stock.h"

CLFEXP void claro_graphics_init( );

/* Standard includes always needed */
#include <assert.h>


#ifdef _MAC
#	include "platform/macosx_cocoa.h"
#endif

#ifdef _NIX
#	include "platform/gtk.h"
#endif

#ifdef _WIN32
#	include "platform/win32.h"
#endif

#ifdef CLARO_SOURCE
#undef CLARO_APPLICATION
#define CLARO_APPLICATION "claro.graphics"
#endif


#ifdef __cplusplus
}
#endif

#endif

