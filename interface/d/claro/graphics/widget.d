module claro.graphics.widget;

public import claro.base.objectsys;

struct bounds_t {
	int x, y;
	int w, h;
	
	object_t *owner;
}

struct widget_t {}

extern (C)
{
	bounds_t *new_bounds( int x, int y, int w, int h );
	bounds_t *get_req_bounds( object_t *widget );
}

class Bounds {
	bounds_t *obj;
	
	this( int x, int y, int w, int h ) {
		this.obj = new_bounds( x, y, w, h );
	}
	
	this( bounds_t *obj ) {
		this.obj = obj;
	}
	
	int x( ) { return this.obj.x; }
	int y( ) { return this.obj.y; }
	int w( ) { return this.obj.w; }
	int h( ) { return this.obj.h; }

	int left( ) { return this.obj.x; }
	int top( ) { return this.obj.y; }
	int width( ) { return this.obj.w; }
	int height( ) { return this.obj.h; }
}

class Widget : CObject {
	Bounds b;
	
	this( CObject parent ) {
		super( parent );
	}
	
	this() { super(); }
	
	void created( ) {
		
	}
	
	Bounds bounds( ) {
		return new Bounds( get_req_bounds(this.obj) );
	}
}
