module claro.graphics.widgets.menubar;

private import claro.graphics.widget;
private import claro.graphics.listitem;
private import claro.graphics.image;

extern (C)
{
	object_t *menubar_widget_create( object_t *parent, int flags );
	list_item_t *menubar_append_item( object_t *menubar, list_item_t *parent, image_t *image, char *title );
	void menubar_add_key_binding( object_t * menubar, list_item_t * item, char * utf8_key, int modifier );
}

class MenuBarWidget : Widget {
	static ModifierShift = 1 << 0;
	static ModifierCommand = 1 << 1;
	
	this( CObject parent, int flags ) {
		super( parent );
		
		this.obj = menubar_widget_create( CObject.getObj(parent), flags );
		this.created( );
	}
	
	this( CObject parent ) {
		this( parent, 0 );
	}
	
	ListItem appendItem( ListItem parent, Image im, char[] title ) {
		auto obj = menubar_append_item( this.obj, cast(list_item_t*) CObject.getObj(parent), cast(image_t*) CObject.getObj(im), std.string.toStringz(title) );
		return new ListItem( obj, parent );
	}
	
	ListItem appendItem( ListItem parent, char[] title ) {
		return this.appendItem( parent, null, title );
	}
	
	ListItem appendItem( char[] title ) {
		return this.appendItem( null, null, title );
	}
	
	ListItem appendItem( Image im, char[] title ) {
		return this.appendItem( null, im, title );
	}
	
	void addKeyBinding( ListItem item, char[] key, int modifiers ) {
		menubar_add_key_binding( this.obj, cast(list_item_t*) item.obj, std.string.toStringz(key), modifiers );
	}
}