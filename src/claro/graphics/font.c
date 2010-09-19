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
#include <claro/graphics/platform.h>

void widget_set_font( object_t *widget, char *face, int size, int weight, int slant, int decoration )
{
	char *tface;
	widget_t *rwidget = WIDGET(widget);
	
	rwidget->font.used = 2; // mark as changed so font wont be cached
	tface = strdup( face );
	if ( rwidget->font.face != 0 )
		free( rwidget->font.face );
	rwidget->font.face = tface;
	rwidget->font.size = size;
	rwidget->font.weight = weight;
	rwidget->font.slant = slant;
	rwidget->font.decoration = decoration;
	
	cgraphics_widget_set_font( rwidget, &rwidget->font );
}

int widget_font_string_width( object_t *w, char *text, int chars )
{
	return cgraphics_widget_font_string_width( w, text, chars );
}
