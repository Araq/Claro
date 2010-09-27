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

object_t *dialog_widget_create( object_t *parent, bounds_t *bounds, const char *format, int flags )
{
    return NULL;
}

void dialog_set_text( object_t *obj, const char *text )
{
}

void dialog_set_default_icon(const char *type, const char *file)
{
}

const char *dialog_get_default_icon(int dialog_type)
{
    return NULL;
}

int dialog_warning(const char *format, const char *text)
{
    return 0;
}

int dialog_info(const char *format, const char *text)
{
    return 0;
}

int dialog_error(const char *format, const char *text)
{
    return 0;
}

int dialog_other(const char *format, const char *text, const char *default_icon)
{
    return 0;
}
