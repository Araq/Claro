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

/* Progressbar */
void cgraphics_progress_widget_create(widget_t *widget)
{
	widget->native = gtk_progress_bar_new();	
	assert(widget->native != NULL && "failed to create GTK progress_bar");

	cgraphics_progress_set_orientation(widget, widget->flags);

	cgraphics_widget_create( widget );
}

void cgraphics_progress_set_level(widget_t *widget, double percentage)
{
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widget->native), percentage);
}

void cgraphics_progress_set_orientation(widget_t *widget, int flags)
{
	// TODO: merge the new orient flags into the current flags
	if(flags & cProgressRightLeft) {
		gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(widget->native), GTK_PROGRESS_RIGHT_TO_LEFT);
	} else if(flags & cProgressBottomTop) {
		gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(widget->native), GTK_PROGRESS_BOTTOM_TO_TOP);
	} else if(flags & cProgressTopBottom) {
		gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(widget->native), GTK_PROGRESS_TOP_TO_BOTTOM);
	} else {
		gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(widget->native), GTK_PROGRESS_LEFT_TO_RIGHT);
	}
}
