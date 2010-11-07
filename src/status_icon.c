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

CLFEXP object_t * status_icon_create(object_t *parent, image_t * icon, int flags)
{
    status_icon_t * status;
    
    status = (status_icon_t*)object_create(parent, sizeof(status_icon_t), "claro.graphics.status_icon");
    
    cgraphics_status_icon_create(status, flags);
    cgraphics_status_icon_set_icon(status, icon);
    
    return (object_t*)status;
}

CLFEXP void status_icon_set_icon(object_t * status, image_t * icon)
{
    assert_valid_status_icon(status, "status");
    
    cgraphics_status_icon_set_icon((status_icon_t*)status, icon);
}

CLFEXP void status_icon_set_menu(object_t * status, object_t * menu)
{
    assert_valid_status_icon(status, "status");
    assert_valid_menu_widget(menu, "menu");
    
    cgraphics_status_icon_set_menu((status_icon_t*)status, menu);
}

CLFEXP void status_icon_set_visible(object_t * status, int visible)
{
    assert_valid_status_icon(status, "status");
    
    cgraphics_status_icon_set_visible((status_icon_t*)status, visible);
}

CLFEXP void status_icon_set_tooltip(object_t * status, const char * tooltip)
{
    assert_valid_status_icon(status, "status");
    
    cgraphics_status_icon_set_tooltip((status_icon_t*)status, tooltip);
}



