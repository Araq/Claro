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
#include <math.h>
#include <assert.h>

widget_t *c;

char *disptext = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Fusce mollis ipsum at felis. Etiam vel mauris. Vivamus et elit vitae nunc pretium cursus. Praesent suscipit turpis id enim. Nunc in elit vel urna ornare blandit. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Sed laoreet, augue at pellentesque vulputate, tortor ipsum tincidunt diam, non tristique augue mi non purus. Praesent vehicula. Fusce ultricies tristique massa. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos hymenaeos. Proin sit amet arcu at sapien faucibus vulputate. Quisque dolor lacus, hendrerit sed, aliquet sed, elementum ut, est. Ut rutrum nisi eget felis. Donec lectus pede, placerat quis, sodales a, porta eu, risus. Quisque eleifend pretium nisi. Morbi ipsum pede, convallis in, convallis a, aliquet in, ipsum. Proin tempor tristique nisl. Sed leo. Sed vestibulum.";

int calc_best_end( const char *text, int max )
{
	int a = max - 1;
	char *lptext;
	
	lptext = &text[a];
	
	while ( lptext != text && *lptext != ' ' )
		lptext--;
	
	/* if we didn't find a space, return the original. */
	if ( lptext == text )
		return max;
	
	/* otherwise, return the length to the space */
	return lptext - text + 1;
}

int mouse_sel = 0;
int mouse_startx = 0, mouse_starty = 0;
int mouse_endx = 0, mouse_endy = 0;

void handle_mousedown( object_t *obj, event_t *event )
{
	mouse_sel = 1;
	mouse_startx = event_get_arg_int( event, 0 );
	mouse_starty = event_get_arg_int( event, 1 );
	mouse_endx = event_get_arg_int( event, 0 );
	mouse_endy = event_get_arg_int( event, 1 );
	
	canvas_redraw( c );
}

void handle_mouseup( object_t *obj, event_t *event )
{
	mouse_sel = 0;
	
	canvas_redraw( c );
}

void handle_mousemove( object_t *obj, event_t *event )
{
	mouse_endx = event_get_arg_int( event, 0 );
	mouse_endy = event_get_arg_int( event, 1 );
	
	canvas_redraw( c );
}

void handle_redraw( object_t *obj, event_t *event )
{
	canvas_widget_t *cvsw = (canvas_widget_t *)obj;
	cairo_t *cr = cvsw->cr;
	int font_height = 12;
	int padding = 2;
	int height = cvsw->widget.size_req->h;
	int usable_width = cvsw->widget.size_req->w - padding;
	int lines = ceil((float)height / font_height);
	int a, b;
	int is_hl = 0;
	int is_hls, is_hle, hln, hln2, hlx;
	int tmp_mouse_startx=0, tmp_mouse_starty=0;
	int tmp_mouse_endx=0, tmp_mouse_endy=0;
		
	cairo_text_extents_t te;
	
	canvas_set_text_font( WIDGET(cvsw), "Verdana", font_height, CTEXT_WEIGHT_NORMAL, CTEXT_SLANT_NORMAL, 0 );
	font_height += 4;
	
	tmp_mouse_startx = mouse_startx;
	tmp_mouse_starty = mouse_starty;
	tmp_mouse_endx = mouse_endx;
	tmp_mouse_endy = mouse_endy;
	
	// forward or backward highlighting?
	if ( mouse_sel )
	{
		hln =  floor( (height-tmp_mouse_starty) / font_height );
		hln2 = floor( (height-tmp_mouse_endy) / font_height );
		
		if ( (hln == hln2 && tmp_mouse_startx > tmp_mouse_endx) || hln2 > hln )
		{
			tmp_mouse_startx = mouse_endx;
			tmp_mouse_starty = mouse_endy;
			tmp_mouse_endx = mouse_startx;
			tmp_mouse_endy = mouse_starty;
		}
	}
	
	/*
	cairo_select_font_face( cr, "Verdana", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
	cairo_set_font_size( cr, font_height );
	*/
	cairo_set_source_rgba( cr, 1, 1, 1, 1 );
	
	canvas_set_text_bgcolor( WIDGET(cvsw), 1, 1, 1, 1 );
	
	cairo_move_to( cr, 0, 0 );
	cairo_line_to( cr, cvsw->widget.size_req->w, 0 );
	cairo_line_to( cr, cvsw->widget.size_req->w, cvsw->widget.size_req->h );
	cairo_line_to( cr, 0, cvsw->widget.size_req->h );
	cairo_fill( cr );
	
	cairo_translate( cr, padding, -padding );
	
	canvas_set_text_bgcolor( WIDGET(cvsw), 1, 1, 1, 1 );
	
	for ( a = 0; a < lines; a++ )
	{
		char *tptr = disptext;
		int l = 0;
		int s;
		
		while ( *tptr != 0 )
		{
			l++;
			
			b = canvas_text_display_count( WIDGET(cvsw), tptr, usable_width );
			if ( tptr[b] != 0 )
				b = calc_best_end( tptr, b );
			tptr += b;
		}
		
		tptr = disptext;
		s = a + l;

#define SET_NORMAL canvas_set_text_bgcolor( WIDGET(cvsw), 1, 1, 1, 1 )
#define SET_HIGHLT canvas_set_text_bgcolor( WIDGET(cvsw), 0.5, 0.5, 0.5, 1 )

		while ( *tptr != 0 )
		{
			canvas_set_text_color( WIDGET(cvsw), 0.5, 0, 0, 1 );
			b = canvas_text_display_count( WIDGET(cvsw), tptr, usable_width );
			if ( tptr[b] != 0 )
				b = calc_best_end( tptr, b );
			
			// highlighting
			
			is_hls = is_hle = 0;
			
			if ( mouse_sel )
			{
				int tmpx = height - (s * font_height);
				int tmpx2 = tmpx+font_height;
				
				is_hls = (tmp_mouse_starty > tmpx) && (tmp_mouse_starty <= tmpx2);
				is_hle = (tmp_mouse_endy > tmpx) && (tmp_mouse_endy <= tmpx2);
				
				is_hl = (tmp_mouse_starty < tmpx) && (tmp_mouse_endy > tmpx2);
			}
			else
				is_hl = 0;
			
			if ( is_hls || is_hle )
			{
				// how many characters until we hit the start/end of highlighting?
				hln = canvas_text_display_count( WIDGET(cvsw), tptr, (is_hls?tmp_mouse_startx:tmp_mouse_endx) );
				
				if ( hln > b )
					hln = b;
				
				// select the normal style
				if ( is_hls )
					SET_NORMAL;
				else
					SET_HIGHLT;
				
				// draw everything until that point
				canvas_show_text( WIDGET(cvsw), 2, (height) - (s * font_height), tptr, hln );
				
				// where did that leave us?
				hlx = cgraphics_canvas_text_width( WIDGET(cvsw), tptr, hln );
				
				// select the highlighting style
				if ( is_hls )
					SET_HIGHLT;
				else
					SET_NORMAL;
				
				if ( is_hls && is_hle )
				{
					// draw to the end of highlighting - same as above code, read those comments :p
					hln2 = canvas_text_display_count( WIDGET(cvsw), tptr, tmp_mouse_endx );
				
					if ( hln2 > b )
						hln2 = b;
					
					// draw to the end of highlighting
					canvas_show_text( WIDGET(cvsw), 2+hlx, (height) - (s * font_height), tptr+hln, hln2-hln );
					
					// again, where are we? :)
					hlx = cgraphics_canvas_text_width( WIDGET(cvsw), tptr, hln2 );
					
					SET_NORMAL;
					
					hln = hln2;
				}
				
				// draw everything that's highlighted (to end of line)
				canvas_show_text( WIDGET(cvsw), 2+hlx, (height) - (s * font_height), tptr+hln, b-hln );
			}
			else
			{
				if ( is_hl ) // highlighted completely
					SET_HIGHLT;
				else // not highlighted at all
					SET_NORMAL;
				
				canvas_show_text( WIDGET(cvsw), 2, (height) - (s * font_height), tptr, b );
			}
			
			tptr += b;
			
			if ( *tptr == 0 )
				break;
			
			a++;
			s--;
		}
	}
}


void window_closed( object_t *btn, event_t *event )
{
	exit(0);
}

int main( int argc, char *argv[] )
{
	object_t *w;
	layout_t *lt;
	object_t *btn = NULL;
	bounds_t *b = NULL;
	
	claro_base_init( );
	claro_graphics_init( );
	
	//log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	b = new_bounds(50, 50, 300, 300);
	
	w = window_widget_create(NULL, b, 0);
	window_set_title(w, "Canvas Speed Test");

	object_addhandler(w, "destroy", window_closed);
		
	lt = layout_create(w, "[][_<a|canvas|>a][]", *b, 10, 10 );
	assert(lt != NULL && "layout was failed to parse");
	   
	c = (widget_t *)canvas_widget_create( w, lt_bounds(lt, "canvas"), 0 );
	
	object_addhandler( OBJECT(c), "redraw", handle_redraw );
	
	object_addhandler( OBJECT(c), "clicked", handle_mousedown );
	object_addhandler( OBJECT(c), "released", handle_mouseup );
	object_addhandler( OBJECT(c), "mouse_moved", handle_mousemove );
	
	window_show( w );
	window_focus( w );
	
	claro_loop( );
	
	return 0;
}
