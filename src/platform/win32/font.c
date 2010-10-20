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


#include "platform/win32.h"

extern HFONT nicerfont;

void *cgraphics_font_from_font( font_t *font )
{
	HFONT f;
	int w = FW_NORMAL;
	
	if ( font->used == 1 )
		return font->native;
	
	if ( font->weight == CTEXT_WEIGHT_BOLD )
		w = FW_BOLD;
	
	f = CreateFont( font->size, 0, 0, 0, w, (font->slant==CTEXT_SLANT_ITALIC), (font->decoration&CTEXT_EXTRA_UNDERLINE), 0, DEFAULT_CHARSET, 
							OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
							ANTIALIASED_QUALITY | CLEARTYPE_QUALITY, 
							DEFAULT_PITCH | FF_DONTCARE, font->face );
	
	return f;
}

void cgraphics_widget_set_font( widget_t *widget, font_t *font )
{
	HFONT f = (HFONT)cgraphics_font_from_font( font );
	
	SendMessage( widget->native, WM_SETFONT, (size_t)f, true );
	
	if ( f != widget->font.native && widget->font.native != 0 && widget->font.native != nicerfont )
		DeleteObject( widget->font.native );
	
	widget->font.used = 1;
	widget->font.native = f;
}

int cgraphics_widget_font_string_width( widget_t *w, char *text, int chars )
{
	SIZE s;
	HDC dc;
	
	dc = GetDC( w->native );
	
	SelectObject( dc, w->font.native );
	GetTextExtentPoint32( dc, text, chars, &s );
	
	ReleaseDC( w->native, dc ); 
	
	return s.cx;
}
