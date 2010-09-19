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


#ifndef _CLARO_GRAPHICS_WIDGETS_IMAGE_H
#define _CLARO_GRAPHICS_WIDGETS_IMAGE_H
/** \defgroup image_widget_t Image Widget
 * \brief The Image Widget
 * @{
 */

#define assert_only_image_widget(o,n)		assert_type_check( o, n, "claro.graphics.widgets.image" )
#define assert_valid_image_widget(o,n)		assert_not_null( o, n, "Image" ); \
						assert_only_image_widget( o, n )

typedef struct image_widget_ {
    widget_t widget;
    
    image_t *src;
} image_widget_t;

/**
 * \brief Creates an Image widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Image widget object.
 */
CLFEXP object_t *image_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Creates an Image widget with an image
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \param image A valid Image object.
 * \return A new Image widget object.
 */
CLFEXP object_t *image_widget_create_with_image( object_t *parent, bounds_t *bounds, int flags, image_t *image );

/**
 * \brief Sets the image object of the image widget
 * 
 * \param image A valid image widget
 * \param src The source image object
 */
CLFEXP void image_set_image( object_t *image, image_t *src );

/*\@}*/
#endif

