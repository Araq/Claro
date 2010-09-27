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


#ifndef _CLARO_GRAPHICS_SYSTEM_H
#define _CLARO_GRAPHICS_SYSTEM_H
/** \defgroup system System
 * \brief A group of wrappers for general functions like the clipboard.
 * @{
 */

/**
 * \brief Opens the default web browser and visits the specified url.
 *
 * \param w The widget requesting the action, some platforms may use this value.
 * \param url Full URL to open.
 */
CLFEXP void open_browser( widget_t *w, char *url );

/**
 * \brief Sets the (text) clipboard to the specified text value.
 *
 * \param w The widget requesting the action, some platforms may use this value.
 * \param text The text to place in the clipboard.
 * \return 1 on success, 0 on failure.
 */
CLFEXP int clipboard_set_text( widget_t *w, char *text );

/*\@}*/
#endif
