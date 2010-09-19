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


#ifndef _CLARO_GRAPHICS_STATUS_ICON_H
#define _CLARO_GRAPHICS_STATUS_ICON_H
/** \defgroup image_t Image
 * \brief Represents an image
 * @{
 */

#define assert_only_status_icon(o,n)	assert_type_check( o, n, "claro.graphics.status_icon" )
#define assert_valid_status_icon(o,n)	assert_not_null( o, n, "StatusIcon" ); \
				assert_only_status_icon( o, n )

typedef struct status_icon_ status_icon_t;

struct status_icon_
{
    object_t object;
	
	image_t * icon;
    void *native;
    void *native2;
};

/**
 * \brief Creates a status icon
 *
 * \param parent Parent object (usually the application's main window),
 *               can be NULL.
 * \param image The image object for the icon NOT NULL
 * \param flags Flags
 * \return New status_icon_t object
 */
CLFEXP object_t * status_icon_create(object_t *parent, image_t * icon, int flags);

/**
 * \brief sets the status icon's image 
 *
 * \param status Status Icon
 * \param image The image object for the icon
 */
CLFEXP void status_icon_set_icon(object_t * status, image_t * icon);

/**
 * \brief sets the status icons's menu
 *
 * \param status Status Icon
 * \param menu The menu object for the popup menu
 */
CLFEXP void status_icon_set_menu(object_t * status, object_t * menu);

/**
 * \brief sets the status icon's visibility
 *
 * \param status Status Icon
 * \param visible whether the status icon is visible or not
 */
CLFEXP void status_icon_set_visible(object_t * status, int visible);

/**
 * \brief sets the status icon's tooltip
 *
 * \param status Status Icon
 * \param tooltip Tooltip string
 */
CLFEXP void status_icon_set_tooltip(object_t * status, const char * tooltip);

/*\@}*/
#endif

