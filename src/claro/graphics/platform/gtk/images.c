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

/* returns a valid pixbuf with a ref count of 1 on success, null on failure */
static GdkPixbuf * load_inline_png(const unsigned char * data, int len)
{
	GError * err = NULL;
	GdkPixbufLoader * loader = gdk_pixbuf_loader_new_with_mime_type("image/png", &err);
	GdkPixbuf * pixbuf = NULL;

	if(!gdk_pixbuf_loader_write(loader,data,len,&err))
	{
		g_object_unref(loader);
		return NULL;		
	}
	if(!gdk_pixbuf_loader_close(loader,NULL))
	{
		g_object_unref(loader);
		return NULL;
	}
	pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
	g_object_ref(pixbuf);
	g_object_unref(loader);

	return pixbuf;
}

void cgraphics_image_load( image_t *img, const char *file )
{
	GError *e = NULL;
	
	img->native = gdk_pixbuf_new_from_file( file, &e );
}

void cgraphics_image_load_inline_png( image_t *img, const unsigned char * data, int len )
{
	img->native = load_inline_png(data,len);
}
