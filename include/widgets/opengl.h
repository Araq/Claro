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


#ifndef _CLARO_GRAPHICS_WIDGETS_OPENGL_H
#define _CLARO_GRAPHICS_WIDGETS_OPENGL_H
/** \defgroup opengl_widget_t OpenGL Widget
 * \brief The OpenGL Widget
 * @{
 */

#define assert_only_opengl_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.opengl" )
#define assert_valid_opengl_widget(o,n)	assert_not_null( o, n, "OpenGL" ); \
					assert_only_opengl_widget( o, n )

#ifdef CLARO_OPENGL

typedef struct opengl_widget_ opengl_widget_t;

struct opengl_widget_
{
	widget_t widget;
	void *gldata;
};

/* functions */

/**
 * \brief Creates a OpenGL widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new OpenGL widget object.
 */
CLFEXP object_t *opengl_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Flips the front and back buffers
 * 
 * \param widget A valid OpenGL widget object
 */
CLFEXP void opengl_flip( object_t *widget );

/**
 * \brief Activates this OpenGL widget's context
 * 
 * \param widget A valid OpenGL widget object
 */
CLFEXP void opengl_activate( object_t *widget );

#endif

/*\@}*/
#endif
