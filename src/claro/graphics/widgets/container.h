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


#ifndef _CLARO_GRAPHICS_WIDGETS_CONTAINER_H
#define _CLARO_GRAPHICS_WIDGETS_CONTAINER_H
/** \defgroup container_widget_t Container Widget
 * \brief The Container Widget
 * @{
 */

#define assert_only_container_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.container" )
#define assert_valid_container_widget(o,n)	assert_not_null( o, n, "Container Widget" ); \
						assert_only_container_widget( o, n )

typedef struct container_widget_ container_widget_t;

struct container_widget_
{
	widget_t widget;
};

/* functions */

/**
 * \brief Creates a Container widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Container widget object.
 */
CLFEXP object_t *container_widget_create( object_t *parent, bounds_t *bounds, int flags );

/*\@}*/
#endif

