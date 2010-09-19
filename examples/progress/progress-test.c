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


#include <claro/base.h>
#include <claro/graphics.h>
#include <assert.h>

object_t *b2t, *t2b, *l2r, *r2l;
object_t *time_label;

void tick_progress( object_t *obj, event_t *event )
{
	static int last_update = 0;
	int t;
	double second = 0.0;
	char timestr[20];
	
	t = time( 0 );
	
	if ( t > last_update )
	{
		second = (double)(t % 60);
		snprintf(timestr, 19, "%d", (int)second);
		label_set_text(time_label, timestr);
		progress_set_level(b2t, second/60.0);
		progress_set_level(t2b, second/60.0);
		progress_set_level(l2r, second/60.0);
		progress_set_level(r2l, second/60.0);
	}
}

void window_closed( object_t *btn, event_t *event ) {
	exit(0);
}


int main( int argc, char *argv[] )
{
	
	object_t *w;
	layout_t *lt;
	bounds_t *b = NULL;
	
	claro_base_init( );
	claro_graphics_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
	b = new_bounds(50, 50, 300, 300);
	w = window_widget_create(NULL, b, 0);
	window_set_title(w, "Progress Demo");
	object_addhandler(w, "destroy", window_closed);

	lt = layout_create(w, "[_bottom2top(30)|(10)|top2bottom(30)|time][{10}][left2right][{10}][right2left]", *b, 30, 30 );
	assert(lt != NULL && "failed to parse layout");
		
	b2t = progress_widget_create(w, lt_bounds(lt, "bottom2top"), cProgressBottomTop);
	t2b = progress_widget_create(w, lt_bounds(lt, "top2bottom"), cProgressTopBottom);
	l2r = progress_widget_create(w, lt_bounds(lt, "left2right"), cProgressLeftRight);
	r2l = progress_widget_create(w, lt_bounds(lt, "right2left"), cProgressRightLeft);
	time_label = label_widget_create_with_text(w, lt_bounds(lt, "time"), cLabelCenter, "0");	
	
	object_addhandler( claro, "mainloop", tick_progress );
	
	window_show( w );
	window_focus( w );
	
	block_heap_loginfo( );
	
	claro_loop( );
	
	return 0;
}
