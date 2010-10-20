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


#ifndef _CLARO_GRAPHICS_WIDGET_H
#define _CLARO_GRAPHICS_WIDGET_H
/** \defgroup widget_t Widget
 * \brief The base type of all widgets
 * @{
 */

#define assert_only_widget(o,n)		assert_type_check_relaxed( o, n, "claro.graphics.widgets." )
#define assert_valid_widget(o,n)	assert_not_null( o, n, "Widget" ); \
					assert_only_widget( o, n )

#define cWidgetNoBorder (1<<24)
#define cWidgetCustomDraw (1<<25)

typedef struct widget_ widget_t;

typedef struct bounds_
{
	int x, y;
	int w, h;
	
	object_t *owner;
} bounds_t;

#define cSizeRequestChanged 1

struct widget_
{
    object_t object;
	
    bounds_t *size_req;
    bounds_t size;
    bounds_t size_ct;
    
    int supports_alpha;
    
    int size_flags;
    
    int flags;
    int visible;
    int notify_flags;
    
    font_t font;
    
    void *native;      /* native widget */
    void *ndata;       /* additional native data */
    void *container;   /* native widget container (if not ->native) */
    void *naddress[4]; /* addressed for something we override or need to remember */
};

/* notify flags, bits */
enum cWidgetNotify
{
	cNotifyMouse = 1,
	cNotifyKey = 2,
};

/**
 * \brief Makes the specified widget visible.
 *
 * \param widget A widget
 */
CLFEXP void widget_show( object_t *widget );

/**
 * \brief Makes the specified widget invisible.
 *
 * \param widget A widget
 */
CLFEXP void widget_hide( object_t *widget );

/**
 * \brief Enables the widget, allowing focus
 *
 * \param widget A widget
 */
CLFEXP void widget_enable( object_t *widget );

/**
 * \brief Disables the widget
 * When disabled, a widget appears greyed and cannot
 * receive focus.
 *
 * \param widget A widget
 */
CLFEXP void widget_disable( object_t *widget );

/**
 * \brief Give focus to the specified widget
 *
 * \param widget A widget
 */
CLFEXP void widget_focus( object_t *widget );

/**
 * \brief Closes a widget
 *
 * Requests that a widget be closed by the platform code. 
 * This may or may not result in immediate destruction of the widget,
 * however the actual Claro widget object will remain valid until at
 * least the next loop itteration.
 *
 * \param widget A widget
 */
CLFEXP void widget_close( object_t *widget );

/**
 * \brief Retrieve the screen offset of the specified widget.
 *
 * Retrieves the X and Y screen positions of the widget.
 *
 * \param widget A widget
 * \param dx Pointer to the location to place the X position.
 * \param dy Pointer to the location to place the Y position.
 */
CLFEXP void widget_screen_offset( object_t *widget, int *dx, int *dy );

/**
 * \brief Sets the additional notify events that should be sent.
 *
 * For performance reasons, some events, like mouse and key events,
 * are not sent by default. By specifying such events here, you can
 * elect to receive these events.
 *
 * \param widget A widget
 * \param flags Any number of cWidgetNotify flags ORed together.
 */
CLFEXP void widget_set_notify( object_t *widget, int flags );

enum
{
	cCursorNormal=0,
	cCursorTextEdit,
	cCursorWait,
	cCursorPoint,
};

/**
 * \brief Sets the mouse cursor for the widget
 *
 * \param widget A widget
 * \param cursor A valid cCursor* value
 */
CLFEXP void widget_set_cursor( object_t *widget, int cursor );

/**
 * \brief Retrieves the key pressed in a key notify event.
 *
 * \param widget A widget
 * \param event An event resource
 * \return The keycode of the key pressed.
 */
CLFEXP int widget_get_notify_key( object_t *widget, event_t *event );

/**
 * \brief Updates the bounds structure with new values
 *
 * This function should \b always be used instead of setting the
 * members manually. In the future, there may be a \b real reason
 * for this.
 *
 * \param bounds A bounds structure
 * \param x The new X position
 * \param y The new Y position
 * \param w The new width
 * \param h The new height
 */
CLFEXP void bounds_set( bounds_t *bounds, int x, int y, int w, int h );

/**
 * \brief Create a new bounds object
 *
 * Creates a new bounds_t for the specified bounds.
 *
 * \param x X position
 * \param y Y position
 * \param w Width
 * \param h Height
 * \return A new bounds_t structure
 */
CLFEXP bounds_t *new_bounds( int x, int y, int w, int h );


CLFEXP bounds_t *get_req_bounds( object_t *widget );

#define WIDGET(x) ((widget_t*)x)

#define NO_BOUNDS (&no_bounds)


/************************************************************************/


/** \defgroup internals_widget_t Widget Internals
 * @{
 */


/** \internal
 * \brief Internal pre-inititalisation hook
 *
 * \param widget A widget
 */
CLFEXP void widget_pre_init( object_t *widget );

/** \internal
 * \brief Internal post-inititalisation hook
 *
 * \param widget A widget
 */
CLFEXP void widget_post_init( object_t *widget );

/** \internal
 * \brief Internal resize event handler
 *
 * \param obj An object
 * \param event An event resource
 */
CLFEXP void widget_resized_handle( object_t *obj, event_t *event );

CLVEXP bounds_t no_bounds;

#ifndef CLARO_SCRIPT_INTERFACE
typedef void (*cgraphics_create_function)(widget_t *widget);
#endif

/** \internal
 * \brief Internal default widget creation function
 *
 * \param parent The parent of the widget
 * \param widget_size The size in bytes of the widget's structure
 * \param widget_name The object type of the widget (claro.graphics.widgets.*)
 * \param size_req The initial bounds of the widget
 * \param flags Widget flags
 * \param creator The platform function that will be called to actually create
 *                the widget natively.
 * \return A new widget object
 */
CLFEXP object_t *default_widget_create(object_t *parent, 
                                     size_t widget_size, const char *widget_name, bounds_t *size_req,
                                     int flags, cgraphics_create_function creator);

/** \internal
 * \brief Retrieves the native container of the widget's children
 *
 * \param widget A widget
 * \return A pointer to the native widget that will hold w's children
 */
CLFEXP void *widget_get_container( object_t *widget );

/** \internal
 * \brief Sets the content size of the widget.
 *
 * \param widget A widget
 * \param w New width of the content area of the widget
 * \param h New height of the content area of the widget
 * \param event Whether to send a content_size event
 */
CLFEXP void widget_set_content_size( object_t *widget, int w, int h, int event );

/** \internal
 * \brief Sets the size of the widget.
 *
 * \param widget A widget
 * \param w New width of the widget
 * \param h New height of the widget
 * \param event Whether to send a resize event
 */
CLFEXP void widget_set_size( object_t *widget, int w, int h, int event );

/** \internal
 * \brief Sets the position of the widget's content area.
 *
 * \param widget A widget
 * \param x New X position of the widget's content area
 * \param y New Y position of the widget's content area
 * \param event Whether to send a content_move event
 */
CLFEXP void widget_set_content_position( object_t *widget, int x, int y, int event );

/** \internal
 * \brief Sets the position of the widget.
 *
 * \param widget A widget
 * \param x New X position of the widget's content area
 * \param y New Y position of the widget's content area
 * \param event Whether to send a moved event
 */
CLFEXP void widget_set_position( object_t *widget, int x, int y, int event );

/** \internal
 * \brief Sends a destroy event to the specified widget.
 *
 * You should use widget_close() in application code instead.
 *
 * \param widget A widget
 */
CLFEXP void widget_destroy( object_t *widget );

/*\@}*/

/************************************************************************/

/*\@}*/


#endif
