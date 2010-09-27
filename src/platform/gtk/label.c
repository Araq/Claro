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

/* Label */

void cgraphics_label_widget_create(widget_t *widget)
{
	widget->native = gtk_label_new(((label_widget_t *)widget)->text);

	cgraphics_widget_create( widget );
	//cgraphics_label_update_justify( widget );
}

void cgraphics_label_update_text(widget_t *widget)
{
	label_widget_t *w = (label_widget_t *)widget;
	gtk_label_set_text( GTK_LABEL(w->widget.native), w->text );
}

void cgraphics_label_update_justify(widget_t *widget)
{
	GtkJustification just;
	
	switch(widget->flags) {
		case cLabelRight:
			just = GTK_JUSTIFY_RIGHT;
			gtk_misc_set_alignment( GTK_MISC(widget->native), 1, 0 );
			break;
		case cLabelCenter:
			just = GTK_JUSTIFY_CENTER;
			gtk_misc_set_alignment( GTK_MISC(widget->native), 0.5, 0 );
			break;
		case cLabelFill:
			just = GTK_JUSTIFY_FILL;
			break;
		case cLabelLeft:
		default:
			just = GTK_JUSTIFY_LEFT;
			gtk_misc_set_alignment( GTK_MISC(widget->native), 0, 0 );
			break;
	}
	
	// gtk_label_set_justify() has no effect on labels containing only a single line.
	// gtk_misc_set_alignment() handles this above.
	gtk_label_set_justify(GTK_LABEL(widget->native), just);
}
