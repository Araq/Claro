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
#include "platform.h"

void claro_graphics_loop( object_t *object, event_t *event );

/* Initialise claro.graphics */
void claro_graphics_init( )
{
	object_addhandler( claro, "mainloop", claro_graphics_loop );
	
	// these bounds are mainly for splitters.. a default size is needed to stop issues
	no_bounds.x = no_bounds.y = 0;
	no_bounds.w = no_bounds.h = 50;
	no_bounds.owner = claro;
	
	cgraphics_init( );

	claro_stock_init();
}

/* claro.graphics loop itteration */
void claro_graphics_loop( object_t *object, event_t *event )
{
	cgraphics_check_events( );
}
