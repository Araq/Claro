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

/* Splitter */

void cgraphics_splitter_widget_create( widget_t *widget )
{
	widget->ndata = (void *)0; // (ab)using the ndata pointer for a count
	
	if ( widget->flags & cSplitterVertical )
		widget->native = gtk_vpaned_new( );
	else
		widget->native = gtk_hpaned_new( );
		
	cgraphics_widget_create( widget );
}

void cgraphics_splitter_updated( widget_t *widget, int child )
{
	splitter_widget_t *sw = (splitter_widget_t *)widget;
	GtkPaned *paned = GTK_PANED( widget->native );
	
	//sw->children[child].flex
	//printf( "%p\n", sw->children[child].w );
	//g_object_set( G_OBJECT(sw->children[child].w->native), "resize", sw->children[child].flex, NULL );
	//GTK_WIDGET(sw->children[child].w->native)->resize = sw->children[child].flex;
	
	if ( child == 0 )
	{
		paned->child1_resize = sw->children[child].flex;
		paned->child1_shrink = FALSE;
	}
	else
	{
		paned->child2_resize = sw->children[child].flex;
		paned->child2_shrink = FALSE;
	}
	
	if ( sw->children[child].size > -1 )
	{
		if ( widget->flags & cSplitterVertical )
		{
			gtk_widget_set_size_request( GTK_WIDGET(sw->children[child].w->native), -1, sw->children[child].size );
			if ( child == 0 )
				gtk_paned_set_position( paned, sw->children[child].size );
			else
			{
				GtkRequisition r;
				gtk_widget_size_request( paned, &r );
				gtk_paned_set_position( paned, r.height - sw->children[child].size );
			}
		}
		else
		{
			gtk_widget_set_size_request( GTK_WIDGET(sw->children[child].w->native), sw->children[child].size, -1 );
			if ( child == 0 )
				gtk_paned_set_position( paned, sw->children[child].size );
			else
			{
				GtkRequisition r;
				gtk_widget_size_request( paned, &r );
				gtk_paned_set_position( paned, r.width - sw->children[child].size );
			}
		}
	}
}
