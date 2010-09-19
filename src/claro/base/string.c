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


#include <claro/base.h>

/* FIXME: more could be copied over from libatheme, but the formatting and such
    does need to be cleaned up, so I'm leaving it for later  --Theo */

/* copy at most len-1 characters from a string to a buffer, NULL terminate */
char *strscpy( char *d, const char *s, size_t len )
{
	char *d_orig = d;
	
	if ( !len )
		return d;
	
	while ( --len && (*d++ = *s++) );
	*d = 0;
	
	return d_orig;
}

