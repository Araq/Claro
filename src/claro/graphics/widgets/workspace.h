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


#ifndef _CLARO_GRAPHICS_WIDGETS_WORKSPACE_H
#define _CLARO_GRAPHICS_WIDGETS_WORKSPACE_H
/** \defgroup workspace_widget_t Workspace Widget
 * \brief The Workspace Widget
 * @{
 */

#define assert_only_workspace_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.workspace" )
#define assert_valid_workspace_widget(o,n)	assert_not_null( o, n, "Workspace" ); \
						assert_only_workspace_widget( o, n )

#define assert_only_workspace_window_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.workspace.window" )
#define assert_valid_workspace_window_widget(o,n)	assert_not_null( o, n, "Workspace Window" ); \
							assert_only_workspace_window_widget( o, n )

#define cWorkspaceTileHorizontally 0
#define cWorkspaceTileVertically 1

typedef struct workspace_widget_ workspace_widget_t;
typedef struct workspace_window_widget_ workspace_window_widget_t;

struct workspace_widget_
{
	widget_t widget;
};

struct workspace_window_widget_
{
	widget_t widget;
	
	image_t *icon;
	
	char title[512];
	
	workspace_widget_t *workspace;
};

/* functions (workspace) */

/**
 * \brief Creates a Workspace widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Workspace widget object.
 */
CLFEXP object_t *workspace_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets the active (visible) workspace child
 * 
 * \param workspace A valid workspace widget
 * \param child A valid workspace window widget
 */
CLFEXP void workspace_set_active( object_t *workspace, object_t *child );

/**
 * \brief Returns the active (visible) workspace child
 * 
 * \param workspace A valid workspace widget
 * \return The active workspace window widget
 */
CLFEXP object_t *workspace_get_active( object_t *workspace );

/**
 * \brief Cascades all workspace windows
 * 
 * \param workspace A valid workspace widget
 */
CLFEXP void workspace_cascade( object_t *workspace );

/**
 * \brief Tiles all workspace windows
 * 
 * \param workspace A valid workspace widget
 * \param dir The direction to tile child widgets
 */
CLFEXP void workspace_tile( object_t *workspace, int dir );

/* functions (workspace_window) */

/**
 * \brief Creates a Workspace widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Workspace widget object.
 */
CLFEXP object_t *workspace_window_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets the title of a Workspace Window widget
 * 
 * \param window A valid Workspace Window widget
 * \param title The new title for the widget
 */
CLFEXP void workspace_window_set_title( object_t *window, char *title );

/**
 * \brief Makes a Workspace Window widget visible
 * 
 * \param window A valid Workspace Window widget
 */
CLFEXP void workspace_window_show( object_t *window );

/**
 * \brief Makes a Workspace Window widget invisible
 * 
 * \param window A valid Workspace Window widget
 */
CLFEXP void workspace_window_hide( object_t *window );

/**
 * \brief Restores a Workspace Window widget
 * 
 * \param window A valid Workspace Window widget
 */
CLFEXP void workspace_window_restore( object_t *window );

/**
 * \brief Minimises a Workspace Window widget
 * 
 * \param window A valid Workspace Window widget
 */
CLFEXP void workspace_window_minimise( object_t *window );

/**
 * \brief Maxmimises a Workspace Window widget
 * 
 * \param window A valid Workspace Window widget
 */
CLFEXP void workspace_window_maximise( object_t *window );

/**
 * \brief Sets the icon of a Workspace Window widget
 * 
 * \param window A valid Workspace Window widget
 * \param icon A valid Image object.
 */
CLFEXP void workspace_window_set_icon( object_t *w, image_t *icon );

/* American spelling */
#define workspace_window_minimize workspace_window_minimise
#define workspace_window_maximize workspace_window_maximise

/*\@}*/
#endif
