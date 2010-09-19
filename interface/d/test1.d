import claro.core;
import claro.base.all;
import claro.graphics.all;

import std.stdio;
import std.math;
import std.c.time;

class test_window : WindowWidget {
	CanvasWidget canvas;
	Layout lt;
	this( ) {
		super( null, new Bounds( 0, 0, 300, 300 ), 0 );
		
		this.title = "test";
		
		lt = new Layout( this, "[][_<|clock|<][]", 10, 10 );
		
		Claro.addToLoop( &this.handle_main );
		
		this.add_handler( "destroy", &this.closed );
		
		this.make_menu( );
		
		this.canvas = new CanvasWidget( this, lt.bounds("clock") );
		this.canvas.redrawDelegate = &this.draw_cb;
	}
	
	MenuBarWidget mb;
	void make_menu( ) {
		this.mb = new MenuBarWidget( this );
		auto file_menu = this.mb.appendItem( "File" );
		auto file_quit = this.mb.appendItem( file_menu, Stock.getImage("system-log-out"), "Quit" );
		this.mb.addKeyBinding( file_quit, "Q", MenuBarWidget.ModifierCommand );
		file_quit.add_handler( "pushed", &this.closed );
	}
	
	void draw_cb( CanvasWidget w, Context c ) {
		Bounds b = w.bounds;
		
		time_t rawtime;
		time( &rawtime );
		auto tm = localtime( &rawtime );
		
		c.scale( b.width, b.height );
		
		double minutes = tm.tm_min * PI / 30;
		double hours = tm.tm_hour * PI / 6;
		double seconds = tm.tm_sec * PI / 30;
		
		c.lineCap = LineCap.Round;
		c.lineWidth = 0.1;
		
		// translate to the center of the context
		c.translate( 0.5, 0.5 );
		
		// draw black outline
		c.setSourceRGBA( 0, 0, 0, 1 );
		c.arc( 0, 0, 0.4, 0, PI*2 );
		c.stroke( );
		
		// draw white dot on current second
		c.setSourceRGBA( 1, 1, 1, 0.6 );
		c.arc( sin(seconds) * 0.4, -cos(seconds) * 0.4, 0.05, 0, PI * 2 );
		c.fill( );
		
		// draw minutes
		c.setSourceRGBA( 0.2, 0.2, 1, 0.6 );
		c.moveTo( 0, 0 );
		c.lineTo( sin(minutes) * 0.4, -cos(minutes) * 0.4 );
		c.stroke( );

	    // draw hours		
	    c.moveTo( 0, 0 );
	    c.lineTo( sin(hours) * 0.2, -cos(hours) * 0.2 );
	    c.stroke( );
	}
	
	void closed( CEvent evt, CObject obj ) {
		Claro.shutdown( );
	}
	
	// Gets run every claro loop iteration. Every second, redraws canvas.
	void handle_main( CEvent evt, CObject obj ) {
		int t = time( null );
		static int last_time = 0;
		
		if ( t > last_time ) {
			last_time = t;
			
			this.canvas.redraw( );
		}
	}
}

void main( ) {
	Claro.init();
	Claro.Graphics.init();
	
	auto w = new test_window( );
	
	w.show( );
	w.focus( );

	Claro.loop();
}
