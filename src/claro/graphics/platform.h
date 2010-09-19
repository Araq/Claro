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


#ifndef _CLARO_GRAPHICS_PLATFORM_H
#define _CLARO_GRAPHICS_PLATFORM_H

#include "graphics.h"

void cgraphics_init( );
void cgraphics_check_events( );
void cgraphics_widget_create( widget_t *widget );

/* System */
void cgraphics_open_browser( widget_t *w, char *url );
int cgraphics_clipboard_set_text( widget_t *w, char *text );

/* Windows */
void cgraphics_window_widget_create( widget_t *widget );
void cgraphics_window_show( widget_t *w );
void cgraphics_window_hide( widget_t *w );
void cgraphics_window_focus( widget_t *w );
void cgraphics_window_update_title( widget_t *w );
void cgraphics_window_minimise( widget_t *w );
void cgraphics_window_maximise( widget_t *w );
void cgraphics_window_restore( widget_t *w );
void cgraphics_window_update_icon( widget_t *w );

/* Scrollbar */
void cgraphics_scrollbar_widget_create( widget_t *w );
int cgraphics_scrollbar_get_sys_width( );
void cgraphics_scrollbar_set_range( widget_t *w );
void cgraphics_scrollbar_set_pos( widget_t *w, int pos );
int cgraphics_scrollbar_get_pos( widget_t *w );

/* Toolbar */
void cgraphics_toolbar_widget_create( widget_t *widget );
void cgraphics_toolbar_new_icon( widget_t *widget, list_item_t *item );
void cgraphics_toolbar_remove_icon( widget_t *widget, list_item_t *item );
void cgraphics_toolbar_set_menu( widget_t *widget, list_item_t *item );
/*
void cgraphics_toolbar_new_popup_item( widget_t *widget, list_item_t *item );
void cgraphics_toolbar_remove_popup_item( widget_t *widget, list_item_t *item );
*/

/* Menubar */
void cgraphics_menubar_widget_create( widget_t *widget );
void cgraphics_menubar_new_item( widget_t *widget, list_item_t *item );
void cgraphics_menubar_remove_item( widget_t *widget, list_item_t *item );

void cgraphics_menubar_disable_item( widget_t *menubar, list_item_t *item );
void cgraphics_menubar_enable_item( widget_t *menubar, list_item_t *item );

/* Menu */
void cgraphics_menu_widget_create( widget_t *widget );
void cgraphics_menu_new_item( widget_t *widget, list_item_t *item );
void cgraphics_menu_remove_item( widget_t *widget, list_item_t *item );

void cgraphics_menu_disable_item( widget_t *menubar, list_item_t *item );
void cgraphics_menu_enable_item( widget_t *menubar, list_item_t *item );

void cgraphics_menu_popup( widget_t *menu, int x, int y, int flags );

/* TextBox */
void cgraphics_textbox_widget_create( widget_t *widget );
void cgraphics_textbox_update_text( textbox_widget_t *widget );
void cgraphics_textbox_set_pos( widget_t *widget, int pos );
int cgraphics_textbox_get_pos( widget_t *widget );

/* Button */
void cgraphics_button_widget_create( widget_t *widget );
void cgraphics_button_update_text( button_widget_t *widget );
void cgraphics_button_update_image( button_widget_t *widget, const char *filename);

/* Canvas */
void cgraphics_canvas_widget_create( widget_t *widget );
void cgraphics_canvas_redraw( canvas_widget_t *w );

void cgraphics_canvas_set_text_color( widget_t *widget, double r, double g, double b, double a );
void cgraphics_canvas_set_text_bgcolor( widget_t *widget, double r, double g, double b, double a );
void cgraphics_canvas_set_text_font( widget_t *widget, char *face, int size, int weight, int slant, int decoration );
int cgraphics_canvas_text_width( widget_t *widget, const char *text, int len );
int cgraphics_canvas_text_box_width( widget_t *widget, const char *text, int len );
int cgraphics_canvas_text_display_count( widget_t *widget, const char *text, int width );
void cgraphics_canvas_show_text( widget_t *widget, int x, int y, const char *text, int len );

void cgraphics_canvas_fill_rect( widget_t *widget, int x, int y, int w, int h, double r, double g, double b, double a );
void cgraphics_canvas_draw_image( widget_t *widget, image_t *image, int x, int y );

/* Splitter */
void cgraphics_splitter_widget_create( widget_t *widget);
void cgraphics_splitter_updated( widget_t *widget, int child );

/* Container */
void cgraphics_container_widget_create( widget_t *widget );

/* Label */
void cgraphics_label_widget_create(widget_t *widget);
void cgraphics_label_update_text(widget_t *widget);
void cgraphics_label_update_justify(widget_t *widget);

/* Widgets (general) */
void cgraphics_widget_show( widget_t *widget );
void cgraphics_widget_hide( widget_t *widget );
void cgraphics_widget_enable( widget_t *widget );
void cgraphics_widget_disable( widget_t *widget );
void cgraphics_widget_close( widget_t *widget );
void cgraphics_widget_focus( widget_t *widget );
void cgraphics_update_bounds( object_t *obj );
void cgraphics_post_init( widget_t *widget );

void cgraphics_widget_screen_offset( widget_t *object, int *dx, int *dy );
void cgraphics_widget_set_cursor( widget_t *widget, int cursor );

/* Widgets (font) */
void cgraphics_widget_set_font( widget_t *widget, font_t *font );
int cgraphics_widget_font_string_width( widget_t *w, char *text, int chars );
void *cgraphics_font_from_font( font_t *font );

/* Statusbar */
void cgraphics_statusbar_widget_create(widget_t *widget);
void cgraphics_statusbar_widget_update_text(statusbar_widget_t *sb);

/** Progressbar */
void cgraphics_progress_widget_create(widget_t *widget);
void cgraphics_progress_set_level(widget_t *progress, double percentage);
void cgraphics_progress_set_orientation(widget_t *progress, int flags);

/* Checkbox */
void cgraphics_checkbox_widget_create(widget_t *widget);
void cgraphics_checkbox_set_text(widget_t *widget);
void cgraphics_checkbox_update_checked( widget_t *widget );

/* Radio */
void cgraphics_radiogroup_create( radiogroup_t *group );
void cgraphics_radiobutton_widget_create(widget_t *widget);
void cgraphics_radiobutton_set_text(widget_t *widget);
void cgraphics_radiobutton_set_group(widget_t *radio);

/* Textarea */
void cgraphics_textarea_widget_create(widget_t *widget);
void cgraphics_textarea_set_text( widget_t *obj );
char *cgraphics_textarea_get_text( widget_t *obj);

/* Frame */
void cgraphics_frame_widget_create(widget_t *widget);
void cgraphics_frame_set_text(widget_t *widget );

/* Image Widget */
void cgraphics_image_widget_create( widget_t *widget );
void cgraphics_image_set_image( widget_t *widget );

/* Image Functions */
void cgraphics_image_load( image_t *img, const char *file );

/* ListView */
void cgraphics_listview_widget_create( widget_t *widget );
void cgraphics_listview_new_row( widget_t *widget, list_item_t *item );
void cgraphics_listview_remove_row( widget_t *widget, list_item_t *item );
void cgraphics_listview_select_row( widget_t *widget, list_item_t *item );
void cgraphics_listview_widget_create_columns( widget_t *widget );

/* ListBox */
void cgraphics_listbox_widget_create( widget_t *widget );
void cgraphics_listbox_new_row( widget_t *widget, list_item_t *item );
void cgraphics_listbox_remove_row( widget_t *widget, list_item_t *item );
void cgraphics_listbox_select_row( widget_t *widget, list_item_t *item );

/* Combo */
void cgraphics_combo_widget_create( widget_t *widget );
void cgraphics_combo_new_row( widget_t *widget, list_item_t *item );
void cgraphics_combo_remove_row( widget_t *widget, list_item_t *item );
void cgraphics_combo_select_row( widget_t *widget, list_item_t *item );

/* Workspace */

void cgraphics_workspace_widget_create( widget_t *widget );
void cgraphics_workspace_set_active( widget_t *widget, object_t *child );
object_t *cgraphics_workspace_get_active( widget_t *widget );
void cgraphics_workspace_cascade( widget_t *widget );
void cgraphics_workspace_tile( widget_t *widget, int dir );

void cgraphics_workspace_window_widget_create( widget_t *widget );
void cgraphics_workspace_window_update_title( widget_t *widget );
void cgraphics_workspace_window_show( widget_t *widget );
void cgraphics_workspace_window_hide( widget_t *widget );
void cgraphics_workspace_window_restore( widget_t *widget );
void cgraphics_workspace_window_minimise( widget_t *widget );
void cgraphics_workspace_window_maximise( widget_t *widget );
void cgraphics_workspace_window_update_icon( widget_t *w );

/* OpenGL */
void cgraphics_opengl_widget_create( widget_t *widget );
void cgraphics_opengl_flip( widget_t *widget );
void cgraphics_opengl_activate( widget_t *widget );

/* TreeView */
void cgraphics_treeview_widget_create( widget_t *widget );
void cgraphics_treeview_new_row( widget_t *widget, list_item_t *item );
void cgraphics_treeview_remove_row( widget_t *widget, list_item_t *item );
void cgraphics_treeview_edit_row( widget_t *widget, list_item_t *item );
void cgraphics_treeview_select_row( widget_t *widget, list_item_t *item );
void cgraphics_treeview_expand( widget_t *widget, list_item_t *item );
void cgraphics_treeview_collapse( widget_t *widget, list_item_t *item );

/* Font Selection Dialog */
void cgraphics_font_dialog_widget_create( widget_t *widget );
void cgraphics_font_dialog_update_from_selection( widget_t *widget );
void cgraphics_font_dialog_update_to_selection( widget_t *widget );

#endif
