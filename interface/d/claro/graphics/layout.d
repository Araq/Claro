module claro.graphics.layout;

private import claro.base.objectsys;
private import claro.graphics.widget;

import std.string;

struct layout_t {}

extern (C)
{
	layout_t *layout_create( object_t *parent, char *layout_spec, bounds_t bounds, int min_w, int min_h );
	bounds_t *lt_bounds( layout_t *lt, char *name );
}

class Layout : CObject {
	layout_t *lt;
	
	this( CObject parent, char[] layout_spec, Bounds b, int min_width, int min_height )
	{
		super( parent );
		
		// FIXME: the layout's use of *b.obj obviously means a likely memory leak.
		// when a new bounds is passed just for this purpose, its .obj will not be freed.
		
		this.lt = layout_create( CObject.getObj(parent), std.string.toStringz(layout_spec), *b.obj, min_width, min_height );
		this.obj = cast(object_t*)this.lt;
	}
	
	this( CObject parent, char[] layout_spec, int min_width, int min_height )
	{
		// for convenience, we can use null bounds. just seems like a good idea,
		// in most cases it doesn't matter because the parent widget will adjust
		// our bounds anyway.
		
		static Bounds b = null;
		
		if ( b is null ) {
			b = new Bounds( 0, 0, 0, 0 );
		}
		
		this( parent, layout_spec, b, min_width, min_height );
	}
	
	Bounds bounds( char[] name )
	{
		return new Bounds( lt_bounds( this.lt, std.string.toStringz(name) ) );
	}
}
