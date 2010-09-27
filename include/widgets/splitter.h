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


#ifndef _CLARO_GRAPHICS_WIDGETS_SPLITTER_H
#define _CLARO_GRAPHICS_WIDGETS_SPLITTER_H
/** \defgroup splitter_widget_t Splitter Widget
 * \brief The Splitter Widget
 * @{
 */

#define assert_only_splitter_widget(o,n)	assert_type_check( o, n, "claro.graphics.widgets.splitter" )
#define assert_valid_splitter_widget(o,n)	assert_not_null( o, n, "Splitter" ); \
						assert_only_splitter_widget( o, n )

typedef struct splitter_widget_ splitter_widget_t;

enum cSplitterChildren {
	cSplitterFirst=0,
	cSplitterSecond=1,
};

typedef struct
{
	int flex;
	int size;
	
	widget_t *w;
} splitter_widget_child;

struct splitter_widget_
{
	widget_t widget;
	
	splitter_widget_child children[2];
};

enum
{
	cSplitterHorizontal=0,
	cSplitterVertical=1,
};

/* functions */

/**
 * \brief Creates a Splitter widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Splitter widget object.
 */
CLFEXP object_t *splitter_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets the sizing information of a child
 * 
 * \param splitter A valid splitter widget object
 * \param child The child number, either cSplitterFirst or cSplitterSecond.
 * \param flex 1 if this child should receive extra space as the splitter expands, 0 if not
 * \param size The size of this child
 */
CLFEXP void splitter_set_info( object_t *splitter, int child, int flex, int size );

/*\@}*/
#endif
