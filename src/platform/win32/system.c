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

void cgraphics_open_browser( widget_t *w, char *url )
{
	ShellExecute( NULL, "open", url, NULL, NULL, SW_SHOWNORMAL );
}

int cgraphics_clipboard_set_text( widget_t *w, char *text )
{
	HGLOBAL hglbCopy;
	LPTSTR lptstrCopy;
	
	if ( !OpenClipboard( w->native ) )
		return 0;
	
	EmptyClipboard( );
	
	hglbCopy = GlobalAlloc( GMEM_MOVEABLE, strlen( text ) + 1 ); 
	
	if ( hglbCopy == NULL )
	{
		CloseClipboard(); 
		return 0;
	}
	
	lptstrCopy = GlobalLock( hglbCopy );
	memcpy( lptstrCopy, text, strlen( text ) + 1 );
	GlobalUnlock( hglbCopy );
	
	SetClipboardData( CF_TEXT, hglbCopy );
	
	CloseClipboard( );
	
	return 1;
}
