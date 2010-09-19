module claro.graphics.stock;

private import claro.base.objectsys;
private import claro.graphics.image;

extern (C)
{
	image_t *stock_get_image( char *stock_id );
}

class Stock {
	static Image getImage( char[] stock_id ) {
		return new Image( stock_get_image( std.string.toStringz(stock_id) ), null );
	}
}