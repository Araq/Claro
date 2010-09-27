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


#include "graphics.h"
#include <assert.h>
#include "platform.h"

void cgraphics_open_browser( widget_t *w, char *url )
{
	char *browser, tmp[1024];
	int a, b, c;
	
	browser = g_find_program_in_path( "gnome-open" );
	if ( !browser ) /* if gnome-open doesn't exist, fall back on firefox */
		browser = g_find_program_in_path( "firefox" );
	
	if ( !browser ) /* if no gnome-open or firefox, bail out. */
		return;
	
	strcpy( tmp, browser );
	strcat( tmp, " \"" );
	c = strlen(tmp);
	for ( a = c, b = 0; a < 1023 && url[b] != 0; a++, b++ )
	{
		switch ( url[b] )
		{
			case ' ':
				strcat( tmp, "%20" );
				a += 2;
				break;
			case '"':
				strcat( tmp, "%22" );
				a += 2;
				break;
			case '\'':
				strcat( tmp, "%27" );
				a += 2;
				break;
			case '\\':
				strcat( tmp, "%5C" );
				a += 2;
				break;
			default:
				tmp[a] = url[b];
				tmp[a+1] = 0;
				break;
		}
	}
	tmp[a] = 0;
	strcat( tmp, "\"" );
	
	system( tmp );
	
	g_free( browser );
}

int cgraphics_clipboard_set_text( widget_t *w, char *text )
{
	GtkClipboard *cb = gtk_clipboard_get( GDK_SELECTION_CLIPBOARD );
	gtk_clipboard_set_text( cb, text, strlen( text ) );
	return 1;
}

