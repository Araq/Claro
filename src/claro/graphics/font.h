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


#ifndef _CLARO_GRAPHICS_FONT_H
#define _CLARO_GRAPHICS_FONT_H_H
/** \addtogroup widget_t
 * @{
 */

#define cFontSlantNormal 0
#define cFontSlantItalic 1

#define cFontWeightNormal 0
#define cFontWeightBold 1

#define cFontDecorationNormal 0
#define cFontDecorationUnderline 1

typedef struct
{
	int used;
	
	char *face;
	int size;
	int weight;
	int slant;
	int decoration;
	
	void *native;
} font_t;

typedef struct
{
	int used;
	
	float r, g, b, a;
} color_t;

/**
 * \brief Sets the font details of the specified widget
 *
 * \param widget A widget
 * \param face Font face string
 * \param size Size of the font in pixels
 * \param weight The weight of the font
 * \param slant The sland of the font
 * \param decoration The decoration of the font
 */
CLFEXP void widget_set_font( object_t *widget, char *face, int size, int weight, int slant, int decoration );

/**
 * \brief Calculates the pixel width of the text in the widget's font.
 *
 * \param widget A widget
 * \param text The text to calculate the width of
 * \param chars The number of characters of text to calculate
 * \return The width of the specified text in pixels
 */
CLFEXP int widget_font_string_width( object_t *widget, char *text, int chars );

/*\@}*/
#endif
