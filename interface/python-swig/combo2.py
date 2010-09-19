import clarowrap
claro = clarowrap

class combo_window( claro.graphics.window ):
	def created( self ):
		self.set_title( "Combo Example" )
		self.add_handler("destroy", self.window_closed)

		self.lt = claro.graphics.layout( self, "[][_<|frame|<][]", self.bounds, 10, 10 )
		
		b = self.lt.get_bounds( "frame" )
		self.fr = claro.graphics.frame( self, b, 0, "Combo example" )
		
		self.lt2 = claro.graphics.layout( self.fr, "[combo][{10}][text]", b, 25, 25 )
		
		self.combo = claro.graphics.combo( self.fr, self.lt2.get_bounds( "combo" ), 0 )
		self.tbox  = claro.graphics.textbox( self.fr, self.lt2.get_bounds( "text" ), 0 )
		
		self.combo.add_handler( "selected", self.combo_item_selected )
		
		self.combo.append( "True" )
		self.combo.append( "False" )
		self.combo.append( "NULL" )
		self.combo.append( "Candy" )
	
	def combo_item_selected( self, obj, evt ):
		tmp = ''
		li = self.combo.selected( )

		if li != None:
			tmp = li.get_string(0)

		self.tbox.set_text( tmp )
	

	def window_closed( self, o, e ):
		claro.base.shutdown( )

claro.base.init( )
claro.graphics.init( )

b = claro.graphics.bounds( 100, 100, 200, 120 )
w = combo_window( None, b )

w.show( )
w.focus( )

claro.base.loop( )
