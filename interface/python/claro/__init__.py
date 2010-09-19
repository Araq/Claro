import claro.wrapper.base
import claro.wrapper.graphics
from claro import wrapper

__all__ = ['wrapper']

# claro.base interface class
class base_class:
	CL_ANY = 0
	CL_DEBUG = 1
	CL_INFO = 2
	CL_WARNING = 3
	CL_ERROR = 4
	CL_CRITICAL = 5
	
	class cobject:
		def __init__( self ):
			self.obj = 0
		
		def addhandler( self, name, func ):
			pass
	
	def init( self ): claro.wrapper.base.init( )
	def log( self, a, b ): claro.wrapper.base.log( a, b )
	def loop( self ): claro.wrapper.base.loop( )

# claro.graphics interface classes

class graphics_class:
	class widget( base_class.cobject ):
		def __init__( self ):
			pass
			
		def show( self ):
			claro.wrapper.graphics.widget_show( self.obj )
			
		def hide( self ):
			claro.wrapper.graphics.widget_hide( self.obj )
		
		def focus( self ):
			claro.wrapper.graphics.widget_focus( self.obj )
	
	class window( widget ):
		def __init__( self, p, x, y, w, h, f ):
			if p == None:
				p = 0
			else:
				p = p.obj
			
			self.obj = claro.wrapper.graphics.window_widget_create( p, x, y, w, h, f )
		
		def show( self ):
			claro.wrapper.graphics.window_show( self.obj )
			
		def hide( self ):
			claro.wrapper.graphics.window_hide( self.obj )
		
		def focus( self ):
			claro.wrapper.graphics.window_focus( self.obj )
		
		def set_title( self, title ):
			pass
	
	class button( widget ):
		def __init__( self, p, x, y, w, h, f ):
			if p == None:
				p = 0
			else:
				p = p.obj
			
			self.obj = claro.wrapper.graphics.button_widget_create( p, x, y, w, h, f )
		
		def set_text( self, text ):
			claro.wrapper.graphics.button_set_text( self.obj, text )
	
	class textbox( widget ):
		def __init__( self, p, x, y, w, h, f ):
			if p == None:
				p = 0
			else:
				p = p.obj
			
			self.obj = claro.wrapper.graphics.textbox_widget_create( p, x, y, w, h, f )
		
		def set_text( self, text ):
			claro.wrapper.graphics.textbox_set_text( self.obj, text )
	
	def init( self ): claro.wrapper.graphics.init( )
	#def window( self, p, x, y, w, h, f ): return window()claro.wrapper.graphics.window_widget_create( p, x, y, w, h, f )

base = base_class( )
graphics = graphics_class( )