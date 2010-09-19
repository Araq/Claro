module claro.graphics.image;

private import claro.base.objectsys;

//struct image_t {}
alias object_t image_t;

extern (C)
{
	image_t *image_load( object_t *parent, char *file );
}

class Image : CObject {
	this( image_t *me, CObject parent ) {
		super( parent );
		this.obj = me;
	}
	
	this( CObject parent, char[] filename ) {
		super( parent );
		
		this.obj = image_load( CObject.getObj(parent), std.string.toStringz(filename) );
	}
	
	this( char[] filename ) {
		this( null, filename );
	}
}
