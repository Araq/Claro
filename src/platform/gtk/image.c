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

/* Image */
void cgraphics_image_widget_create( widget_t *widget )
{
	widget->native = gtk_image_new_from_pixbuf( NULL );
	
	cgraphics_widget_create( widget );
}

void cgraphics_image_set_image( widget_t *widget )
{
	image_widget_t *iw = (image_widget_t *)widget;
	gtk_image_set_from_pixbuf( widget->native, (GdkPixbuf *)iw->src->native );
}
