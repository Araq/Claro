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

/* Standard includes always needed */
#include <assert.h>



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
#include "graphics/font.h"

#include "graphics/widget.h"
#include "graphics/layout/layout.h"

/* Basic image handling */
#include "graphics/image.h"

/* Widgets must go here not above this */
#include "graphics/widgets/list.h"

#include "graphics/widgets/window.h"
#include "graphics/widgets/menubar.h"
#include "graphics/widgets/toolbar.h"
#include "graphics/widgets/textbox.h"
#include "graphics/widgets/button.h"
#include "graphics/widgets/canvas.h"
#include "graphics/widgets/splitter.h"
#include "graphics/widgets/container.h"
#include "graphics/widgets/textarea.h"
#include "graphics/widgets/checkbox.h"
#include "graphics/widgets/dialog.h"
#include "graphics/widgets/label.h"
#include "graphics/widgets/progress.h"
#include "graphics/widgets/radio.h"
#include "graphics/widgets/splitter.h"
#include "graphics/widgets/statusbar.h"
#include "graphics/widgets/frame.h"
#include "graphics/widgets/image.h"

#include "graphics/widgets/workspace.h"
#include "graphics/widgets/scrollbar.h"
#include "graphics/widgets/menu.h"

#include "graphics/widgets/listview.h"
#include "graphics/widgets/treeview.h"
#include "graphics/widgets/listbox.h"
#include "graphics/widgets/combo.h"

#include "graphics/widgets/opengl.h"

#include "graphics/widgets/font_dialog.h"

#include "graphics/system.h"

#include "graphics/notify.h"

//added by Cody,2/9/06
#include "graphics/widgets/stock.h"

CLFEXP void claro_graphics_init( );

#ifdef __cplusplus
}
#endif

#endif


