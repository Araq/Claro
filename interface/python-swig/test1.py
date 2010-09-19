import clarowrap as claro
import claro as _claro
cg = claro.graphics
cb = claro.base

class test_window(cg.window):
	def created(self):
		self.set_title("test")
		img = cg.image_load(self, "icon.png")
		self.set_icon(img)
		self.add_handler("destroy", self.close_cb)
		
		self.make_menu()
		self.make_toolbar()
		for child in self.get_children():
			print child.get_type()
		
		self.canvas = cg.canvas(self, cg.bounds(0,0,300,300))
		self.add_handler("redraw", self.draw_cb)
		self.canvas.redraw()
		
	def draw_cb(self,o,e):
		print "draw()"
		self.canvas.fill_rect(0,0,200,200,0,0,0,1)	
		
	def make_menu(self):
		self.mb = cg.menubar(self)
		file_menu = self.mb.append_item(None, None, "File")
		file_quit = self.mb.append_item(file_menu, cg.stock.get_image("system-log-out"), "Quit")
		self.mb.add_key_binding(file_quit, "Q", cg.menubar.ModifierCommand)
		file_quit.add_handler("pushed", self.close_cb)
	
	def make_toolbar(self):
		self.tb = cg.toolbar(self)
		prev = self.tb.append_icon(cg.stock.get_image("go-previous"),"None","Previous")
		next = self.tb.append_icon(cg.stock.get_image("go-next"),None,"Next")
		
	def close_cb(self,o,e):
		claro.base.shutdown()
		

cb.init()
cg.init()

w = test_window(None,cg.bounds(0,0,300,300))

w.show()
w.focus()

cb.loop()
