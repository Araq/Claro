module claro.claro;

private import claro.base.base;
private import claro.base.objectsys;
private import claro.graphics.base;

import std.stdio;

class Claro
{
	static CObject claroo;
	
	static void init( )
	{
		claro_base_init( );
	}
	
	static void addToLoop( event_handler_t handler )
	{
		if ( Claro.claroo is null )
		{
			Claro.claroo = new CObject;
			Claro.claroo.obj = get_claro_object( );
		}

		Claro.claroo.add_handler( "mainloop", handler );
	}
	
	static void loop( )
	{
		claro_loop( );
	}
	
	static void shutdown( )
	{
		claro_shutdown( );
	}
	
	class Graphics
	{
		static void init( )
		{
			claro_graphics_init( );
		}
	}
}
