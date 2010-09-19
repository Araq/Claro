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


#include <claro/graphics.h>
#include <assert.h>
#include <claro/graphics/platform.h>

void *cgraphics_font_from_font( font_t *font )
{
	return NULL;
}

void cgraphics_widget_set_font( widget_t *widget, font_t *font )
{
	
}

int cgraphics_widget_font_string_width( widget_t *w, char *text, int chars )
{
	PangoLayout *layout = gtk_widget_create_pango_layout( w->native, text );
	int width, height;
	
	pango_layout_get_pixel_size( layout, &width, &height );
	
	g_object_unref( G_OBJECT(layout) );
	
	return width;
}
