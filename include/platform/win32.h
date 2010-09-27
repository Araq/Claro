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


#ifndef _CLARO_GRAPHICS_PLATFORMS_WIN32_H
#define _CLARO_GRAPHICS_PLATFORMS_WIN32_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <windows.h>
#include <commctrl.h>

#include <claro/graphics.h>
#ifndef NO_CAIRO
#include <cairo-win32.h>
#endif

#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY 5
#endif

widget_t *cg_find_by_native( HWND hwnd );

typedef HWND native_widget_t;
//typedef unsigned short bool;

typedef struct
{
	
} native_window_widget_t;

typedef struct
{
	/* */
} native_toolbar_widget_t;

typedef struct
{
	/* */
} native_textbox_widget_t;

typedef struct
{
	/* */
} native_button_widget_t;

typedef struct
{
	HBITMAP bmp;
	HFONT font;
	
	COLORREF fg, bg;
} native_canvas_widget_t;

typedef struct
{
	layout_t *lt;
	int splitter_num;
	int ml_x, ml_y;
	int splitter_gap;
	widget_t *widgets[2];
	int frozen;
} native_splitter_widget_t;

typedef struct
{
	/* */
} native_container_widget_t;

extern int curr_classname_id;
extern HFONT nicerfont;

#ifndef true
#define true 1
#define false 0
#endif

#endif
