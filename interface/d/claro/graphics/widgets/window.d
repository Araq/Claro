module claro.graphics.widgets.window;

private import claro.graphics.widget;

extern (C)
{
	object_t *window_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void window_show( object_t * );
	void window_focus( object_t * );
	void window_set_title( object_t*, char *title );
}

class WindowWidget : Widget {
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = window_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void show( ) {
		window_show( this.obj );
	}
	
	void focus( ) {
		window_focus( this.obj );
	}
	
	void title( char[] title ) {
		window_set_title( this.obj, std.string.toStringz(title) );
	}
}
