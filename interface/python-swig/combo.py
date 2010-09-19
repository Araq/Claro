from claro import *

def window_closed( o, e ):
	claro_shutdown( )

def combo_item_selected( o, e ):
	tmp = ''
	li = combo_get_selected( o )
	
	if li != None:
		tmp = li.get_string(0)
	
	textbox_set_text( tbox, tmp )

init( )
graphics_init( )

b = new_bounds( 100, 100, 200, 120 )
w = window_widget_create( cvar.claro, b, 0 )
w.addhandler( "destroy", window_closed )
window_set_title( w, "Combo example" )

lt = layout_create( w, "[][_<|frame|<][]", b, 10, 10 )

# create a frame
b = lt_bounds(lt,"frame")
fr = frame_widget_create_with_label( w, b, 0, "Combo example" )
lt2 = layout_create( fr, "[combo][{10}][text]", b, 25, 25 )

combo = combo_widget_create( fr, lt_bounds( lt2, "combo" ), 0 )
tbox  = textbox_widget_create( fr, lt_bounds( lt2, "text" ), 0 )

combo_append_row( combo, "True" )
combo_append_row( combo, "False" )
combo_append_row( combo, "NULL" )
combo_append_row( combo, "Candy" )

combo.addhandler( "selected", combo_item_selected )

window_show( w )
window_focus( w )

loop( )

