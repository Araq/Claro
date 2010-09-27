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


#ifndef _CLARO_GRAPHICS_WIDGETS_STOCK_H
#define _CLARO_GRAPHICS_WIDGETS_STOCK_H
/** \defgroup stock_items Stock items
 * \brief Stock items
 * @{
 */

/* functions */

/**
 * \brief obtains a stock image
 * 
 * \param stock_id The string ID of the stock image, NOT NULL.
 * \return The Image object.
 */
CLFEXP image_t * stock_get_image(const char * stock_id);


/**
 * \brief adds a stock id image
 * 
 * \param stock_id The string ID of the stock image, NOT NULL.
 * \param img The Image object to add.
 * \return The Image object.
 */
CLFEXP void stock_add_image(const char * stock_id, image_t * img);


/**
 *	INTERNAL
 */

void claro_stock_init();


















/*\@}*/
#endif

