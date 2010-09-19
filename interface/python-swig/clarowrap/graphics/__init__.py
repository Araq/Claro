import claro, clarowrap


def init( ):
	claro.graphics_init( )

def image_load( parent, filename ):
	return claro.image_load( parent.obj, filename )


class bounds:
	def __init__( self, x, y, w, h, b=None ):
		if b != None:
			self.obj = b
			return
		self.obj = claro.new_bounds( x, y, w, h )


class layout:
	def __init__( self, widget, fmt, bounds, defw, defh ):
		self.obj = claro.layout_create( widget.obj, fmt, bounds.obj, defw, defh )
		
	def get_bounds( self, name ):
		return bounds( 0, 0, 0, 0, claro.lt_bounds( self.obj, name ) )


class widget( clarowrap.base.cobject ):
	def __init__( self, parent ):
		self.obj = None
	
	def created( self ):
		pass

class stock:
	@staticmethod
	def get_image( stock_id ):
		return claro.stock_get_image( stock_id )
		
	@staticmethod
	def add_image( stock_id, img ):
		claro.stock_add_image( stock_id, img )	


class window( widget ):
	def __init__( self, parent, bounds, flags=0 ):
		self.parent = parent
		if parent:
			parent = parent.obj
		self.bounds = bounds
		self.obj = claro.window_widget_create( parent, bounds.obj, flags )
		self.find_event_handlers( )
		self.created( )
	
	def set_title( self, title ):
		claro.window_set_title( self.obj, title )
	
	def set_icon( self, icon ):
		claro.window_set_icon( self.obj, icon )
		
	def show( self ):
		claro.window_show( self.obj )
	
	def focus( self ):
		claro.window_focus( self.obj )


class frame( widget ):
	def __init__( self, parent, bounds, flags=0, label="" ):
		self.parent = parent
		if parent:
			parent = parent.obj
		self.obj = claro.frame_widget_create_with_label( parent, bounds.obj, flags, label )
		self.find_event_handlers( )
		self.created( )


class list_item( clarowrap.base.cobject ):
	def __init__( self, obj, parent ):
		self.obj = obj
		self.parent = parent


class canvas( widget ):
	def __init__( self, parent, bounds, flags=0 ):
		self.parent = parent
		if parent:
			parent = parent.obj
		self.obj = claro.canvas_widget_create( parent, bounds.obj, flags )
		self.find_event_handlers( )
		self.created( )

	def redraw( self ):
		claro.canvas_redraw( self.obj )

	def fill_rect( self, x, y, w, h, r, g, b, a ):
		claro.canvas_fill_rect( self.obj, x, y, w, h, r, g, b, a )


class menubar( widget ):
	
	ModifierShift = claro.cModifierShift
	ModifierCommand = claro.cModifierCommand
	
	def __init__( self, parent, flags=0 ):
		self.parent = parent
		if parent:
			parent = parent.obj
		self.obj = claro.menubar_widget_create( parent, flags )
		self.find_event_handlers( )
		self.created( )
		
	def add_key_binding( self, item, utf8_key, mod ):
		claro.menubar_add_key_binding( self.obj, item.obj, utf8_key, mod )	

	def append_item( self, parent, img, label ):
		if parent:
			parent_obj = parent.obj
		else:
			parent_obj = None	
		obj = claro.menubar_append_item( self.obj, parent_obj, img, label )
		return list_item( obj, parent )

	def append_separator( self, parent ):
		if parent:
			parent_obj = parent.obj
		else:
			parent_obj = None	
		obj = claro.menubar_append_separator( self.obj, parent_obj )
		return list_item( obj, parent )
			
			
class toolbar( widget ):		
	
	ToolbarShowText = claro.cToolbarShowText
	ToolbarShowImages = claro.cToolbarShowImages
	ToolbarShowBoth = claro.cToolbarShowBoth
	ToolbarAutoSizeButtons = claro.cToolbarAutoSizeButtons
	
	def __init__( self, parent, flags=0 ):
		self.parent = parent
		if parent:
			parent = parent.obj		
		self.obj = claro.toolbar_widget_create( parent, flags )
		self.find_event_handlers( )	
		self.created( )
		
	def append_icon( self, img, label, tooltip ):
		if not label:
			label = ""
		if not tooltip:
			tooltip = ""	
		obj = claro.toolbar_append_icon(self.obj, img, label, tooltip )
		return list_item( obj, self )
			
class textbox( widget ):
	def __init__( self, parent, bounds, flags=0, label="" ):
		self.parent = parent
		if parent:
			parent = parent.obj
		self.obj = claro.textbox_widget_create( parent, bounds.obj, flags )
		self.find_event_handlers( )
		self.created( )
	def set_text( self, text ):
		claro.textbox_set_text( self.obj, text )


class combo( widget ):
	def __init__( self, parent, bounds, flags=0 ):
		self.parent = parent
		if parent:
			parent = parent.obj
		self.obj = claro.combo_widget_create( parent, bounds.obj, flags )
		self.find_event_handlers( )
		self.created( )
	def append( self, text ):
		return claro.combo_append_row( self.obj, text )
	def selected( self ):
		return claro.combo_get_selected( self.obj )
		
		
