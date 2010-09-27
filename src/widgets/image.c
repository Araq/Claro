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
#include "platform.h"

object_t *image_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = default_widget_create(parent, sizeof(image_widget_t),
						"claro.graphics.widgets.image", bounds, 
						flags, cgraphics_image_widget_create);
	
	return obj;
}

object_t *image_widget_create_with_image( object_t *parent, bounds_t *bounds, int flags, image_t *image )
{
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	assert_valid_image( image, "image" );
	
	obj = default_widget_create(parent, sizeof(image_widget_t),
						"claro.graphics.widgets.image", bounds, 
						flags, cgraphics_image_widget_create);
	
	image_set_image( obj, image );
	
	return obj;
}

void image_set_image( object_t *image, image_t *src )
{
	image_widget_t *iw = (image_widget_t *)image;
	
	assert_valid_image_widget( image, "image" );
	assert_valid_image( src, "src" );
	
	iw->src = src;
	
	cgraphics_image_set_image( WIDGET(image) );
}
