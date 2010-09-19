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
#include <claro/contextual.h>
#include <math.h>
#include <assert.h>

widget_t *c;
int red = 0, blue = 0, green = 0;

void handle_main( object_t *obj, event_t *event )
{
    static int last_update = 0;
    int t;
	
    t = time( 0 );
	
    if ( t > last_update )
    {
        canvas_redraw( c );
        last_update = t;
    }
}


void red_clicked( object_t *btn, event_t *event ) {
    red = !red;
    
    if(red) {
        button_set_text(btn, "Red On");
    } else {
        button_set_text(btn, "Red Off");
    }

    canvas_redraw( c );
}


void blue_clicked( object_t *btn, event_t *event ) {
    blue = !blue;
    if(blue) {
        button_set_text(btn, "Blue On");
    } else {
        button_set_text(btn, "Blue Off");
    }
    
    canvas_redraw( c );
}


void green_clicked( object_t *btn, event_t *event ) {
    green = !green;
    
    if(green) {
        button_set_text(btn, "Green On");
    } else {
        button_set_text(btn, "Green Off");
    }
    
    canvas_redraw( c );
}


void handle_redraw( object_t *obj, event_t *event )
{
    /* this code was borrowed from cairoclock.tar.gz, can't remember
     * the site it came from: Google it for the original (gtkmm) source */
	
    canvas_widget_t *cvsw = (canvas_widget_t *)obj;
    cairo_t *cr = cvsw->cr;
	
    cairo_scale( cr, cvsw->widget.size_req->w, cvsw->widget.size_req->h );
	
    // store the current time
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime (&rawtime);
	
    // compute the angles of the indicators of our clock
    double minutes = timeinfo->tm_min * M_PI / 30;
    double hours = timeinfo->tm_hour * M_PI / 6;
    double seconds= timeinfo->tm_sec * M_PI / 30;
	
    // who doesn't want all those nice line settings :)
    cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
    cairo_set_line_width( cr, 0.1 );
	
    // translate to the center of the rendering context and draw a black clock outline
    cairo_set_source_rgba( cr, red, green, blue, 1 );
    cairo_translate( cr, 0.5, 0.5 );
    cairo_arc( cr, 0, 0, 0.4, 0, M_PI * 2 );
    cairo_stroke( cr );
	
    // draw a white dot on the current second.
    cairo_set_source_rgba( cr, 1, 1, 1, 0.6 );
    cairo_arc( cr, sin(seconds) * 0.4, -cos(seconds) * 0.4, 0.05, 0, M_PI * 2 );
    cairo_fill( cr );
	
    // draw the minutes wijzer
    cairo_set_source_rgba( cr, 0.2, 0.2, 1, 0.6 );
    cairo_move_to( cr, 0, 0 );
    cairo_line_to( cr, sin(minutes) * 0.4, -cos(minutes) * 0.4 );
    cairo_stroke( cr );

    // draw the hours wijzer		
    cairo_move_to( cr, 0, 0 );
    cairo_line_to( cr, sin(hours) * 0.2, -cos(hours) * 0.2 );
    cairo_stroke( cr );
}


void window_closed( object_t *btn, event_t *event ) {
    exit(0);
}

void canvas_step_final( void )
{
    object_t *w;
    layout_t *lt;
    object_t *btn = NULL;
    bounds_t *b = NULL;

    b = new_bounds(50, 50, 300, 300);
    
    w = window_widget_create(NULL, b, 0);
    window_set_title(w, "Canvas Clock");

    object_addhandler(w, "destroy", window_closed);
        
    lt = layout_create(w, "[][_<a|clock|>a][{40}Red|Green|Blue]", *b, 10, 10 );
    assert(lt != NULL && "layout was failed to parse");
        
    c = (widget_t *)canvas_widget_create( w, lt_bounds(lt, "clock"), 0 );
	
    object_addhandler(OBJECT(c), "redraw", handle_redraw );
	        
    btn = button_widget_create( w, lt_bounds(lt, "Red"), 0 );
    button_set_text(btn, "Red Off");
    object_addhandler(btn, "pushed", red_clicked );

    btn = button_widget_create( w, lt_bounds(lt, "Green"), 0 );
    button_set_text(btn, "Green Off");
    object_addhandler(btn, "pushed", green_clicked );
        
    btn = button_widget_create( w, lt_bounds(lt, "Blue"), 0 );
    button_set_text(btn, "Blue Off");
    object_addhandler(btn, "pushed", blue_clicked );
        
    window_show( w );
    window_focus( w );
	
    block_heap_loginfo( );
	
    /* add our main loop */
    object_addhandler( claro, "mainloop", handle_main );
}

void *c_init_steps[] = { &claro_graphics_init, &canvas_step_final, NULL };

context_t canvas_context = {
	.context_name = "Contextual Canvas Demo",
	.init_steps = c_init_steps,
};

DECLARE_ROOT_CONTEXT(canvas_context);
