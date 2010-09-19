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

/* Button */

gint cgraphics_font_dialog_response_handler( GtkWidget *widget, gint resp, widget_t *w )
{
	int ok = 0;
	
	if ( resp == GTK_RESPONSE_OK )
		ok = 1;
	
	event_send( OBJECT(w), "response", "i", ok );
	
	return 1;
}

void cgraphics_font_dialog_widget_create( widget_t *widget )
{
	widget->native = gtk_font_selection_dialog_new( "Select Font" );
	gtk_window_set_modal( widget->native, TRUE );
	
	g_signal_connect( G_OBJECT(widget->native), "response", G_CALLBACK(cgraphics_font_dialog_response_handler), widget );
}

void cgraphics_font_dialog_update_from_selection( widget_t *widget )
{
	char name[1024];
	font_dialog_widget_t *fdw = (font_dialog_widget_t *)widget;
	
	sprintf( name, "%s %s%s%s%d", fdw->selected.face, (fdw->selected.weight==cFontWeightBold?"Bold ":""), (fdw->selected.slant==cFontSlantItalic?"Italic ":""), (fdw->selected.decoration&cFontDecorationUnderline?"Underline ":""), fdw->selected.size ); // (int)((double)fdw->selected.size * (72.0 / 96.0)) );
	
	gtk_font_selection_dialog_set_font_name( GTK_FONT_SELECTION_DIALOG(widget->native), name );
}

void cgraphics_font_dialog_update_to_selection( widget_t *widget )
{
	font_dialog_widget_t *fdw = (font_dialog_widget_t *)widget;
	PangoFontDescription *pfd;
	gchar *str;
	const char *tmp;
	
	str = gtk_font_selection_dialog_get_font_name( GTK_FONT_SELECTION_DIALOG(widget->native) );
	
	if ( str == NULL )
		return;
	
	pfd = pango_font_description_from_string( str );
	
	tmp = pango_font_description_get_family( pfd );
	if ( tmp != NULL )
	{
		if ( fdw->selected.face != NULL )
			free( fdw->selected.face );
		
		fdw->selected.face = strdup( tmp );
	}
	
	fdw->selected.slant = cFontSlantNormal;
	if ( pango_font_description_get_style( pfd ) == PANGO_STYLE_ITALIC )
		fdw->selected.slant = cFontSlantItalic;
	
	fdw->selected.weight = cFontWeightNormal;
	if ( pango_font_description_get_weight( pfd ) >= PANGO_WEIGHT_BOLD )
		fdw->selected.weight = cFontWeightBold;
	
	fdw->selected.size = pango_font_description_get_size( pfd );
	
	if ( !pango_font_description_get_size_is_absolute( pfd ) )
	{
		fdw->selected.size /= PANGO_SCALE;
	}
	
	//fdw->selected.size = (double)fdw->selected.size * (96.0 / 72.0);
	
	pango_font_description_free( pfd );
	
	g_free( str );
}
