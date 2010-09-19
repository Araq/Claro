import claro

## Found this page while writing the code here:
## http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/223613
## May help if someone wants to use functions instead of bound methods?
	
class cobject:
	def __init__( self, parent ):
		self.obj = None
		
	def get_type( self ):
		return self.obj.type
			
	def get_children( self ):
		l = []
		raw = self.obj.children.get_items()
		for child in raw:
			obj = cobject(self.obj)
			obj.obj = child
			l.append(obj)
		return l	
	
	def add_handler( self, name, handler ):
		self.obj.addhandler( name, handler )
	
	def handle_event( self, obj, event ):
		getattr( self, 'on_'+event.name )( event )
	
	def find_event_handlers( self ):
		funcs = dir( self )
		for f in funcs:
			if f[:3] != 'on_':
				continue
			self.add_handler( f[3:], self.handle_event )

def init( ):
	claro.init( )

def loop( ):
	claro.loop( )

def shutdown( ):
	claro.claro_shutdown( )
