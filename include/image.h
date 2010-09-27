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


#ifndef _CLARO_GRAPHICS_IMAGE_H
#define _CLARO_GRAPHICS_IMAGE_H
/** \defgroup image_t Image
 * \brief Represents an image
 * @{
 */

#define assert_only_image(o,n)	assert_type_check( o, n, "claro.graphics.image" )
#define assert_valid_image(o,n)	assert_not_null( o, n, "Image" ); \
				assert_only_image( o, n )

typedef struct image_ image_t;

struct image_
{
    object_t object;
	
	int width, height;
	
    void *native;
    void *native2;
    void *native3;
    void *icon;
};

/**
 * \brief Loads an image from a file and returns a new image object.
 *
 * The supported formats depend on the platform.
 * The main effort is to ensure that PNG images will always work.
 * Generally, JPEGs and possibly GIFs will also work.
 *
 * \param parent Parent object (usually the application's main window),
 *               can be NULL.
 * \param file The filename of the image file.
 * \return New image_t object containing the loaded image.
 */
CLFEXP image_t *image_load( object_t *parent, const char *file );

/**
 * \brief Loads an image from inline data and returns a new image object.
 *
 *
 * \param parent Parent object (usually the application's main window),
 *               can be NULL.
 * \param data raw PNG image
 * \param len size of data
 * \return New image_t object containing the loaded image.
 */
CLFEXP image_t * image_load_inline_png( object_t *parent, const unsigned char * data, int len);

/*\@}*/
#endif
